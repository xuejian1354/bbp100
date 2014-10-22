#ifndef __BTN_IC_H__
#define __BTN_IC_H__
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS


#define BUTTON_PRESSED 1 	//The button has been pressed. 
#define BUTTON_RELEASED 0 	//The button has been released. 
#define BUTTON_NOT_SUPPORTED -1 	//The button is not supported.

typedef enum{
    RESET_BTN =0,
    TAMPER_BTN,
    // keypad buttons
    NUM_ZERO_BTN,
    NUM_ONE_BTN,
    NUM_TWO_BTN,
    NUM_THREE_BTN,
    NUM_FOUR_BTN,
    NUM_FIVE_BTN,
    NUM_SIX_BTN,
    NUM_SEVEN_BTN,
    NUM_EIGHT_BTN,
    NUM_NINE_BTN,
    ASTERISK_BTN,
    POUND_BTN,
    A_BTN,
    B_BTN,
    // security buttons
    PANIC_POLICE_BTN,
    PANIC_MEDICAL_BTN,
    PANIC_FIRE_BTN,
    ARM_AWAY_BTN,
    ARM_STAY_BTN,
    ARM_NIGHT_BTN,
    // menu buttons
    MENU_UP_BTN,
    MENU_DOWN_BTN,
    MENU_LEFT_BTN,
    MENU_RIGHT_BTN,
    MENU_SELECT_BTN,
    BTN_COUNT
} btn_name_t;


void hal_btn_register(void(*press_function)(btn_name_t), void(*release_function)(btn_name_t));
int hal_btn_get_state(btn_name_t btn_name);
int hal_btn_get_version();

/*
 * The id of this module
 */
#define BUTTON_HARDWARE_MODULE_ID      "button"

typedef struct _btn_module_t {
    struct hw_module_t common;
} btn_module_t;


typedef struct _btn_control_device_t {
    struct hw_device_t common;

    void(*btn_register)(void(*press_function)(btn_name_t), void(*release_function)(btn_name_t));
    int(*btn_get_state)(btn_name_t btn_name);
    int(*btn_get_version)();

} btn_control_device_t;

__END_DECLS

#endif	// __BTN_IC_H__
