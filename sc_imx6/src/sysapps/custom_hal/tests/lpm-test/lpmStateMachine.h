/*
 * lpmStateMachine.h
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
 *       - wait for any alarm/trouble messages to be acknowledged by the server (or timeout)
 *   - notify NetworkMgr
 *       - stop gprs
 *   - notify ZigbeeMgr
 *   - run equivalent to suspend_gprs
 *
 * LPM_STANDBY_STATE
 *   - notify ZigbeeMgr
 *   - check to see if an alarm is in progress
 *
 * LPM_SUSPEND_STATE
 *   - lowPowerMgr runs the 'suspend cpu' logic
 *
 */

#ifndef LPM_STATEMACHINE_H_
#define LPM_STATEMACHINE_H_

/*
 * min/max screen values
 */
#define SCREEN_OFF			0
#define SCREEN_ON			255

/*
 * init the state machine
 */
void initStateMachine();

/*
 * returns 1 if we're in low power mode.  0 if not
 */
int isInLPM();

/*
 * similar to isInLPM(), but specific to screen state.
 * returns 1 if we're in low power mode and the screen should
 * be off.  0 if not
 */
int isInDarkLPM();

/*
 * return the current state of the machine
 * this can take a while if the state is currently
 * being transitioned.
 */
hal_lpm_state_t getCurrentState();

/**
 * for debugging
 */
const char *getStateStr(hal_lpm_state_t state);

/*
 * Starts the sequence of state changes to enter low power mode.
 * Called when we receive the "AC unplugged" event.
 */
int startLowPowerSequence();

/* stops the current sequence */
void cancelLowPowerSequence();

/* returns 1 if the sequence is running */
int isLowPowerSequenceRunning();

/*
 * Starts the sequence of state changes to leave low power mode.
 * Called when we receive the "AC restored" event.
 */
int startNormalPowerSequence();

/* stops the current sequence */
void cancelNormalPowerSequence();

/* returns 1 if the sequence is running */
int isNormalPowerSequenceRunning();

/* adjust the screen power */
void adjustScreenPower(int level);

#endif /* LPM_STATEMACHINE_H_ */
