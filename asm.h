#ifndef ASM_H
#define ASM_H


#include <stdio.h>
#include "device.h"
#include "instructions.h"
#include "hash.h"

/**
 * The size of the buffer for containg temp or short strings
 */
#define ASM_FILENAME_LEN 		254
/**
 * The key to press which will stop the file viewer
 */
#define	ASM_SCROLLER_QUIT_KEY 	'q'

struct asm_args;
/**
 * A Function for handling input on the file viewer
 */
typedef void (*asm_input_handler)(const char input, struct asm_args * const p_self);

/**
 * @brief      The Loaded ASM file information
 */
struct asm_file{
	char			file_name[ASM_FILENAME_LEN]; 
	int 			current_line;
	int 			max_lines;
	FILE 			*fp;
	char 			**file_contents;
};

/**
 * @brief      For passing to functions which might need the asm file and the device settings
 */
struct asm_args{
	struct asm_file 		*p_file;
	struct device_settings 	*p_device;
	asm_input_handler 		handle_input;
	map_t					mnemonic_map;
	char					quit;
};

void asm_menu(void*p);

#endif