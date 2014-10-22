/*
 * lpmStateMachine.c (subset of stateMachine.c)
 *
 * Model the state of the device for low power management.  The states
 * can move adjacent to one another in sequence (meaning don't jump around)
 *
 * Each state has a set of operations:
 *
 * LPM_NORMAL_STATE
 *   - nothing to do
 *
 * LPM_BEGIN_STATE
 *   - notify CommMgr
 *       - creates "loss of power message" and queue's it for delivery to the server (via UDP)
 *   - notify NetworkMgr
 *       - stops bband (wifi and/or ethernet)
 *   - notify AudioMgr
 *       - stop playing sound
 *   - notify ZigbeeMgr
 *   - run equivalent to suspend_per (and kills the screen)
 *
 * LPM_TEAR_DOWN_STATE
 *   - notify CommMgr
 *   - notify NetworkMgr
 *       - stop gprs
 *   - notify ZigbeeMgr
 *   - run equivalent to suspend_gprs
 *
 * LPM_STANDBY_STATE
 *   - notify ZigbeeMgr
 *
 * LPM_SUSPEND_STATE
 *   - lowPowerMgr runs the 'suspend cpu' logic
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

#include <hal/lpmHW.h>
#include <hal/battery.h>
#include "lpmStateMachine.h"
#include "lpmTest.h"

/* string values for the states (for debugging) */
#define LPM_NORMAL_STATE_STR        "NORMAL_STATE"
#define LPM_PREP_STATE_STR          "PREP_STATE"
#define LPM_BEGIN_STATE_STR         "BEGIN_STATE"
#define LPM_TEAR_DOWN_STATE_STR     "TEAR_DOWN_STATE"
#define LPM_STANDBY_STATE_STR       "STANDBY_STATE"
#define LPM_SUSPEND_STATE_STR       "SUSPEND_STATE"

/* amount of time to wait (in seconds) */
#define STANDARD_WAIT_TIME      60
#define ALARM_WAIT_TIME         (7 * 60)	/* allow for cancel window and messages to sent */

typedef enum {
    TRANS_IN,       /* entering the state */
    TRANS_OUT       /* exiting the state */
} transition;

typedef struct _monitorArgs {
    hal_lpm_state_t transTo;
    int seconds;
} monitorArgs;

/* forward declarations of private functions */
int moveToState(hal_lpm_state_t newState, hal_lpm_action_t action);
int transitionToPrepState(transition trans);
int transitionToBeginState(transition trans);
int transitionToTearDownState(transition trans);
int transitionToStandbyState(transition trans);
void transitionToSuspendState();
void startMonitorThread(hal_lpm_state_t transTo, int seconds);
int getUserEventCounts(long *buttonPressCount, long *screenPressCount);

/* global variables */
static int lowSeqRunning = 0;
static int lowSeqCancel = 0;
static int normSeqRunning = 0;
static int normSeqCancel = 0;
static int monitorRunning = 0;
static long	buttonCount = 0;
static long screenCount = 0;

static hal_lpm_state_t currentState = LPM_NORMAL_STATE;

static pthread_mutex_t stateMTX = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;    // held when changing/examining states
static pthread_mutex_t runMTX   = PTHREAD_MUTEX_INITIALIZER;


/*
 * init the state machine
 */
void initStateMachine()
{
	logDebugMsg("lpmTest: Initializing state machine.\n");
    // init flags
    currentState = LPM_NORMAL_STATE;
    lowSeqRunning = 0;
    lowSeqCancel = 0;
    normSeqRunning = 0;
    normSeqCancel = 0;

    // see if we started off with the AC unplugged
    //
    if (hal_battery_is_power_charger_attached() == 0)
    {
    	  logDebugMsg("lpmTest: Power not attached. Start low power sequence.\n\n");
    	  printf("[LPM-Test]  Power not attached. Begin the Low Power sequence\n");
          startLowPowerSequence();
    }
    else
    {
    	  logDebugMsg("lpmTest: Power attached. State machine initialized.\n");
     	  printf("[LPM-Test]  Power attached. Ready to begin test.\n\n");
    }
}

/*
 * returns 1 if we're in low power mode.  0 if not
 */
int isInLPM()
{
    // first see if one of our sequences is running (or monitor thread).
    // if so, we can assume we're CURRENTLY in low power mode
    //
    pthread_mutex_lock(&runMTX);
    if (lowSeqRunning != 0 || normSeqRunning != 0 || monitorRunning != 0)
    {
        pthread_mutex_unlock(&runMTX);
        return 1;
    }
    pthread_mutex_unlock(&runMTX);

    // probably save to get the current state
    //
    if (getCurrentState() != LPM_NORMAL_STATE)
    {
        return 1;
    }

    return 0;
}

/*
 * similar to isInLPM(), but specific to screen state.
 * returns 1 if we're in low power mode and the screen should
 * be off.  0 if not
 */
int isInDarkLPM()
{
    // get the current state.  If we're greater then tear down
    // then the screen is turned off.
	//
    if (getCurrentState() >= LPM_TEAR_DOWN_STATE)
    {
        return 1;
    }

	return 0;
}

/*
 * return the current state of the machine
 * this can take a while if the state is currently
 * being transitioned.
 */
hal_lpm_state_t getCurrentState()
{
    int rc = -1;

    pthread_mutex_lock(&stateMTX);
    rc = currentState;
    pthread_mutex_unlock(&stateMTX);

    return rc;
}

/*-------------------------------------
 *
 * Low Power Sequence
 *
 *-------------------------------------*/

/*
 * thread to go into low power mode
 */
static void* enterLowPowerThread(void* arg)
{
    // step forward through the machine until we reach 'suspend'
    //
    pthread_mutex_lock(&stateMTX);
    logDebugMsg("lpmTest: starting 'low power' sequence\n");
    printf("[LPM-Test]  Low Power sequence initiated.\n");    

    hal_lpm_state_t curr = currentState;
    int rc = 0;
    while (curr < LPM_SUSPEND_STATE && rc == 0)
    {
        // see if told to cancel
        //
        pthread_mutex_lock(&runMTX);
        if (lowSeqCancel == 1)
        {
        	logDebugMsg("lpmTest: told to cancel 'low power' sequence'");
            pthread_mutex_unlock(&runMTX);
            break;
        }
        pthread_mutex_unlock(&runMTX);

        // increment from where we are currently at
        // then move to that new state
        //
        curr++;
        rc = moveToState(curr, SLEEP);

        if (curr >= LPM_SUSPEND_STATE)
        {
            // went into, then back out of SUSPEND state.
            // need to go back 1 to STANDBY so zigbee can
            // process stuff
            //
            startMonitorThread(LPM_TEAR_DOWN_STATE, STANDARD_WAIT_TIME);
            break;
        }
    }

    // cleanup running flags
    //
    pthread_mutex_lock(&runMTX);
    lowSeqRunning = 0;
    lowSeqCancel = 0;
    pthread_mutex_unlock(&runMTX);

    // release mutex now that we're done (or canceled)
    //
    logDebugMsg("lpmTest: done with 'low power' sequence'");
    pthread_mutex_unlock(&stateMTX);

    return NULL;
}

/*
 * Starts the sequence of state changes to enter low power mode.
 * Called when we receive the "AC unplugged" event.
 */
int startLowPowerSequence()
{
    pthread_attr_t tattr;
    pthread_t tid;
    logDebugMsg("lpmTest: startLowPowerSequence");

    // see if a sequence is already running
    //
    pthread_mutex_lock(&runMTX);
    if (lowSeqRunning == 1 || normSeqRunning == 1)
    {
    	logDebugMsg("lpmTest: unable to start 'low power' sequence, a sequence is running");
        pthread_mutex_unlock(&runMTX);
        return -1;
    }

    // set the run flag
    //
    lowSeqRunning = 1;
    lowSeqCancel = 0;

    // Start a thread to begin the sequence, this way the caller can continue
    // to process events.  Hold onto the run mutex until we launch our thread
    // (to help reduce cancel before it starts)
    //
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &tattr, enterLowPowerThread, NULL);

    pthread_mutex_unlock(&runMTX);
    return 0;
}

