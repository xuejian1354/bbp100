#ifndef __LEDHW_IC_H__
#define __LEDHW_IC_H__
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS


#define RED_LED	 0xFF0000
#define GREEN_LED  0x00FF00
#define BLUE_LED  0x0000FF
#define PURPLE_LED  0x800080
#define YELLOW_LED  0xFFFF00
#define ORANGE_LED  0xFFA500
#define PINK_LED  0xFF00FF

typedef enum {
    STATUS_LED,
    POWER_LED,
    ETH_LED,
    WIFI_LED,
    DEVICES_LED,
    CELL_LED,
    ERROR_LED,
    LED_COUNT
} led_name_t;  

typedef enum {
    BLINK_OFF,
    BLINK_SLOW, // Pulse
    BLINK_FAST
} led_blink_rate_t; 


int hal_led_set_color(led_name_t index, int rgb);
int hal_led_get_color(led_name_t index, int *rgb);
int hal_led_set_blink(led_name_t index, led_blink_rate_t rate);
int hal_led_get_supported_list(led_name_t **supported_led_list);
int hal_led_set_brightness(led_name_t index, int brightness);
int hal_led_get_brightness(led_name_t index, int *brightness);
int hal_led_get_version();

/*
 * The id of this module
 */
#define LED_HARDWARE_MODULE_ID      "Led"

typedef struct _led_module_t {
    struct hw_module_t common;
} led_module_t;


typedef struct _led_control_device_t {
    struct hw_device_t common;

    int (*led_set_color)(led_name_t index, int rgb);
    int (*led_get_color)(led_name_t index, int *rgb);
    int (*led_set_blink)(led_name_t index, led_blink_rate_t rate);
    int (*led_get_supported_list)(led_name_t **supported_led_list);
    int (*led_set_brightness)(led_name_t index, int brightness);
    int (*led_get_brightness)(led_name_t index, int *brightness);
    int (*led_get_version)();

} led_control_device_t;

__END_DECLS

#endif	// __LEDHW_IC_H__
