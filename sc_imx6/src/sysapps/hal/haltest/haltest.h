#ifndef __HALTEST_SC_H__
#define __HALTEST_SC_H__
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS

int hal_test_print();

/*
 * The id of this module
 */
#define HALTEST_HARDWARE_MODULE_ID      "haltest"

#define HALTEST_IOCTL_PRINT	1

typedef struct _haltest_module_t {
    struct hw_module_t common;
} haltest_module_t;


typedef struct _haltest_control_device_t {
    struct hw_device_t common;

    int (*test_print)();

} haltest_control_device_t;

__END_DECLS

#endif	// __HALTEST_SC_H__