/*
 * stops the current sequence
 */
void cancelLowPowerSequence()
{
	logDebugMsg("lpmTest: cancelLowPowerSequence");
    // set the cancel flag
    //
    pthread_mutex_lock(&runMTX);
    if (lowSeqRunning == 1)
    {
    	logDebugMsg("lpmTest: canceling the current 'low power' sequence");
        lowSeqCancel = 1;
    }
    pthread_mutex_unlock(&runMTX);

    // wait for it to stop
    //
    while (isLowPowerSequenceRunning() == 1)
    {
        sleep(1);
    }
}

/*
 * returns 1 if the sequence is running
 */
int isLowPowerSequenceRunning()
{
    int rc = 0;

    pthread_mutex_lock(&runMTX);
    rc = lowSeqRunning;
    pthread_mutex_unlock(&runMTX);

    return rc;
}


/*-------------------------------------
 *
 * Normal Power Sequence
 *
 *-------------------------------------*/

/*
 * thread to go into normal power mode
 */
static void* enterNormalPowerThread(void* arg)
{
    // step backward through the machine until we reach 'normal'
    //
    pthread_mutex_lock(&stateMTX);
    logDebugMsg("lpmTest: enterNormalPowerThread: starting 'normal power' sequence'");
    hal_lpm_state_t curr = currentState;
    int rc = 0;
    while (curr > LPM_NORMAL_STATE && rc == 0)
    {
        // see if told to cancel
        //
        pthread_mutex_lock(&runMTX);
        if (normSeqCancel == 1)
        {
        	logDebugMsg("lpmTest: told to cancel 'normal power' sequence'");
            pthread_mutex_unlock(&runMTX);
            break;
        }
        pthread_mutex_unlock(&runMTX);

        // decrement from where we are currently at
        // then move to that new state
        //
        curr--;
        rc = moveToState(curr, RESTORE);
    }

    // cleanup running flags
    //
    pthread_mutex_lock(&runMTX);
    normSeqRunning = 0;
    normSeqCancel = 0;
    pthread_mutex_unlock(&runMTX);

    // release mutex now that we're done (or canceled)
    //
    logDebugMsg("lpmTest: done with 'normal power' sequence'");
    pthread_mutex_unlock(&stateMTX);

    return NULL;
}


/*
 * Starts the sequence of state changes to leave low power mode.
 * Called when we receive the "AC restored" event.
 */
int startNormalPowerSequence()
{
    pthread_attr_t tattr;
    pthread_t tid;

    logDebugMsg("lpmTest: startNormalPowerSequence");

    // see if a sequence is already running
    //
    pthread_mutex_lock(&runMTX);
    if (lowSeqRunning == 1 || normSeqRunning == 1)
    {
    	logDebugMsg("lpmTest: unable to start 'normal power' sequence, a sequence is running");
        pthread_mutex_unlock(&runMTX);
        return -1;
    }

    // set the run flag
    //
    normSeqRunning = 1;
    normSeqCancel = 0;

    // Start a thread to begin the sequence, this way the caller can continue
    // to process events.  Hold onto the run mutex until we launch our thread
    // (to help reduce cancel before it starts)
    //
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &tattr, enterNormalPowerThread, NULL);

    pthread_mutex_unlock(&runMTX);
    return 0;
}

/*
 * stops the current sequence
 */
void cancelNormalPowerSequence()
{
	logDebugMsg("lpmTest: cancelNormalPowerSequence");
    // set the cancel flag
    //
    pthread_mutex_lock(&runMTX);
    if (normSeqRunning == 1)
    {
    	logDebugMsg("lpmTest: canceling the 'normal power' sequence");
        normSeqCancel = 1;
    }
    pthread_mutex_unlock(&runMTX);

    // wait for it to stop
    //
    while (isNormalPowerSequenceRunning() == 1)
    {
        sleep(1);
    }
}

/*
 * returns 1 if the sequence is running
 */
int isNormalPowerSequenceRunning()
{
    int rc = 0;

    pthread_mutex_lock(&runMTX);
    rc = normSeqRunning;
    pthread_mutex_unlock(&runMTX);

    return rc;
}


