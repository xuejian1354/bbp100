#include <hal/btn.h>


/************************************************************
 *
 * @fn	     hal_btn_register
 *
 * @brief    Register a callback function for receiving button
 *	     events.The callback accepts one argument: the 
 *	     btn_name_t enum for the button id
 *
 * @param    press_function, release_function
 *
 * @return   none
 */
void hal_btn_register(void(*press_function)(btn_name_t), void(*release_function)(btn_name_t))
{
    return;
}
 

/************************************************************
 *
 * @fn	     hal_btn_get_state
 *
 * @brief    Get the current state of any button. Allows for 
 *	     checking if the button is pressed at startup before 
 *	     events have occurred. 
 *
 * @param    btn_name
 *
 * @return   0 = released 
 *	     1 = pressed
 *	     -1 = button not supported (in addition, errno should 
 *	     be set to ENOTSUP)
 */
int hal_btn_get_state(btn_name_t btn_name)
{
    return 0;
}


/***************************************************************** 
 * @fn	     hal_btn_get_version
 *
 * @brief    Returns the API Version of the HAL. See Revision History 
 *	     for Button HAL.
 *
 * @param    none
 * 
 * @return   version
 */
int hal_btn_get_version()
{
    return 0;
}
 

