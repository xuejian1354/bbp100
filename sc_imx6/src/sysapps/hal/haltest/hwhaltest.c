#include <fcntl.h>
#include <errno.h>

#include <hal/haltest.h>

/***************************************************************************
***************************************************************************/
static int haltest_exit(struct hw_device_t *device) 
{
    haltest_control_device_t *dev = (haltest_control_device_t *)device;
    if(NULL != dev)
    {
        free(dev);
    }
    return 0;
}


static int haltest_init(const struct hw_module_t *module, char const *name,
	struct hw_device_t **device)
{
    haltest_control_device_t *dev;
    dev = (haltest_control_device_t *)malloc(sizeof(haltest_control_device_t));
    if (NULL == dev)
    {
        return 0;
    }

    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = haltest_exit;

    *device = (struct hw_device_t *)&dev->common;

    dev->test_print = hal_test_print;

    return 0;
}


static struct hw_module_methods_t haltest_module_methods = {
    open: haltest_init
};

const haltest_module_t HAL_MODULE_INFO_SYS = {
    common:
    {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: HALTEST_HARDWARE_MODULE_ID,
        name: "haltest Stub",
        author: "bbp",
        methods: &haltest_module_methods,
    }
};

