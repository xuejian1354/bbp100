#include <fcntl.h>
#include <errno.h>

#include <hal/ledHW.h>

/***************************************************************************
***************************************************************************/
static int led_exit(struct hw_device_t *device) 
{
    led_control_device_t *dev = (led_control_device_t *)device;
    if(NULL != dev)
    {
        free(dev);
    }
    return 0;
}


static int led_init(const struct hw_module_t *module, char const *name,
	struct hw_device_t **device)
{
    led_control_device_t *dev;
    dev = (led_control_device_t *)malloc(sizeof(led_control_device_t));
    if (NULL == dev)
    {
        return 0;
    }

    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = led_exit;

    *device = (struct hw_device_t *)&dev->common;

    dev->led_set_color = hal_led_set_color;
    dev->led_get_color = hal_led_get_color;
    dev->led_set_blink = hal_led_set_blink;
    dev->led_get_supported_list = hal_led_get_supported_list;
    dev->led_set_brightness = hal_led_set_brightness;
    dev->led_get_brightness = hal_led_get_brightness;
    dev->led_get_version = hal_led_get_version;

    return 0;
}


static struct hw_module_methods_t led_module_methods = {
    open: led_init
};

const led_module_t HAL_MODULE_INFO_SYS = {
    common:
    {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: LED_HARDWARE_MODULE_ID,
        name: "led Stub",
        author: "bbp",
        methods: &led_module_methods,
    }
};

