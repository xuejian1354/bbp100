/****************************************************************************
*
* @file			lpmTest.h
* @brief	  	Header file for the Low Power Manager (LPM) HAL Android test.
* @date			Jun 14, 2012
* @Author		jgleason
*
****************************************************************************/
#ifndef LPM_TEST_H_
#define LPM_TEST_H_

#include "lpmStateMachine.h"

/*
 * Stubs for notifying the managers
 */
extern void notifyCommMgr(hal_lpm_state_t state);
extern void notifyNetworkMgr(hal_lpm_state_t state);
extern void notifyAudioMgr(hal_lpm_state_t state);
extern void notifyZigbeeMgr(hal_lpm_state_t state);

extern ssize_t logDebugMsg(const char *fmt, ...);

#endif /* LPM_TEST_H_ */