/*-------------------------------------
 *
 * Helper Functions
 *
 *-------------------------------------*/

/*
 * similar to startNormalPowerSequence, but don't go to the top
 * and do not allow something to cancel the operation
 */
void temporaryStateChange(hal_lpm_state_t desiredState)
{
    pthread_mutex_lock(&stateMTX);
    logDebugMsg("lpmTest: temporarily restoring toward '%s' state", getStateStr(desiredState));
    hal_lpm_state_t curr = currentState;
    int rc = 0;
    while (curr > desiredState && rc == 0)
    {
        // decrement from where we are currently at
        // then move to that new state
        //
        curr--;
        rc = moveToState(curr, RESTORE);
    }
    logDebugMsg("lpmTest: done with temporarily restoring to '%s' state", getStateStr(desiredState));
    pthread_mutex_unlock(&stateMTX);
}

/*
 * thread to monitor the state of the system and go back into "low power mode" or
 * out to "normal mode".
 */
static void* monitorThread(void* arg)
{
    monitorArgs *info = (monitorArgs *)arg;
    int rc = 0;
    int totalWait = 0;
    int waitSecs = info->seconds;
    int inAlarm = 0;
    long button = 0;
    long screen = 0;

    // first move upward to the state we desire
    //
    temporaryStateChange(info->transTo);

    // wait up to 'info->seconds' seconds for an alarm to occur
    // or for power to be restored.
    //
    logDebugMsg("lpmTest: monitorThread: started, waiting up to %d seconds", waitSecs);
    while (totalWait < waitSecs)
    {
    	logDebugMsg("lpmTest: monitorThread: ---------> total=%d wait=%d", totalWait, waitSecs);
        // see if we're still in low power mode
        //
        if (hal_battery_is_power_charger_attached() > 0)
        {
            // nothing more to do
            //
        	logDebugMsg("lpmTest: monitorThread: charger attached, bailing");
            break;
        }

        // wait a couple of seconds to let things progress
        //
        sleep(2);
        totalWait += 2;
        logDebugMsg("lpmTest: After sleep, totalWait is %d, waitSecs is %d",totalWait,waitSecs);

        // see if the user has been pressing buttons or the screen (which could be why we're not in suspend mode)
        //
        if (totalWait > 2 && getUserEventCounts(&button, &screen) == 0)
        {
        	int goBegin = 0;

        	// compare to the static values to see if it's changed
        	//
        	pthread_mutex_lock(&stateMTX);
        	if (button > buttonCount)
        	{
        		// button was pressed, so reset the counter.  we'll leave
        		// the 'wait time' the same since we don't know if we're in
        		// alarm mode or not
        		//
        		logDebugMsg("lpmTest: monitorThread: user pressed a button, resetting the %d timer", waitSecs);
        		buttonCount = button;
        		totalWait = 0;
                adjustScreenPower(SCREEN_ON);

                goBegin = 1;
        	}
        	if (screen > screenCount)
        	{
        		logDebugMsg("lpmTest: monitorThread: user pressed the screen, resetting the %d timer", waitSecs);
        		screenCount = screen;
        		totalWait = 0;
                goBegin = 1;
        	}
        	pthread_mutex_unlock(&stateMTX);

        	if (goBegin > 0)
        	{
        		// done here AFTER we release the lock
        		//
        		temporaryStateChange(LPM_BEGIN_STATE);
        	}
        }
    }

    // if we got here, then our time has elapsed.  The system
    // should not still be in alarm and we do not have power charger attached
    //
    if (hal_battery_is_power_charger_attached() == 0)
    {
    	logDebugMsg("lpmTest: monitorThread: system needs to go back into deep sleep (after waiting %d seconds)", totalWait);
        startLowPowerSequence();
    }

    // reset flag
    //
    pthread_mutex_lock(&runMTX);
    monitorRunning = 0;
    pthread_mutex_unlock(&runMTX);

    // mem cleanup
    //
    free(info);

    return NULL;
}

/*
 *
 */
