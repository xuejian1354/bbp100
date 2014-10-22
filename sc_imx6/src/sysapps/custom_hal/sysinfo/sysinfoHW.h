#ifndef __SYSINFOHW_IC_H__
#define __SYSINFOHW_IC_H__
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS


typedef enum {
    SYSINFO_DEVICE_MEM_LOCKDOWN = 0,    // the system is configured to only boot from internal
                                        // memory (NAND, NOR, etc.)
} hal_sysinfo_capability_t; 

int hal_sysinfo_get_hwver(uint8_t *hwVersion);
int hal_sysinfo_get_serialnum(char *serialNumber, uint8_t length);
int hal_sysinfo_get_macaddr(uint8_t macAddress[6]);
int hal_sysinfo_get_capability(hal_sysinfo_capability_t capability);
int hal_sysinfo_set_capability(hal_sysinfo_capability_t capability);
int hal_sys_info_get_version();

/*
 * The id of this module
 */
#define SYSINFO_HARDWARE_MODULE_ID      "SYSINFO"

typedef struct _sysinfo_module_t {
    struct hw_module_t common;

} sysinfo_module_t;


typedef struct _sysinfo_control_device_t {
    struct hw_device_t common;

    int(*sysinfo_get_hwver)(uint8_t *hwVersion);
    int(*sysinfo_get_serialnum)(char *serialNumber, uint8_t length);
    int(*sysinfo_get_macaddr)(uint8_t macAddress[6]);
    int(*sysinfo_get_capability)(hal_sysinfo_capability_t capability);
    int(*sysinfo_set_capability)(hal_sysinfo_capability_t capability);
    int(*sys_info_get_version)();

} sysinfo_control_device_t;

__END_DECLS

#endif	// __SYSINFOHW_IC_H__
