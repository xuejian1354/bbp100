/****************************************************************************
*
* @file			lpmTest.c
* @brief	  	Android integration test for Low Power Mode HAL.
* @date			Jun 14, 2012
* @Author		jgleason
*
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <libgen.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <android/log.h>

#include <hal/battery.h>
#include <hal/lpmHW.h>
#include "lpmTest.h"
#include "lpmStateMachine.h"


/*
 * Register for power events, button events, screen touch events
 * Buttons must be mapped, etc.
 * In Normal Mode
 * Power is removed
 * Transition through power states
 * At any point, if power is re-applied, transition the back out.
 * Otherwise, go all the way to complete suspend.
 * If button is pressed, turn screen on.
 * If screen is touched, turn screen on.
 * After inactivity (? seconds), screen back off.
 * Power attached, walk back up to normal mode.
 *
 */

char 					*managerLogName = (char *)0;
static int				loggingInitialized = 0;
static char 			*logName;
static pthread_mutex_t	DPP_MTX = PTHREAD_MUTEX_INITIALIZER;

/* Function Prototypes */
static void powerEventCallback(int eventData, powerEvent *extraInfo);
void initAndroidLogger(char *processName);
static void *lpmTestThread(void *arg);

/*
 * 	Code begins here...
 */

int main(int argc, char *argv[])
{
    //
	printf("\n\n[LPM-Test]  Low Power Mode (LPM) Integration Test.\n");
  printf("[LPM-Test]  This test will detect when power is removed and then step through the Low Power states sequentially.\n");
  printf("[LPM-Test]  The proper sequence is: NORMAL -> BEGIN -> TEAR-DOWN -> STANDBY -> SUSPEND\n");
	printf("[LPM-Test]  The power cable and a charged battery should be attached.\n\n");

	// Android wants logging directly initialized
	//
  managerLogName = basename(argv[0]);
	initAndroidLogger(managerLogName);

	// register for android power events
        hal_battery_register(powerEventCallback);

	// start our state machine
	//
	initStateMachine();

	// start the test thread
	//
	lpmTestThread(NULL);

	// don't reach here, but put it here to make the compiler happy
	//
	return EXIT_SUCCESS;
}

/*
 *  For Android only, init the logger directly
 */
void initAndroidLogger(char *processName)
{
    pthread_mutex_lock(&DPP_MTX);
	if (loggingInitialized == 0)
	{
		if (processName != NULL)
		{
			logName = strdup(processName);
		}
		else
		{
			logName = strdup("lpmTest");
		}

		loggingInitialized++;
	}
    pthread_mutex_unlock(&DPP_MTX);
}


/*
 * callback from androidHardwareEvents when Power & Battery events occur
 */
static void powerEventCallback(int eventData, powerEvent *extraInfo)
{
    logDebugMsg("lpmTest: got hardware event notification, value=%d", eventData);
    if (eventData == POWER_EVENT_CHARGER_DETTACHED)
    {
    	if (isLowPowerSequenceRunning() > 0)
    	{
    		logDebugMsg("lpmTest: already processing a low power sequence, bailing");
    		return;
    	}

    	// tell state machine to start the low power sequence
    	// first, cancel any current sequence going on
    	//
   		cancelNormalPowerSequence();
    	startLowPowerSequence();
    }
    else if (eventData == POWER_EVENT_CHARGER_ATTACHED)
    {
    	if (isNormalPowerSequenceRunning() > 0)
    	{
    		logDebugMsg("lpmTest: already processing a normal power sequence, bailing");
    		return;
    	}

    	// tell state machine to cancel the low power sequence
    	// first, cancel any current sequence going on
    	//
   		cancelLowPowerSequence();
    	startNormalPowerSequence();
    }
}

// Stubs/Logging for Notifying the managers
void notifyCommMgr(hal_lpm_state_t state)
{
	if(state == LPM_NORMAL_STATE)
		printf("[LPM-Test] (0) - NORMAL_STATE, CommMgr notified.\n");
	else if (state == LPM_PREP_STATE)
		printf("[LPM-Test] (1) - PREP_STATE, CommMgr notified.\n");
	else if (state == LPM_BEGIN_STATE)
		printf("[LPM-Test] (2) - BEGIN_STATE, CommMgr notified.\n");
	else if (state == LPM_TEAR_DOWN_STATE)
		printf("[LPM-Test] (3) - TEAR_DOWN_STATE, CommMgr notified.\n");
	else if (state == LPM_STANDBY_STATE)
		printf("[LPM-Test] (4) - STANDBY_STATE, CommMgr notified.\n");
	else if (state == LPM_SUSPEND_STATE)
		printf("[LPM-Test] (5) - SUSPEND_STATE, CommMgr notified.\n");
	else
		printf("[LPM-Test] (X) - INVALID_STATE, CommMgr notified.\n");
	return;
}