void startMonitorThread(hal_lpm_state_t transTo, int seconds)
{
	logDebugMsg("lpmTest: startMonitorThread");
    // see if a monitoring thread is already running
    //
    pthread_mutex_lock(&runMTX);
    if (monitorRunning != 0)
    {
    	logDebugMsg("lpmTest: not starting a monitor thread, one already running");
        pthread_mutex_unlock(&runMTX);
        return;
    }
    monitorRunning = 1;
    pthread_mutex_unlock(&runMTX);

    // start the thread
    //
    pthread_attr_t tattr;
    pthread_t tid;
    monitorArgs *args = (monitorArgs *)malloc(sizeof(monitorArgs));

    memset(args, 0, sizeof(args));
    args->transTo = transTo;
    args->seconds = seconds;

    logDebugMsg("lpmTest: starting the monitor thread");
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &tattr, monitorThread, args);
}

/**
 * extract the content of a file as an int
 */
long readCountFile(const char *fileName)
{
	long retVal = -1;
	char buffer[64];

	// open the file
	//
	FILE *target = fopen(fileName, "r");
	if (target == (FILE *)0)
	{
		return retVal;
	}

	// read it's contents
	//
	if (fgets(buffer, 64, target) == (char *)0)
	{
		fclose(target);
		return retVal;
	}

	// convert to a number
	//
	retVal = atol(buffer);

	fclose(target);
	return retVal;
}

/*
 * gather the number of button presses and touch screen events
 */
int getUserEventCounts(long *buttonPressCount, long *screenPressCount)
{
	int rc = 0;
	int exitVal = 0;
	int retVal = -1;

	// TODO: put this into an abstraction so that different hardware
	//       handles this differently.  For now, we're going direct...
	//

	// first, get the button press count
	//
	logDebugMsg("lpmTest: getting number of button presses...");
	rc = system("grep mxckpd /proc/interrupts | awk '{print $2}' > /tmp/count");
	exitVal = WEXITSTATUS(rc);
	if (exitVal == 0)
	{
		// get the count from the file
		//
		long temp = readCountFile("/tmp/count");
		logDebugMsg("lpmTest: -----> count file == %ld", temp);
		if (temp >= 0)
		{
			*buttonPressCount = temp;
			retVal = 0;
		}
	}

	// now get the screen touch count
	//
	logDebugMsg("lpmTest: getting number of screen presses...");
	rc = system("grep TMA340 /proc/interrupts | awk '{ print $2 }' > /tmp/count");
	exitVal = WEXITSTATUS(rc);
	if (exitVal == 0)
	{
		// get the count from the file
		//
		long temp = readCountFile("/tmp/count");
		logDebugMsg("lpmTest: -----> count file == %ld", temp);
		if (temp >= 0)
		{
			*screenPressCount = temp;
			retVal = 0;
		}
	}

	return retVal;
}

/*
 * internal function to move from one state to the next
 * assumes caller has lock on 'stateMTX'.  Each all to
 * this can take quite a while to complete (several seconds)
 */
