#include <fcntl.h>
#include <errno.h>

#include <hal/battery.h>

/***************************************************************************
***************************************************************************/
static int battery_exit(struct hw_device_t *device) 
{
    battery_control_device_t *dev = (battery_control_device_t *)device;
    if(NULL != dev)
    {
        free(dev);
    }
    return 0;
}


static int battery_init(const struct hw_module_t *module, char const *name,
	struct hw_device_t **device)
{
    battery_control_device_t *dev;
    dev = (battery_control_device_t *)malloc(sizeof(battery_control_device_t));
    if (NULL == dev)
    {
        return 0;
    }

    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = battery_exit;

    *device = (struct hw_device_t *)&dev->common;

    dev->battery_is_power_charger_attached = hal_battery_is_power_charger_attached;
    dev->battery_is_battery_attached = hal_battery_is_battery_attached;
    dev->battery_is_battery_bad = hal_battery_is_battery_bad;
    dev->battery_is_battery_over_temp = hal_battery_is_battery_over_temp;
    dev->battery_register = hal_battery_register;
    dev->battery_get_version = hal_battery_get_version;


    return 0;
}


static struct hw_module_methods_t battery_module_methods = {
    open: battery_init
};

const battery_module_t HAL_MODULE_INFO_SYS = {
    common:
    {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: BATTERY_HARDWARE_MODULE_ID,
        name: "battery Stub",
        author: "bbp",
        methods: &battery_module_methods,
    }
};

