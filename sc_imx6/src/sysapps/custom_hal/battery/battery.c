#include <hal/battery.h>


/****************************************************************
 *
 * @fn	     hal_battery_is_power_charger_attached
 *
 * @brief    Used to instantly see the last known state of the 
 *	     power charger. Returns the last known value from the 
 *           hardware event.
 *
 * @param    none
 *
 * @return   1 on YES, 0 on NO, -1 if UNKNOWN
 */
int hal_battery_is_power_charger_attached()
{
    return 0;
}


/****************************************************************
 *
 * @fn	     hal_battery_is_battery_attached
 *
 * @brief    Used to instantly see the last known state of the 
 *	     battery. Returns the last known value from the hardware
 *	     event.
 *
 * @param    none
 *
 * @return   1 on YES, 0 on NO, -1 if UNKNOWN
 */
int hal_battery_is_battery_attached()
{
    return 0;
}


/****************************************************************
 *
 * @fn	     hal_battery_is_battery_bad
 *
 * @brief    Used to instantly see if the battery is detected bad. 
 *	     Returns the last known value from the hardware event 
 *
 * @param    none
 *
 * @return   1 on YES, 0 on NO, and -1 on UNKNOWN. 
 */
int hal_battery_is_battery_bad()
{
    return 0;
}


/****************************************************************
 *
 * @fn	     hal_battery_is_battery_over_temp
 *
 * @brief    Determine if battery temperature is above the allowed 
 *	     threshold. The threshold may be hardware-specific. 
 *	     But this event informs upper application in case it 
 *	     needs to notify user. If temp cannot be measured, 
 *	     returns -1. 
 *
 * @param    none
 *
 * @return   1 on YES, 0 on NO, and -1 on UNKNOWN. 
 */
int hal_battery_is_battery_over_temp()
{
    return 0;
}


/****************************************************************
 *
 * @fn	     hal_battery_register
 *
 * @brief    Register a function of type powerEventCallbackType to 
 *	     receive events whenever this HAL detects a battery event. 
 *
 * @param    powerEventCallbackType funct
 *
 * @return   none
 */
int hal_battery_register(powerEventCallbackType funct)
{
    return 0;
}


/****************************************************************
 *
 * @fn	     hal_battery_get_version
 *
 * @brief    Returns the API Version of the battery HAL. See Revision 
 *	     History for Battery HAL. 
 *
 * @param    none
 *
 * @return   version
 */
int hal_battery_get_version()
{
    return 0;
}