int moveToState(hal_lpm_state_t newState, hal_lpm_action_t action)
{
    int rc = -1;

    logDebugMsg("lpmTest: moveToState");
    // look at our current state and see if it's adjacent to 'newState'
    //
    if (action == RESTORE)
    {
        // tying to move toward NORMAL
        //
        if ((currentState - 1) != newState)
        {
        	logDebugMsg("lpmTest: unable to skip states.  Attempted to go directly (RESTORE) from %s to %s", getStateStr(currentState), getStateStr(newState));
            return rc;
        }
    }
    else // action == SLEEP
    {
        // tying to move toward SUSPEND
        //
        if ((currentState + 1) != newState)
        {
        	logDebugMsg("lpmTest: unable to skip states.  Attempted to go directly (SLEEP) from %s to %s", getStateStr(currentState), getStateStr(newState));
            return rc;
        }
    }

    // good to go, so transition from currentState to newState
    // handle differently based on the 'hal_lpm_action_t'
    //
    if (action == SLEEP)
    {
        // tying to move toward SUSPEND
        //
        switch (newState)
        {
            case LPM_NORMAL_STATE:
            {
                // going into NORMAL?  can't happen since there's nothing before it
                //
                break;
            }
            case LPM_PREP_STATE:
            {
            	// going from NORMAL to PREP
            	//
            	rc = transitionToPrepState(TRANS_IN);
            	break;
            }
            case LPM_BEGIN_STATE:
            {
                // going from PREP to BEGIN
                //
                rc = transitionToBeginState(TRANS_IN);
                break;
            }
            case LPM_TEAR_DOWN_STATE:
            {
                // going from BEGIN to TEAR_DOWN
                //
                rc = transitionToTearDownState(TRANS_IN);
                break;
            }

            case LPM_STANDBY_STATE:
            {
                // going from TEAR_DOWN to STANDBY
                //
                rc = transitionToStandbyState(TRANS_IN);
                break;
            }

            case LPM_SUSPEND_STATE:
            {
                // going into SUSPEND.  we don't come back from this
                // unless the CPU comes out of sleep mode.  Therefore,
                // save the state FIRST.
                //
                currentState = newState;
                transitionToSuspendState();

                // if we're here, we are no longer in suspend
                // return -1 so the calling loop stops
                //
                return -1;
            }
        }
    }
    else // action == RESTORE
    {
        // trying to move toward NORMAL
        //
        switch (currentState)
        {
            case LPM_NORMAL_STATE:
            {
                // going less then NORMAL?  can't happen since there's nothing before it
                //
                break;
            }
            case LPM_PREP_STATE:
            {
            	// going from PREP to NORMAL
            	//
            	rc = transitionToPrepState(TRANS_OUT);
            	break;
            }
            case LPM_BEGIN_STATE:
            {
                // going from BEGIN to PREP
                //
                rc = transitionToBeginState(TRANS_OUT);
                break;
            }
            case LPM_TEAR_DOWN_STATE:
            {
                // going from TEAR_DOWN to BEGIN
                //
                rc = transitionToTearDownState(TRANS_OUT);
                break;
            }
            case LPM_STANDBY_STATE:
            {
                // going from SUSPEND to STANDBY
                //
                rc = transitionToStandbyState(TRANS_OUT);
                break;
            }
            case LPM_SUSPEND_STATE:
            {
                // doesn't happen directly...already out of SUSPEND state by
                // the fact this code is running.  Just set rc=0 so that
                // the caller moves to what's next
                //
                rc = 0;
            }
        }
    }

    // save new state
    //
    if (rc == 0)
    {
    	logDebugMsg("lpmTest: Transitioned to power state %s", getStateStr(newState));
        currentState = newState;
    }

    return rc;
}

/*
 * assumes caller has lock on 'stateMTX'
 */
int transitionToPrepState(transition trans)
{
    if (trans == TRANS_IN)
    {
        // entering the state (moving toward SUSPEND)
        //
    	logDebugMsg("lpmTest: Transition into power state %s", getStateStr(LPM_PREP_STATE));
    	printf("[LPM-Test] (1) - %s\n", getStateStr(LPM_PREP_STATE));

        // call LPM HAL to turn off any LEDs controlled by BSP
        hal_lpm_transition(LPM_PREP_STATE, SLEEP);
    }
    else
    {
        // coming out (back to NORMAL)
        //
    	logDebugMsg("lpmTest: Transition out of power state %s", getStateStr(LPM_PREP_STATE));
        // call LPM HAL to power up any LEDs controlled by BSP
        hal_lpm_transition(LPM_PREP_STATE, RESTORE);


        // we are now back in the NORMAL state
      	printf("[LPM-Test] (0) - %s\n", getStateStr(LPM_NORMAL_STATE));
    }

    return 0;
}

/*
 * assumes caller has lock on 'stateMTX'
 */
