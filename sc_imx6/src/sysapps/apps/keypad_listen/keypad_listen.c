#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define KEYPAD_DEV_FILE "/dev/sc_drv_keypad"

typedef enum
{
    KEYPAD_NUM1 = 1,
    KEYPAD_NUM2,
    KEYPAD_NUM3,
    KEYPAD_NUM4,
    KEYPAD_NUM5,
    KEYPAD_NUM6,
    KEYPAD_NUM7,
    KEYPAD_NUM8,
    KEYPAD_NUM9,
    KEYPAD_NUM10,
    KEYPAD_NUM11,
    KEYPAD_NUM12,
    KEYPAD_NUM13,
    KEYPAD_NUM14,
    KEYPAD_NUM15,
    KEYPAD_NUM16,
} KeypadNum_t;

static int keypad_ls = 0;
static pthread_mutex_t mutex;

void num1_handler();
void num2_handler();
void num3_handler();
void num4_handler();
void num5_handler();
void num6_handler();
void num7_handler();
void num8_handler();
void num9_handler();
void num10_handler();
void num11_handler();
void num12_handler();
void num13_handler();
void num14_handler();
void num15_handler();
void num16_handler();


void *keypad_handler(void *arg)
{
    int *res = (int *)arg;
    while (!keypad_ls)
    {
        pthread_mutex_lock(&mutex);
        
        switch(*res)
        {
        case KEYPAD_NUM1: num1_handler(); break;
        case KEYPAD_NUM2: num2_handler(); break;
        case KEYPAD_NUM3: num3_handler(); break;
        case KEYPAD_NUM4: num4_handler(); break;
        case KEYPAD_NUM5: num5_handler(); break;
        case KEYPAD_NUM6: num6_handler(); break;
        case KEYPAD_NUM7: num7_handler(); break;
        case KEYPAD_NUM8: num8_handler(); break;
        case KEYPAD_NUM9: num9_handler(); break;
        case KEYPAD_NUM10: num10_handler(); break;
        case KEYPAD_NUM11: num11_handler(); break;
        case KEYPAD_NUM12: num12_handler(); break;
        case KEYPAD_NUM13: num13_handler(); break;
        case KEYPAD_NUM14: num14_handler(); break;
        case KEYPAD_NUM15: num15_handler(); break;
        case KEYPAD_NUM16: num16_handler(); break;
        default: break;
        }
    }

    return NULL;
}


int main(int argc, char **argv)
{
    int keypad_fd = 0;
    int res;

    pthread_t thread_kp = 0;

    keypad_fd = open(KEYPAD_DEV_FILE, O_RDWR);
    if(keypad_fd < 0)
    {
        fprintf(stderr, "open keypad dev failure! %d\n", keypad_fd);
        return -1;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    pthread_create(&thread_kp, NULL, (void *)keypad_handler, &res);

    do {
        res = read(keypad_fd, NULL, 0);
        
        if(res>=KEYPAD_NUM1 && res<=KEYPAD_NUM16)
        {
            pthread_mutex_unlock(&mutex);
        }
    
    } while (!keypad_ls);

    close(keypad_fd);
    return 0;
}


void num1_handler()
{
    printf("function: %s\n", __func__);
}

void num2_handler()
{
    printf("function: %s\n", __func__);
}

void num3_handler()
{
    printf("function: %s\n", __func__);
}

void num4_handler()
{
    printf("function: %s\n", __func__);
}

void num5_handler()
{
    printf("function: %s\n", __func__);
}

void num6_handler()
{
    printf("function: %s\n", __func__);
}

void num7_handler()
{
    printf("function: %s\n", __func__);
}

void num8_handler()
{
    printf("function: %s\n", __func__);
}

void num9_handler()
{
    printf("function: %s\n", __func__);
}

void num10_handler()
{
    printf("function: %s\n", __func__);
}

void num11_handler()
{
    printf("function: %s\n", __func__);
}

void num12_handler()
{
    printf("function: %s\n", __func__);
}

void num13_handler()
{
    printf("function: %s\n", __func__);
}

void num14_handler()
{
    printf("function: %s\n", __func__);
}

void num15_handler()
{
    printf("function: %s\n", __func__);
}

void num16_handler()
{
    printf("function: %s\n", __func__);
}

void stop_keypad_ls(void)
{
    keypad_ls = 1;
}
