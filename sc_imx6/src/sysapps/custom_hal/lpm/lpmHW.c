#include <hal/lpmHW.h>

/****************************************************************
 *
 * @fn	     hal_lpm_transition
 *
 * @brief    Transition between the states. This is bi-directional. 
 *	     States can be entered in the Suspend flow (going into 
 *	     LPM) and in the Restore mode (coming out of LPM). 
 *	     The direction is indicated by the action (Sleep or 
 *	     Restore).
 *
 * @param    state,action
 *
 * @return   none
 */
int hal_lpm_transition(hal_lpm_state_t state, hal_lpm_action_t action)
{
    return 0;
}
 

/***************************************************************
 *
 * @fn	     hal_lpm_set_screen_state
 *
 * @brief    Turns the display (LCD + backlight) On and Off. 
 *
 * @param    True: turn display on 
 *	     False: turn display off
 *
 * @return   none
 */
int hal_lpm_set_screen_state(bool level)
{
    return 0;
}


/****************************************************************
 *
 * @fn	     hal_lpm_get_version
 *
 * @brief    Returns the API Version of the LPM HAL. See Revision 
 *	     History for LPM HAL.
 *
 * @param    none
 *
 * @return   version
 */
int hal_lpm_get_version()
{
    return 0;
}