int transitionToBeginState(transition trans)
{
    if (trans == TRANS_IN)
    {
        // entering the state (moving toward SUSPEND)
        //
    	logDebugMsg("lpmTest: Transition into power state %s", getStateStr(LPM_BEGIN_STATE));
    	printf("[LPM-Test] (2) - %s\n", getStateStr(LPM_BEGIN_STATE));

        // tell the managers (in sequence)
        //
    	//printf("lpmTest: Notify the managers of state change:\n");
    	//printf("--------------------------------------------\n");
        notifyCommMgr(LPM_BEGIN_STATE);
        notifyNetworkMgr(LPM_BEGIN_STATE);
        notifyAudioMgr(LPM_BEGIN_STATE);
        notifyZigbeeMgr(LPM_BEGIN_STATE);
    	//printf("--------------------------------------------\n");

        // now start shutting down resources
        //
        logDebugMsg("lpmTest: taking down bband...");
        system("ifconfig eth0 down");
        sleep(2);

        logDebugMsg("lpmTest: taking down resources...");

        // call HAL to begin shutting down resources
        hal_lpm_transition(LPM_BEGIN_STATE, SLEEP);

    }
    else
    {
        // coming out (back to NORMAL)
        //
    	logDebugMsg("lpmTest: Transition out of power state %s", getStateStr(LPM_BEGIN_STATE));

        // call HAL to enable resources
    	hal_lpm_transition(LPM_BEGIN_STATE, RESTORE);

      // we are now back in the PREP state
    	printf("[LPM-Test] (1) - %s\n", getStateStr(LPM_PREP_STATE));
       
        // tell the managers (in sequence)
        //
        notifyZigbeeMgr(LPM_NORMAL_STATE);
        notifyAudioMgr(LPM_NORMAL_STATE);
        notifyNetworkMgr(LPM_NORMAL_STATE);
        notifyCommMgr(LPM_NORMAL_STATE);

        // turn the screen on just to be sure that we clear
        // any problems with Droid being asleep
        //
        adjustScreenPower(SCREEN_ON);        
    }

    return 0;
}

/*
 * assumes caller has lock on 'stateMTX'
 */
int transitionToTearDownState(transition trans)
{
    if (trans == TRANS_IN)
    {
    	int waitCount = 0;
    	int screenTurnedOff = 0;
        // entering the state (moving toward SUSPEND)
        //
    	logDebugMsg("lpmTest: Transition into power state %s", getStateStr(LPM_TEAR_DOWN_STATE));
    	printf("[LPM-Test] (3) - %s\n", getStateStr(LPM_TEAR_DOWN_STATE));

        // tell the managers (in sequence)
        //
        notifyCommMgr(LPM_TEAR_DOWN_STATE);
        notifyNetworkMgr(LPM_TEAR_DOWN_STATE);
        notifyZigbeeMgr(LPM_TEAR_DOWN_STATE);

        // disable resources
        //
        logDebugMsg("lpmTest: taking down ppp...");
        system("ifconfig ppp0 down");
        sleep(3);
        system("killall pppd");
        sleep(5);

        // call HAL to shut down resources
        hal_lpm_transition(LPM_TEAR_DOWN_STATE, SLEEP);

        // turn off screen
        adjustScreenPower(SCREEN_OFF);
    }
    else
    {
        // coming out (back toward NORMAL)
        //
    	logDebugMsg("lpmTest: Transition out of power state %s", getStateStr(LPM_TEAR_DOWN_STATE));

        // enable resources
        //
        adjustScreenPower(SCREEN_ON);

        // call HAL to enable resources
        hal_lpm_transition(LPM_TEAR_DOWN_STATE, RESTORE);

      // we are now back in the BEGIN state  
    	printf("[LPM-Test] (2) - %s\n", getStateStr(LPM_BEGIN_STATE));

        // tell the managers (in sequence)
        //
        notifyZigbeeMgr(LPM_BEGIN_STATE);
        notifyNetworkMgr(LPM_BEGIN_STATE);
        notifyCommMgr(LPM_BEGIN_STATE);

    }

    return 0;
}

/*
 * assumes caller has lock on 'stateMTX'
 */
