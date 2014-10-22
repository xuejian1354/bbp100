#define LOG_TAG	"JNITest"

#include "jni.h"
#include "JNIHelp.h"
#include <utils/misc.h>
#include <utils/Log.h>
#include <hal/haltest.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static haltest_control_device_t *sHalTestDevice = NULL;

static inline int haltest_control_open(struct hw_module_t *module,
	haltest_control_device_t **device)
{
    return module->methods->open(module, HALTEST_HARDWARE_MODULE_ID, (struct hw_device_t **)device);
}


static jboolean Java_com_bbp_server_HalTestService_HalTestPrint(JNIEnv *env, jclass clazz)
{
    haltest_module_t *module;

    int err = hw_get_module(HALTEST_HARDWARE_MODULE_ID, (hw_module_t const **)&module);
    if(0 == err)
    {
        if(haltest_control_open(&(module->common), &sHalTestDevice) == 0)
        {
            if(sHalTestDevice)
            {
                sHalTestDevice->test_print();
                sHalTestDevice->common.close(&sHalTestDevice->common);
                return true;
            }
        }
    }
    sHalTestDevice = NULL;
    return false;
}

static const JNINativeMethod method_table[] = {
    {"haltest_print", "()Z", (void *)Java_com_bbp_server_HalTestService_HalTestPrint},
};

int register_com_bbp_server_HaltestService(JNIEnv *env)
{
    static const char* const kClassName = "com/bbp/server/HalTestService";
    jclass clazz;

    clazz  = env->FindClass(kClassName);
    if (NULL == clazz)
    {
        return -1;
    }

    if(env->RegisterNatives(clazz, method_table,
        sizeof(method_table)/sizeof(method_table[0])) != JNI_OK)
    {
        return -1;
    }
    return 0;
}


jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        return result;
    }

    if (register_com_bbp_server_HaltestService(env) != 0)
    {
        return result;
    }
    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif
