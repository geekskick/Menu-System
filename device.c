#include <stdio.h>
#include <stdlib.h>
#include "device.h"
#include "layout.h"
#include <assert.h>

void print_display_cell(const int COLUMN_WIDTH, const int pin_functions, const int pin_function_selected, const int pin_function_to_check);

/**
 * @brief      Check if a pin is out of the limits of the device. Eg if the pin is 60 but the device only has 8 pins
 *
 * @param[in]  p_chip  A pointer to the device
 * @param[in]  pin     The pin
 *
 * @return     True if the pin is out of limits
 */
int pin_oolimits_check(const struct device_settings * const p_chip, const int pin){
	return (p_chip->total_pins <= pin) || (pin < 0);
}

/**
 * @brief      Checks if the function index is actually an available pin function. Eg if the user tries to enable function 11, but there are only 2 in total.
 *
 * @param[in]  fn_idx  The function index
 *
 * @return     True if it's out of limits
 */
int function_ool_check(const int fn_idx){
	return (fn_idx >= N_FUNCTIONS) || (fn_idx < 0);
}

/**
 * @brief      Enable a function on the pin, if that pin has the capability to do that. Prints a success/error message. Returns if the pin or function is out of limits
 *
 * @param      p_chip     A pointer to the device
 * @param[in]  pin       The pin
 * @param[in]  function  The function to enable
 */
void device_enable_function(struct device_settings *const p_chip, const int pin, const enum pin_function function){
	if(pin_oolimits_check(p_chip, pin) || function_ool_check(function)) { return; }
	if(p_chip->available_pin_functions[pin] & pin_functions[function].mask){
		p_chip->current_pin_functions[pin] |= pin_functions[function].mask;
		printf("Pin %d function %s enabled\n", pin, pin_functions[function].text);
	}
	else{
		printf("Pin %d isn't capable of supporting function %s\n", pin, pin_functions[function].text);
	}
}

/**
 * @brief      Disable a function on the pin. Prints a success message. Returns if the pin or function is out of limits
 *
 * @param      p_chip    A pointer to the device settings
 * @param[in]  pin       The pin
 * @param[in]  function  The function
 */
void device_disable_function(struct device_settings *const p_chip, const int pin, const enum pin_function function){
	if(pin_oolimits_check(p_chip, pin) || function_ool_check(function)) { return; }
	p_chip->current_pin_functions[pin] &= ~pin_functions[function].mask;
	printf("Pin %d function %s disabled\n", pin, pin_functions[function].text);
}

/**
 * @brief      Enable the capability of a function on a pin. Prints a success message. Returns if the pin or function is out of limits 
 *
 * @param      p_chip    A pointer to the device
 * @param[in]  pin       The pin
 * @param[in]  function  The function
 */
void device_make_function_available(struct device_settings *const p_chip, const int pin, const enum pin_function function){
	if(pin_oolimits_check(p_chip, pin) || function_ool_check(function)) { return; }
	p_chip->available_pin_functions[pin] |= pin_functions[function].mask;
	printf("Pin %d can support %s\n", pin, pin_functions[function].text);
}

/**
 * @brief      Disable the capability of a function on a pin. Prints a success message. Returns if the pin or function is out of limits 
 *
 * @param      p_chip    A pointer to the device
 * @param[in]  pin       The pin
 * @param[in]  function  The function
 */
void device_make_function_unavailable(struct device_settings *const p_chip, const int pin, const enum pin_function function){
	if(pin_oolimits_check(p_chip, pin) || function_ool_check(function)) { return; }
	p_chip->available_pin_functions[pin] |= ~pin_functions[function].mask;
	printf("Pin %d cannot support %s\n", pin, pin_functions[function].text);
}

/**
 * @brief      Toggles the functionality on a pin, if it has that capability. Prints a success/error message. Returns if function or pin is out of limits.
 *
 * @param      p_chip    Pointer to the device
 * @param[in]  pin       The pin
 * @param[in]  function  The function
 */
void device_toggle_function(struct device_settings *const p_chip, const int pin, const enum pin_function function){
	if(pin_oolimits_check(p_chip, pin) || function_ool_check(function)) { return; }
	if(p_chip->available_pin_functions[pin] & pin_functions[function].mask){
		p_chip->current_pin_functions[pin] ^= pin_functions[function].mask;
		printf("Toggling %s function on pin %d\n", pin_functions[function].text, pin);
	}
	else{
		printf("Pin %d cannot support %s\n", pin, pin_functions[function].text);
	}
}
/**
 * @brief      Toggles the cpability of a pin. Prints a success. Returns if function or pin is out of limits.
 *
 * @param      p_chip    Pointer to the device
 * @param[in]  pin       The pin
 * @param[in]  function  The function
 */

void device_toggle_function_availability(struct device_settings *const p_chip, const int pin, const enum pin_function function){
	if(pin_oolimits_check(p_chip, pin) || function_ool_check(function)) { return; }
	p_chip->available_pin_functions[pin] ^= pin_functions[function].mask;
	printf("Toggling %s function availability on pin %d\n", pin_functions[function].text, pin);

}

