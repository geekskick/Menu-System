#ifndef DEVICE_H
#define DEVICE_H

#include "hash.h"
#include <stdlib.h>

/**
 * Masks for the different functionality available on each pin
 */
#define	PF_NONE_MASK		0
#define PF_ANALOG_MASK 		1
#define PF_DIGO_MASK		2
#define PF_DIGI_MASK		4
#define PF_GND_MASK			8
#define PF_VCC_MASK			16
#define N_FUNCTIONS			5

/**
 * Memory map information
 */
#define REGISTERS_PER_BANK	0x80
#define BANKS				4 
#define TOTAL_REGISTERS		BANKS * REGISTERS_PER_BANK

/**
 * @brief      for storing the registers in a hashmap
 */
struct reg{
	char* 		name;
	uint16_t 	address;
};

/**
 * @brief      The device's pin settings
 */
struct device_settings{
	int 	total_pins;
	int* 	available_pin_functions;
	int* 	current_pin_functions;
	uint8_t registers[TOTAL_REGISTERS];
	map_t	memory_map;
	uint8_t w_reg;
};

/**
 * @brief      The pin function. Each pin function has a description and a mask to go with it
 */
struct pin_function_options{
	char* 	text;
	int 	mask;
};

/**
 * @brief      The pin_function_option table items are in this order
 */
enum pin_function{ ANALOG = 0, INPUT, OUTPUT, VCC, GND, N_COLS };

/**
 * A Table mapping the desciption of the pin with it's mask
 */
static const struct pin_function_options pin_functions[N_FUNCTIONS] = { 
	{"Analog", 	PF_ANALOG_MASK 	},
	{"Input", 	PF_DIGI_MASK 	},
	{"Output", 	PF_DIGO_MASK 	},
	{"Vcc", 	PF_VCC_MASK 	},
	{"GND", 	PF_GND_MASK 	}
};

/**
 * The device pin functions have this signature
 */
typedef void (*device_pin_function)(struct device_settings *const p_chip, const int pin, const enum pin_function function);

void device_reset(struct device_settings * p_chip);
void device_print_chip(struct device_settings * const p_chip);
void device_init_chip(struct device_settings * const p_chip);
void device_deinit_chip(struct device_settings * const p_chip);
void device_enable_function(struct device_settings *const p_chip, const int pin, const enum pin_function function);
void device_toggle_function(struct device_settings *const p_chip, const int pin, const enum pin_function function);
void device_disable_function(struct device_settings *const p_chip, const int pin, const enum pin_function function);
void device_make_function_available(struct device_settings *const p_chip, const int pin, const enum pin_function function);
void device_make_function_unavailable(struct device_settings *const p_chip, const int pin, const enum pin_function function);
void device_toggle_function_availability(struct device_settings *const p_chip, const int pin, const enum pin_function function);
void device_reconfigure(struct device_settings * const p_chip);
void device_show_registers(struct device_settings * const p_chip);
#endif