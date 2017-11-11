// main.c
//-------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "layout.h"
#include "menu.h"
#include "device.h"
#include "asm.h"
#include "getch.h"
#include "hash.h"
#include "instructions.h"



//-------------------------------
void test(void *p);
void print_chip_bridge(void *p);
void mem_leak_test(void *p);
void reset_chip_bridge(void *p);
void reconfigure_device_menu(void *p);
void change_pin_capability_option(void *p);
void change_pin_function_option(void *p);
void menu_clear_bridge(void *p);
void hash_test(void *p);
//-------------------------------

/**
 * @brief      Allows the user to change pin functionality
 *
 * @param      p     Pointer to the device
 */
void reconfigure_device_menu(void* p){
	struct menu_options sub_menu[] = {
		{"Change pin capability", 			change_pin_capability_option	},
		{"Change pin current function",		change_pin_function_option 		},
		{"Return",							menu_quit						}
	};

	menu_loop(sub_menu, menu_length(sub_menu), "RECONFIGURE SUBMENU", p);
}

/**
 * @brief      Provides a bridge between the menu_fn and the menu_pin_selection_submenu.
 *
 * @param      p     Pointer to the device
 */
void change_pin_function_option(void*p){
	menu_pin_selection_submenu("Toggle function", &device_toggle_function, p);
}

/**
 * @brief      Provides a bridge between the menu_fn and the menu_pin_selection_submenu.
 *
 * @param      p     Pointer to the device
 */
void change_pin_capability_option(void*p){
	menu_pin_selection_submenu("Toggle capability of function", &device_toggle_function_availability, p);
}

/**
 * @brief      Used for unit testing in development
 *
 * @param      p     Pointer to the device
 */
void test(void* p){
	
	struct device_settings * p_chip = (struct device_settings*)p;
	printf("\t\tTESTING DEVICE\n");

	device_enable_function(p_chip, 24, ANALOG);
	device_enable_function(p_chip, 25, ANALOG);
	device_print_chip(p_chip);

	device_make_function_available(p_chip, 24, ANALOG);
	device_enable_function(p_chip, 24, ANALOG);
	device_print_chip(p_chip);

	device_disable_function(p_chip, 24, ANALOG);
	device_disable_function(p_chip, 25, ANALOG);
	device_print_chip(p_chip);
}

/**
 * @brief      Make a memory leak of 1 byte
 *
 * @param      p     Not used
 */
void mem_leak_test(void* p){
	(void)p; // stop warnings
	int i = (int)malloc(1);
	(void)i; // stop warnings
}

/**
 * @brief      Provides a bridge between the menu_fn signature the device_print_chip function
 *
 * @param      p     Pointer to the device
 */
void print_chip_bridge(void*p){
	device_print_chip((struct device_settings*)p);
}

/**
 * @brief      Provides a bridge between the menu_fn signature the device_reset function
 *
 * @param      p     Pointer to the device
 */
void reset_chip_bridge(void*p){
	device_reset((struct device_settings*)p);
}


/**
 * @brief      Provides a bridge between the menu_fn signature the menu_clear function
 *
 * @param      p     Pointer to the device
 */
void menu_clear_bridge(void*p){
	menu_cls();
}

/**
 * @brief      Tests the hashmap functionality
 *
 * @param      p     not used
 */
void hash_test(void *p){
	(void)p; //silince warnings
	map_t 	h_map = hashmap_new();
	assert(h_map);

	const struct instruction MNEMONICS[] = { 
		{ "MOV", 2, instruction_test_perform }, 
		{ "LDR", 2, instruction_test_perform }, 
		{ "STO", 2, instruction_test_perform }, 
		{ "PSH", 2, instruction_test_perform }, 
		{ "POP", 2, instruction_test_perform } 
	};

	for(int i = 0; i < 5; i++){
		hashmap_put(h_map, MNEMONICS[i].name, (void*)(MNEMONICS + i));
	}
	printf("Map made\n");

	struct instruction *p_out;
	for(int i = 0; i < 5; i++){
		printf("[%d] temp cleared\n", i);
		int rc = hashmap_get(h_map, MNEMONICS[i].name, (void**)&p_out);
		printf("[%d] map queried with %s\n", i, MNEMONICS[i].name);
		printf("[%d] %s = ", rc, p_out->name);
		p_out->perform(i, i);
	}
		
	hashmap_free(h_map);


}
//-------------------------------
int main(const int argc, const char ** argv){

	struct menu_options menu[] = {
		{"Exit", 				menu_quit 				},
		{"Show Device", 		print_chip_bridge		},
		{"Reconfigure Device", 	reconfigure_device_menu	},
		{"Test", 				test 					},
		{"Reset Device",		reset_chip_bridge  		},
		{"Make a memory leak",	mem_leak_test 			},
		{"Clear Screen",		menu_clear_bridge		},
		{"Load some .asm code", asm_menu				},
		{"Hash Test", 			hash_test				}
	};
	struct device_settings chip;

	printf("Welcome to the MicroChip 16F877A emulator\n");

	device_init_chip(&chip);
	menu_loop(menu, menu_length(menu), "MAIN MENU", &chip);
	device_deinit_chip(&chip);
	return EXIT_SUCCESS;
}
