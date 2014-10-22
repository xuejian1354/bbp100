/**
 * @file ledHW_test.c
 * @brief Test LED HAL implementation
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <hal/ledHW.h>

#define COLORMAX 8

// This maps the led's index to a name, useful for printout
const char *led_name_lookup[LED_COUNT] = {
	"Status LED", "Power LED", "Ethernet LED", "WiFi LED", "ZigBee LED",
	"Cellular LED", "Error LED"
};

// Private data
static int auto_flag;

/*
 * @brief Print the list of leds supported by the vendor
 *
 * @param none
 * @return Number of LEDs on H/W
 */
static int printLedList()
{
	int i;
	led_name_t *led_list;
	int num_leds = hal_led_get_supported_list(&led_list);
	for (i = 0; i < num_leds; ++i)
		printf("\tIndex %d: %s\n", led_list[i], led_name_lookup[led_list[i]]);

	printf("\n");
	return num_leds;
}

/**
 * @brief Ask the user to specify which led they would like to test
 * @param none
 * @return the index of the led to test
 */
static led_name_t get_index()
{
	char user_input[5];
	int index, num_leds = printLedList();

	do {
		printf("Choose Led To Test: ");
		scanf("%s", user_input);
		while (getchar() != '\n');

		if (!isdigit(user_input[0])) {
			printf("Invalid Choice: Must supply a number\n");
		} else {
			index = atoi(user_input);
			if (index < 0)
				printf("Invalid Choice: Number must be positive\n");
			else if (index >= num_leds)
				printf("Invalid Choice: Number must be less than %d\n", LED_COUNT);
			else
				// Given a valid number, exit the choice menu
				return index;
		}
	} while (1);
}

/*
 * @brief Test the led over a series of colors
 * @param index: the index of the led to test
 * @return 0 for success
 */
static int color_sweep(led_name_t index)
{
	char junk;
	int color, sweep_passed = 0;
	const char color_names[COLORMAX][7] = {"Red", "Green", "Blue", "Purple", "Yellow", "Orange", "Pink", "Off"};
	const int colors[COLORMAX] = {
		RED_LED,
		GREEN_LED,
		BLUE_LED,
		PURPLE_LED,
		YELLOW_LED,
		ORANGE_LED,
		PINK_LED,
		0x000000,  // Off
	};

	printf("Testing All Colors for led %s with index %d\n", led_name_lookup[index], index);

	for (color = 0; color < COLORMAX; color++) {
		int pass = 0, result;
		printf("Setting %s color to %s\n", led_name_lookup[index], color_names[color]);

		pass |= hal_led_set_color(index, colors[color]);
		pass |= hal_led_get_color(index, &result);
		pass |= colors[color] != result;

		if (0 == pass)
			printf("\tPassed auto test\n");
		else
			printf("*****FAILED!!!!!!!***** - RGB Colors not set properly\n");

		// During auto tests, do not wait for user input
		if (!auto_flag) {
			printf("\tInspect Manually, then Press enter to continue...\n");
			scanf("%c", &junk);

			/* Dim test for colors other than OFF */
			if (colors[color]) {
				hal_led_set_brightness(index, 50);
				printf("\tConfirm that the LED has dimmed. Press enter to continue...\n");
				scanf("%c", &junk);
				hal_led_set_brightness(index, 100);
			}
		}
		sweep_passed |= pass;
	}

	printf("Finished color sweep on leds\n\n");
	return sweep_passed;
}

/*
 * @brief Blink LED in given mode
 * @param led_name_t index: index of led to test
 * @param int mode: blink mode
 * @return none
 */
void blink(led_name_t index, int mode)
{
	int i;
	char led_str[3][6] = {"Red", "Green", "Blue"};
	char mode_str[3][5] = {"off", "slow", "fast"};
	char junk;

	for (i = 0; i < 3; i++) {
		printf("Blinking %s LED %s\n", led_str[i], mode_str[mode]);
		hal_led_set_color(index, 0xff0000>>i*8);
		hal_led_set_blink(index, mode);
		printf("\tInspect Manually, then Press enter to continue...\n");
		scanf("%c", &junk);
		hal_led_set_blink(index, 0);
	}
}

/*
 * @brief Sweep over all color & supported blink modes
 * @param index of led to test
 * @return none
 */
static void sweep_all_colors_blink(led_name_t index)
{
	int finalPass = 0;

	finalPass = color_sweep(index);
	blink(index, BLINK_SLOW);
	blink(index, BLINK_FAST);

	printf("Final Grade: %s\n", finalPass ? "FAIL" : "PASS");
}

/**
 * @brief This function initiates the test
 * @param none
 * @return none
 */
static void run_test()
{
	int index, i;

	printf("===Starting Led Test===\n");

	// initialize the brightness value to ON (100%)
	for (index = 0; index < LED_COUNT; ++index)
		hal_led_set_brightness(index, 100);

	if (auto_flag) {
		for (index = 0; index < LED_COUNT; ++index)
			sweep_all_colors_blink(index);
	} else {
		index = get_index();
		sweep_all_colors_blink(index);
	}
}

/*
 * @brief: This application tests the led hal implementation
 *         for a specific vendor.
 * @param argc : Number of arguments provided
 * @param argv : a list of the argument strings
 * @return 0 for success
 */
int main(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "ah")) != -1) {
		switch(opt) {
		case 'a':
			auto_flag = 1;
			break;
		case 'h':
			printf("Usage: %s [flags]\n", argv[0]);
			printf("  -a : only run automatic tests\n");
			printf("  -h : display this usage\n");
			return 0;
		default:
			// Getopt is nice enough to yell at user, just need to exit
			return EINVAL;
		}
	}

	run_test();
	return 0;
}
