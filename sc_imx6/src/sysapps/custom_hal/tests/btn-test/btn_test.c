#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <hal/btn.h>


// Private Functions
static void RunTest();
void ButtonPressedEvent(int btn_id);
void ButtonReleasedEvent(int btn_id);
/**
 * @brief: This application tests the btn hal implementation
 *         for a specific vendor.
 * @param argc : Number of arguments provided
 * @param argv : a list of the argument strings
 * @return 0 for success
 */
int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "ah")) != -1)
    {
        switch(opt)
        {
            case 'h':
                printf("Usage: %s [flags]\n", argv[0]);
                printf("  -h : display this usage\n");
                return 0;
            default:
                // Getopt is nice enough to yell at user, just need to exit
                return 0;
        }
        
    }

    RunTest();
    return 0;
}

/**
 * @brief Print the list of btns supported by the vendor
 *
 * @param none
 * @return none
 */

static void RunTest()
{
    int index;
    int state;
   
    printf("===Starting Btn Test===\n");
    hal_btn_register(ButtonPressedEvent, ButtonReleasedEvent);

    state = hal_btn_get_state(TAMPER_BTN);
    printf("Tamper Button status is: %s\n", state ? "Pressed" : "Released");

    while(1)
    {
        sleep(1);
    }
}

void ButtonPressedEvent(int btn_id)
{
    switch (btn_id)
    {
        case RESET_BTN:
            printf("Reset Button ");
            break;
        case TAMPER_BTN:
            printf("Tamper Button ");
            break;
    }
    printf("Pressed\n");
}

void ButtonReleasedEvent(int btn_id)
{
    switch (btn_id)
    {
        case RESET_BTN:
            printf("Reset Button ");
            break;
        case TAMPER_BTN:
            printf("Tamper Button ");
            break;
    }
    printf("Released\n");
}
