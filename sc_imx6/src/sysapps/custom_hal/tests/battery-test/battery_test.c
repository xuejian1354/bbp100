#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <hal/battery.h>


// Private Functions
static void RunTest();
void power_event_handler(int eventData, powerEvent *extraInfo);
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
   
    printf("===Starting Battery Test===\n");
    hal_battery_register(power_event_handler);


    while(1)
    {
        sleep(1);
    }
}

void power_event_handler(int eventData, powerEvent *extraInfo)
{
    printf("\nPower EventData %d Received\n", eventData);
    switch (eventData)
    {
        case POWER_EVENT:
            printf("Power Event\n");
            break;
        case POWER_EVENT_CHARGER_ATTACHED:
            printf("Power Event - Charger Attached\n");
            break;
        case POWER_EVENT_CHARGER_DETTACHED:
            printf("Power Event - Charger Dettached\n");
            break;
        case POWER_EVENT_LOW_BATT:
            printf("Power Event - Low Battery Detected\n");
            break;
        case POWER_EVENT_LOW_BATT_RES:
            printf("Power Event - Low Battery Resolved\n");
            break;
        case POWER_EVENT_BATT_REM:
            printf("Power Event - Battery Removed\n");
            break;
        case POWER_EVENT_BATT_REM_RES:
            printf("Power Event - Battery Attached\n");
            break;
        case POWER_EVENT_BATT_BAD:
            printf("Power Event - Battery Bad\n");
            break;
        case POWER_EVENT_BATT_BAD_RES:
            printf("Power Event - Battery no longer bad\n");
            break;
    }
    printf(" Cable connected %d\n", extraInfo->cableConnected);
    printf(" Battery Connected %d\n", extraInfo->batteryConnected);
    printf(" Battery Charge Level %d\n", extraInfo->batteryChargeLevel);
    printf(" Battery Charging %d\n", extraInfo->batteryCharging);
    printf(" Battery Bad %d\n", extraInfo->batteryBad);
}
