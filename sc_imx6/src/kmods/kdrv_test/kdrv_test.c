#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#define MODULE_NAME "kdrv_test"

static __devinit int kdrv_probe(struct platform_device *pdev)
{
    printk("kdrv_test module init ==>\n");
    return 0;
}

static int __devexit kdrv_remove(struct platform_device *pdev)
{
    printk("kdrv_test module remove <==\n");
    return 0;
}


static struct platform_driver ktest_driver = {
        .driver = {
                .name   = MODULE_NAME,
                .owner  = THIS_MODULE,
        },
        .probe          = kdrv_probe,
        .remove         = __devexit_p(kdrv_remove),
};

static int kdrv_init()
{
    return platform_driver_register(&ktest_driver);
}

static void kdrv_exit()
{
     platform_driver_unregister(&ktest_driver);
}


module_init(kdrv_init);
module_exit(kdrv_exit);
