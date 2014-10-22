#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <hal/sysinfoHW.h>
#include "sysinfoHW_test.h"

typedef unsigned char bool;

/**
 * @brief: This application tests the sysinfo hal implementation
 *         for a specific vendor.
 * @param argc : Number of arguments provided
 * @param argv : a list of the argument strings
 * @return 0 for success
 */
int main(int argc, char *argv[])
{
  uint8_t hwVersion;
  uint8_t macAddress[6];
  char    serialNumber[SERNUM_LENGTH];
  int     ret, i;
  bool    bValue;
  int     choice;
  
  //  Print the test information
	printf("\n\n[SysInfo-Test]  System Information (SysInfo) HAL Test.\n");
  printf("[SysInfo-Test]  This test will test each of the sysinfo HAL functions.\n\n\n");

  // Get the hardware version
  ret = hal_sysinfo_get_hwver(&hwVersion);
  if(ret==0)
  {
    printf("\t\tHardware Version: \t%c\n", hwVersion);  
  }
  else
  {
    printf("\t\tHardware Version: \tERROR = %d\n", ret);  
  }

  // Get the Serial Number
  memset(serialNumber, '\0', SERNUM_LENGTH);
  ret = hal_sysinfo_get_serialnum(&serialNumber[0], SERNUM_LENGTH-1);
  if(ret==0)
  {
    printf("\t\tSerial Number: \t\t%s\n", serialNumber);
  }
    else
  {
    printf("\t\tSerial Number: \t\tERROR = %d\n", ret); 
  } 
  
  // Get the MAC Address
  ret = hal_sysinfo_get_macaddr(&macAddress[0]);
  if(ret==0)
  {
    printf("\t\tMAC Address: \t\t");
  	for(i = 0;i < 5; i++)
  	{
  		printf("%.2x:",macAddress[i]);
  	}
  	printf("%.2x\n",macAddress[i]);
  }
    else
  {
    printf("\t\tMAC Address: \t\tERROR = %d\n", ret);  
  }
  
  printf("\n\n[SysInfo-Test] Now, read the capabilities. Note that for some processors (e.g. i.MX51),\n");
  printf("[SysInfo-Test] an error may print if the fuse bit has already been blown.\n\n");
  sleep(1);

  // Get the NAND boot info
  bValue = hal_sysinfo_get_capability(SYSINFO_DEVICE_MEM_LOCKDOWN);
  sleep(1);
  if(bValue)
      printf("\n\t\tNAND Boot Lock: \tTRUE\n");
  else
      printf("\n\t\tNAND Boot Lock: \tFALSE\n"); 


    // Now, scan if user wants to program the device to only boot from NAND flash.

    /* Display the menu */
    printf("\n\n\n[SysInfo-Test]  Press '0' to Set Boot From NAND Only. ");
    printf("\tWARNING - THIS MAY NOT BE REVERSIBLE.\n");
    printf("[SysInfo-Test]  Press any other key to EXIT.\n\n");

    /* Get the users request */
    scanf("%d", &choice);
    printf("\n\n\n");

    /* Decide what to do with it */
    if(choice == 0)
    {
      printf("\t\tProgramming to boot from NAND only...\n");
      printf("\n\nIf this is a fuse and it has already been blown, then the driver firmware may output error messages.\n\n");
      sleep(2);
      ret = hal_sysinfo_set_capability(SYSINFO_DEVICE_MEM_LOCKDOWN);
      if(ret == 0)
      {      
        printf("\t\tProgramming SUCCEEDED. Exiting.\n\n\n\n");
      }
      else
      {
        printf("\t\tProgramming FAILED!!! Exiting\n\n\n\n");
      }
    }
	return EXIT_SUCCESS;
}