void notifyNetworkMgr(hal_lpm_state_t state)
{
	if(state == LPM_NORMAL_STATE)
		printf("[LPM-Test] (0) - NORMAL_STATE, NetworkMgr notified.\n");
	else if (state == LPM_PREP_STATE)
		printf("[LPM-Test] (1) - PREP_STATE, NetworkMgr notified.\n");
	else if (state == LPM_BEGIN_STATE)
		printf("[LPM-Test] (2) - BEGIN_STATE, NetworkMgr notified.\n");
	else if (state == LPM_TEAR_DOWN_STATE)
		printf("[LPM-Test] (3) - TEAR_DOWN_STATE, NetworkMgr notified.\n");
	else if (state == LPM_STANDBY_STATE)
		printf("[LPM-Test] (4) - STANDBY_STATE, NetworkMgr notified.\n");
	else if (state == LPM_SUSPEND_STATE)
		printf("[LPM-Test] (5) - SUSPEND_STATE, NetworkMgr notified.\n");
	else
		printf("[LPM-Test] (X) - INVALID_STATE, NetworkMgr notified.\n");
	return;
}

void notifyAudioMgr(hal_lpm_state_t state)
{
	if(state == LPM_NORMAL_STATE)
		printf("[LPM-Test] (0) - NORMAL_STATE, AudioMgr notified.\n");
	else if (state == LPM_PREP_STATE)
		printf("[LPM-Test] (1) - PREP_STATE, AudioMgr notified.\n");
	else if (state == LPM_BEGIN_STATE)
		printf("[LPM-Test] (2) - BEGIN_STATE, AudioMgr notified.\n");
	else if (state == LPM_TEAR_DOWN_STATE)
		printf("[LPM-Test] (3) - TEAR_DOWN_STATE, AudioMgr notified.\n");
	else if (state == LPM_STANDBY_STATE)
		printf("[LPM-Test] (4) - STANDBY_STATE, AudioMgr notified.\n");
	else if (state == LPM_SUSPEND_STATE)
		printf("[LPM-Test] (5) - SUSPEND_STATE, AudioMgr notified.\n");
	else
		printf("[LPM-Test] (X) - INVALID_STATE, AudioMgr notified.\n");
	return;
}

void notifyZigbeeMgr(hal_lpm_state_t state)
{
	if(state == LPM_NORMAL_STATE)
		printf("[LPM-Test] (0) - NORMAL_STATE, ZigbeeMgr notified.\n");
	else if (state == LPM_PREP_STATE)
		printf("[LPM-Test] (1) - PREP_STATE, ZigbeeMgr notified.\n");
	else if (state == LPM_BEGIN_STATE)
		printf("[LPM-Test] (2) - BEGIN_STATE, ZigbeeMgr notified.\n");
	else if (state == LPM_TEAR_DOWN_STATE)
		printf("[LPM-Test] (3) - TEAR_DOWN_STATE, ZigbeeMgr notified.\n");
	else if (state == LPM_STANDBY_STATE)
		printf("[LPM-Test] (4) - STANDBY_STATE, ZigbeeMgr notified.\n");
	else if (state == LPM_SUSPEND_STATE)
		printf("[LPM-Test] (5) - SUSPEND_STATE, ZigbeeMgr notified.\n");
	else
		printf("[LPM-Test] (X) - INVALID_STATE, ZigbeeMgr notified.\n");
	return;
}

/**
 * issues a debug message
 */
ssize_t logDebugMsg(const char *fmt, ...)
{
    ssize_t     ret = 0;
    va_list     arglist;

	va_start(arglist, fmt);
	__android_log_vprint(ANDROID_LOG_DEBUG, logName, fmt, arglist);
	va_end(arglist);

    return(ret);
}

static void *lpmTestThread(void *arg)
{
    int   choice=100;

    while (choice != 0) {
        /* Display the menu */
        printf("\n[LPM-Test]  To begin the test, unplug the power cable.\n");
        printf("[LPM-Test]  To Exit the test, Press 0.\n\n\n");
#if 0
// Add functionality here...        
        printf("\n\n\n LPM Integration Test Menu\n\n");
        printf("   1. Get battery value\n");
        printf("   2. TBD\n");
        printf("   3. TBD\n");
        printf("   4. TBD\n");
        printf("\n   0. Exit\n");
        printf("\n   Enter selection: \n");
#endif
        /* Get the users request */
        scanf("%d", &choice);
        printf("\n\n\n");

        /* Decide what to do with it */
        switch (choice) {
            case 0:
                break;

            case 1:
                //printf ("Getting battery value...\n");
                //getBatteryValue();
                break;

            case 2:
                //printf ("TBD...\n");
                //TBD();
                break;

            case 3:
                //printf ("Enter TBD # to TBD: \n");
                //scanf("%d", &TBD);
                //TBD();
                break;

            case 4:
                //printf ("TBD: \n");
                break;

            default:
                printf ("Invalid selection\n");
                sleep(2);
                break;
        }
    }

    exit(EXIT_SUCCESS);
}
