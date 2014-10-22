#include <hal/ledHW.h>

/***************************************************************
 *
 * @fn	     hal_led_set_color
 *
 * @brief    Sets the RGB color value of a particular LED. 
 *
 * @param    index: Index of the LED to set 
 *	     rgb: Color to display
 *
 * @return   none
 */
int hal_led_set_color(led_name_t index, int rgb)
{
    return 0;
}


/**************************************************************
 * @fn	     hal_led_get_color
 *
 * @brief    Gets the color value of particular LED. 
 *
 * @param    index: index of the LED to set
 *	     *rgb: pointer to get color
 *
 * @return    none
 */
int hal_led_get_color(led_name_t index, int *rgb)
{
    return 0;
}


/**************************************************************
 * @fn	     hal_led_set_blink
 *
 * @brief    Sets the blinking rate of particular LED (HARDWARE 
 *	     Implementation).  
 *	     Caller must stop the blinking of one LED color before 
 *	     blinking other color. No support for on-the-fly color 
 *	     changes while LED is set to blink. An LED can be set 
 *	     to blink at any time without regard for the blink 
 *	     state of other LEDs.
 *
 * @param    index: index of LED to set
 *	     rate: blinking rate of LED
 *
 * @return   none
 */
int hal_led_set_blink(led_name_t index, led_blink_rate_t rate)
{
    return 0;
}


/**************************************************************
 * @fn	     hal_led_get_supported_list
 *
 * @brief    Gets a list of LEDs that are supported by the 
 * 	     hardware. This list must not be dynamically generated 
 *	     or allocated. It must be defined in static storage. 
 *	     This list will NOT be freed.
 *
 * @param    **supported_led_list
 *
 * @return   none
 */
int hal_led_get_supported_list(led_name_t **supported_led_list)
{
    return 0;
}
 

/**************************************************************
 *
 * @fn	     hal_led_set_brightness
 *
 * @brief    Sets the brightness level for an LED. 
 *	     Brightness is passed in as a percent from 0 (OFF) 
 *	     to 100 (FULL ON). For low power mode status LED 
 *	     dimming, 50% is used.,
 *
 * @param    index: index of LED to set
 *	     brightness: brightness level
 *
 * @return   none
 */
int hal_led_set_brightness(led_name_t index, int brightness)
{
    return 0;
}

/**************************************************************
 *
 * @fn	     hal_led_get_brightness
 *
 * @brief    Gets the brightness level for an LED. 
 *
 * @param    index: index of LED to set
 *	     *brightness: pointer to get brightness level
 *
 * @return   none
 */
int hal_led_get_brightness(led_name_t index, int *brightness)
{
    return 0;
}


/**************************************************************
 *
 * @fn	     hal_led_get_version 
 *
 * @brief    Returns the API Version of the LED HAL. See Revision 
 *	     History for LED HAL. 
 *
 * @param    none
 *
 * @return   version
 */
int hal_led_get_version()
{
    return 0;
}

