#include <fcntl.h>
#include <errno.h>

#include <hal/lpmHW.h>

/***************************************************************************
***************************************************************************/
static int lpm_exit(struct hw_device_t *device) 
{
    lpm_control_device_t *dev = (lpm_control_device_t *)device;
    if(NULL != dev)
    {
        free(dev);
    }
    return 0;
}


static int lpm_init(const struct hw_module_t *module, char const *name,
	struct hw_device_t **device)
{
    lpm_control_device_t *dev;
    dev = (lpm_control_device_t *)malloc(sizeof(lpm_control_device_t));
    if (NULL == dev)
    {
        return 0;
    }

    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = lpm_exit;

    *device = (struct hw_device_t *)&dev->common;

    dev->lpm_transition = hal_lpm_transition;
    dev->lpm_set_screen_state = hal_lpm_set_screen_state;
    dev->lpm_get_version = hal_lpm_get_version;

    return 0;
}


static struct hw_module_methods_t lpm_module_methods = {
    open: lpm_init
};

const lpm_module_t HAL_MODULE_INFO_SYS = {
    common:
    {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: LPM_HARDWARE_MODULE_ID,
        name: "lpm Stub",
        author: "bbp",
        methods: &lpm_module_methods,
    }
};

