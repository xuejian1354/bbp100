#include <fcntl.h>
#include <errno.h>

#include <hal/sysinfoHW.h>

/***************************************************************************
***************************************************************************/
static int sysinfo_exit(struct hw_device_t *device) 
{
    sysinfo_control_device_t *dev = (sysinfo_control_device_t *)device;
    if(NULL != dev)
    {
        free(dev);
    }
    return 0;
}


static int sysinfo_init(const struct hw_module_t *module, char const *name,
	struct hw_device_t **device)
{
    sysinfo_control_device_t *dev;
    dev = (sysinfo_control_device_t *)malloc(sizeof(sysinfo_control_device_t));
    if (NULL == dev)
    {
        return 0;
    }

    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = sysinfo_exit;

    *device = (struct hw_device_t *)&dev->common;

    dev->sysinfo_get_hwver = hal_sysinfo_get_hwver;
    dev->sysinfo_get_serialnum = hal_sysinfo_get_serialnum;
    dev->sysinfo_get_macaddr = hal_sysinfo_get_macaddr;
    dev->sysinfo_get_capability = hal_sysinfo_get_capability;
    dev->sysinfo_set_capability = hal_sysinfo_set_capability;
    dev->sys_info_get_version = hal_sys_info_get_version;

    return 0;
}


static struct hw_module_methods_t sysinfo_module_methods = {
    open: sysinfo_init
};

const sysinfo_module_t HAL_MODULE_INFO_SYS = {
    common:
    {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: SYSINFO_HARDWARE_MODULE_ID,
        name: "sysinfo Stub",
        author: "bbp",
        methods: &sysinfo_module_methods,
    }
};

