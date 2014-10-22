#include <fcntl.h>
#include <errno.h>

#include <hal/btn.h>

/***************************************************************************
***************************************************************************/
static int btn_exit(struct hw_device_t *device) 
{
    btn_control_device_t *dev = (btn_control_device_t *)device;
    if(NULL != dev)
    {
        free(dev);
    }
    return 0;
}


static int btn_init(const struct hw_module_t *module, char const *name,
	struct hw_device_t **device)
{
    btn_control_device_t *dev;
    dev = (btn_control_device_t *)malloc(sizeof(btn_control_device_t));
    if (NULL == dev)
    {
        return 0;
    }

    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = btn_exit;

    *device = (struct hw_device_t *)&dev->common;

    dev->btn_register = hal_btn_register;
    dev->btn_get_state = hal_btn_get_state;
    dev->btn_get_version = hal_btn_get_version;

    return 0;
}


static struct hw_module_methods_t btn_module_methods = {
    open: btn_init
};

const btn_module_t HAL_MODULE_INFO_SYS = {
    common:
    {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: BUTTON_HARDWARE_MODULE_ID,
        name: "button Stub",
        author: "bbp",
        methods: &btn_module_methods,
    }
};

