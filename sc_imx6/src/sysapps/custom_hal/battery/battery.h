#ifndef __BATTERY_IC_H__
#define __BATTERY_IC_H__
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS


#define POWER_EVENT 17	//Event Code value used by Icontrol trouble sub-system to identify power events
 
#define POWER_EVENT_CHARGER_ATTACHED 0		//Power cable removed 
#define POWER_EVENT_CHARGER_DETTACHED 1		//Power cable attached 
#define POWER_EVENT_LOW_BATT 2		//Battery is low 
#define POWER_EVENT_LOW_BATT_RES 3	//Battery no longer low 
#define POWER_EVENT_BATT_REM 4		//Battery removed 
#define POWER_EVENT_BATT_REM_RES 5	//Battery replaced 
#define POWER_EVENT_BATT_BAD 6		//Battery is bad 
#define POWER_EVENT_BATT_BAD_RES 7	//Battery is no longer bad 
#define POWER_EVENT_BATT_TEMP_OVER 8	//Battery temp is over allowed threshold 
#define POWER_EVENT_BATT_TEMP_NORMAL 9		//Battery temp is within allowed threshold 

/*
 * The id of this module
 */
#define BATTERY_HARDWARE_MODULE_ID	"battery"

typedef struct _battery_module_t {
    struct hw_module_t common;
} battery_module_t;


typedef struct _powerEvent {
    int cableConnected;		///< 0 = No, 1 = Yes
    int batteryConnected; 	///< 0 = No, 1 = Yes
    int batteryChargeLevel;	///< 0 - 100
    int batteryCharging;	///< 0 = No, 1 = Yes
    int batteryBad;		///< 0 = No, 1 = Yes
} powerEvent;

typedef void (*powerEventCallbackType)(int eventData, powerEvent *extraInfo);


typedef struct _battery_control_device_t {
    struct hw_device_t common;
    
    int(*battery_is_power_charger_attached)();
    int(*battery_is_battery_attached)();
    int(*battery_is_battery_bad)();
    int(*battery_is_battery_over_temp)();
    int(*battery_register)(powerEventCallbackType funct);
    int(*battery_get_version)();
} battery_control_device_t;


int hal_battery_is_power_charger_attached();
int hal_battery_is_battery_attached();
int hal_battery_is_battery_bad();
int hal_battery_is_battery_over_temp();
int hal_battery_register(powerEventCallbackType funct);
int hal_battery_get_version();

__END_DECLS

#endif	//__BATTERY_IC_H__