/**
 * @brief      Initialise the device.
 *
 * @param      p_chip Pointer to the device
 */
void device_init_chip(struct device_settings * const p_chip){
	printf("\t\tINITIALISING DEVICE\n");

	p_chip->total_pins 				= CHIP_PINS;
	p_chip->available_pin_functions = calloc(CHIP_PINS, sizeof(int));
	p_chip->current_pin_functions 	= calloc(CHIP_PINS, sizeof(int));
	p_chip->memory_map 				= hashmap_new();

// Analogs
	device_make_function_available(p_chip, 2,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 3,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 4,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 5,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 7,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 8,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 9,  ANALOG | INPUT | OUTPUT 	);
	device_make_function_available(p_chip, 10, ANALOG | INPUT | OUTPUT 	);

// Digital io only
	device_make_function_available(p_chip, 6,  INPUT  | OUTPUT 			);
	device_make_function_available(p_chip, 15, INPUT  | OUTPUT 			);

#warning INCOMPLETE CHIP PIN DEFINITION

	device_make_function_available(p_chip, 12, GND);
	device_make_function_available(p_chip, 31, GND);
	device_make_function_available(p_chip, 11, VCC);
	device_make_function_available(p_chip, 32, VCC);

#warning INCOMPLETE MEMORY MAP
	const struct reg mem[] = {
		{ "TMR0", 	0x1 },
		{ "PCL", 	0x2 },
		{ "STATUS", 0x3 },
		{ "FSR", 	0x4 },
		{ "PORTA", 	0x5 },
		{ "PORTB", 	0x6 },
		{ "PORTC", 	0x7 },
		{ "PORTD", 	0x8 }
	};

	for(int i = 0; i < sizeof(mem)/sizeof(*mem); i++){
		assert(MAP_OK == hashmap_put(p_chip->memory_map, mem[i].name, (void*)(mem + i)));
	}
}

/**
 * @brief      Clean up memory allocted to the device
 *
 * @param      p_chip  Pointer to the device
 */
void device_deinit_chip(struct device_settings * const p_chip){
	printf("\t\tDEINITIALISING DEVICE\n");
	free(p_chip->current_pin_functions);
	free(p_chip->available_pin_functions);
	free(p_chip->memory_map);
}

/**
 * @brief      Displays a cell of the 'print device' table. Displays an 'A' if the pin has that capability. and an 'S' if it's enabled.
 *
 * @param[in]  COLUMN_WIDTH           The column width
 * @param[in]  pin_functions          The pin function capability mask of the selected pin
 * @param[in]  pin_function_selected  The pin function enabled mask of the selected pin
 * @param[in]  pin_function_to_check  The pin function's mask to check against
 */
void print_display_cell(const int COLUMN_WIDTH, const int pin_functions, const int pin_function_selected, const int pin_function_to_check){
	const char * AVAILABLE_STR 	= "A";
	const char * SELECTED_STR 	= "S";
	printf("| %*s%*s ", 	COLUMN_WIDTH/2, pin_functions & pin_function_to_check? AVAILABLE_STR : "", COLUMN_WIDTH/2, pin_function_selected & pin_function_to_check? 	SELECTED_STR : "");

}

/**
 * @brief      Prints the device settings to the screen in a big table
 *
 * @param      p_chip  Pointer to the device
 */
void device_print_chip(struct device_settings * const p_chip){

	printf("\t\tSHOWING DEVICE\n");
	static const int 	COLUMN_WIDTH 	= 	6; // Max width of the COLUMN_HEADERS

	for(int c = 0; c < N_FUNCTIONS; c++){
		printf("| %*s ", COLUMN_WIDTH, pin_functions[c].text);
	}
	printf("|---DEVICE---");
	for(int c = N_FUNCTIONS - 1 ; c >= 0; c--){
		printf("| %*s ", COLUMN_WIDTH, pin_functions[c].text);
	}
	printf("|\n");

	for(int row = 0; row < (p_chip->total_pins)/2; row++){
		for(int c = 0; c < N_FUNCTIONS; c++){
			print_display_cell(COLUMN_WIDTH,  p_chip->available_pin_functions[row], p_chip->current_pin_functions[row], pin_functions[c].mask);
		}
		printf("| %-5d%5d ", 	row+1, CHIP_HEIGHT+row+1);
		for(int c = N_FUNCTIONS -1 ; c >= 0; c--){
			print_display_cell(COLUMN_WIDTH,  p_chip->available_pin_functions[row + CHIP_HEIGHT], p_chip->current_pin_functions[row + CHIP_HEIGHT], pin_functions[c].mask);
		}
		printf("|\n");
	}
}

/**
 * @brief      Cleans up, then reinitialises the device
 *
 * @param      p_chip  Pointer to the device to reset
 */
void device_reset(struct device_settings * p_chip){
	device_deinit_chip(p_chip);
	device_init_chip(p_chip);
}