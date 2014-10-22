#ifndef __LPMHW_IC_H__
#define __LPMHW_IC_H__
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS


typedef unsigned char bool;

/* The Low Power Mode states */ 
typedef enum {
    LPM_NORMAL_STATE = 0,
    LPM_PREP_STATE = 1,     // Prepare for low power mode by dimming the power indicator LED and
                            // turning any other LEDs off
    LPM_BEGIN_STATE = 2,    // Beginning of low power mode
    LPM_TEAR_DOWN_STATE = 3,// Working through internal steps of 'inform server' and bringing down
                            // devices
    LPM_STANDBY_STATE = 4,  // DO NOTHING. The HAL does not do anything for this state.  Icontrol
                            // notifies ZigBee to enter Low Power Mode.
    LPM_SUSPEND_STATE = 5   // Suspend the CPU, won't return unless processor is awakened.
} hal_lpm_state_t;  

// The action to take (Sleep or Restore) when entering the Low Power Mode state.  
typedef enum {
    RESTORE = 0,            // Resources should be powered back up.
    SLEEP = 1               // Resources should be powered down.
} hal_lpm_action_t; 


int hal_lpm_transition(hal_lpm_state_t state, hal_lpm_action_t action);
int hal_lpm_set_screen_state(bool level);
int hal_lpm_get_version();

/*
 * The id of this module
 */
#define LPM_HARDWARE_MODULE_ID      "lpm"

typedef struct _lpm_module_t {
    struct hw_module_t common;
} lpm_module_t;


typedef struct _lpm_control_device_t {
    struct hw_device_t common;

    int(*lpm_transition)(hal_lpm_state_t state, hal_lpm_action_t action);
    int(*lpm_set_screen_state)(bool level);
    int(*lpm_get_version)();

} lpm_control_device_t;

__END_DECLS

#endif	// __LPMHW_IC_H__