int transitionToStandbyState(transition trans)
{
    if (trans == TRANS_IN)
    {
        // entering the state (moving toward SUSPEND)
        //
    	logDebugMsg("lpmTest: Transition into power state %s", getStateStr(LPM_STANDBY_STATE));
    	printf("[LPM-Test] (4) - %s\n", getStateStr(LPM_STANDBY_STATE));

        // tell zigbee manager, and give it some time to send the
        // notification down to the UART
        //
    	notifyZigbeeMgr(LPM_STANDBY_STATE);
      sleep(2);
    }
    else
    {
        // coming out (back toward NORMAL)
        //
    	logDebugMsg("lpmTest: Transition out of power state %s", getStateStr(LPM_STANDBY_STATE));

      // we are now back in the TEAR-DOWN state  
    	printf("[LPM-Test] (3) - %s\n", getStateStr(LPM_TEAR_DOWN_STATE));
    	
        // tell zigbee we're going to 'tear down' so it can process events
        //
    	notifyZigbeeMgr(LPM_TEAR_DOWN_STATE);
    }

    return 0;
}


/*
 * assumes caller has lock on 'stateMTX'
 */
void transitionToSuspendState()
{
    // test suspending the cpu to see what happens
    //
	  logDebugMsg("lpmTest: Transitioned into power state %s", getStateStr(LPM_SUSPEND_STATE));
	  //printf("lpmTest: ------------------------------------------> Enter %s\n", getStateStr(LPM_SUSPEND_STATE));
    printf("[LPM-Test] (5) - %s\n", getStateStr(LPM_SUSPEND_STATE));
    printf("[LPM-Test] (5) - %s, ********* Suspending CPU *********\n\n\n", getStateStr(LPM_SUSPEND_STATE));

    // before going into deep sleep, get the button & screen event counts
    // NOTE: already have the stateMTX locked
    //
    getUserEventCounts(&buttonCount, &screenCount);

    // turn off screen (in case it came on temporarily)
    logDebugMsg("lpmTest: Adjust screen to OFF (may already be off from powerdown)\n");
    sleep(5);
    adjustScreenPower(SCREEN_OFF);

    // call HAL to shut down resources
    logDebugMsg("lpmTest: Suspending CPU ...\n");
    hal_lpm_transition(LPM_SUSPEND_STATE, SLEEP);

    // at this point, we should be suspended; meaning the code below will
    // not run unless the CPU wakes up.  Could be due to a zone fault or
    // just for the shit of it (it happens for an unknown reason)
    //
    logDebugMsg("lpmTest: No longer in SUSPEND state");
    printf("[LPM-Test] (4) - %s, No longer in SUSPEND state. Wakeup sequence started.\n", getStateStr(LPM_STANDBY_STATE));
    sleep(2);
}

/*
 * 0 for off, 255 for on.
 */
void adjustScreenPower(int level)
{
	if (level == SCREEN_OFF)
	{
		hal_lpm_set_screen_state(0);
		logDebugMsg("lpmTest: turned screen OFF");
	}
	else
	{
		hal_lpm_set_screen_state(1);
		logDebugMsg("lpmTest: turned screen on, level=%d", level);

		// need to make sure the system allows the screen on
		// NOTE: only do this if the screen was logically off.  Otherwise we will
		//       compete with the Security App, which could be trying to display
		//       the "disarm" screen - DE7405
		//
		if (isInDarkLPM() == 1)
		{
			// Send android the virtual keypress for the HOME button
			// This is the best way to make sure Android goes through its whole
			// wake sequence, as it's not completely tied in to the sys/power/state
			// transition. :(
			logDebugMsg("lpmTest: simulating user pressing MENU button...");
			system("/system/bin/sendevent /dev/input/event0 1 28 1");
			// We need to send the key down (1) and key up(0) events.
			system("/system/bin/sendevent /dev/input/event0 1 28 0");
		}
	}
}

/**
 * for debugging
 */
const char *getStateStr(hal_lpm_state_t state)
{
    switch(state)
    {
        case LPM_NORMAL_STATE:
            return LPM_NORMAL_STATE_STR;

        case LPM_PREP_STATE:
        	return LPM_PREP_STATE_STR;

        case LPM_BEGIN_STATE:
            return LPM_BEGIN_STATE_STR;

        case LPM_TEAR_DOWN_STATE:
            return LPM_TEAR_DOWN_STATE_STR;

        case LPM_STANDBY_STATE:
            return LPM_STANDBY_STATE_STR;

        case LPM_SUSPEND_STATE:
            return LPM_SUSPEND_STATE_STR;
    }

    return (const char *)"";
}
