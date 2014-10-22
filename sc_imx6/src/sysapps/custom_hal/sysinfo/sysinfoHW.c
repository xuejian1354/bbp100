#include <hal/sysinfoHW.h>

/***************************************************************
 *
 * @fn	     hal_sysinfo_get_hwver
 *
 * @brief    Returns the hardware version (max. 10 characters) 
 *	     which is incremented each time the hardware changes 
 *	     in a way that affects the software
 *
 * @param    *hwVersion: pointer to get hardware version
 *
 * @return   none
 */
int hal_sysinfo_get_hwver(uint8_t *hwVersion)
{
    return 0;
}


/***************************************************************
 *
 * @fn	     hal_sysinfo_get_serialnum
 *
 * @brief    Returns the device serial number as a NULL-terminated 
 *	     string. 
 *	     The length of the buffer for the serial number is 
 *	     passed in - do NOT exceed this length.
 *
 * @param    *serialNumber: pointer to get serialNumber
 *	     length: get serialNumber length
 *
 * @return   none
 */
int hal_sysinfo_get_serialnum(char *serialNumber, uint8_t length)
{
    return 0;
}
 


/****************************************************************
 * 
 * @fn	     hal_sysinfo_get_macaddr
 *
 * @brief    Returns the device LAN MAC address into an unsigned 
 *	     byte array.
 *
 * @param    macAddress[6]: array to get mac address
 *
 * @return   none
 */
int hal_sysinfo_get_macaddr(uint8_t macAddress[6])
{
    return 0;
}


/****************************************************************
 * @fn	     hal_sysinfo_get_capability
 *
 * @brief    Returns capability value (1 for true or 0 for false). 
 *	     True indicates the capability is supported and enabled; 
 *	     False indicates that it is not supported.
 *
 * @param    capability
 *
 * @return   none
 */
int hal_sysinfo_get_capability (hal_sysinfo_capability_t capability)
{
    return 0;
}

 
/******************************************************************
 *
 * @fn	     hal_sysinfo_set_capability
 *
 * @brief    Enables the capability in the device.
 *	     If the device uses eFuses, this call may not be reversible. 
 *	     Understand the function prior to calling it.
 *
 * @param    capability
 *
 * @return   none
 */
int hal_sysinfo_set_capability (hal_sysinfo_capability_t capability)
{
    return 0;
}


/******************************************************************
 *
 * @fn	     hal_sys_info_get_version
 *
 * @brief    Returns the API Version of the SysInfo HAL. See Revision 
 *	     History for SysInfo HAL. 
 *
 * @param    none
 *
 * @return   version
 */
int hal_sys_info_get_version()
{
    return 0;
}



