#include "asm.h"
#include "menu.h"
#include "getch.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assert.h>

// The mneomics will look like this 
// MOVF xxx, yyy

/**
 * Seperates the operands from each other
 */
#define OP_SEPERATOR 		","
/**
 * Separetes the mnemonic from the operands
 */
#define MNEMONIC_SEPERATOR 	" "

//------------------------
#warning SORT OUT THESE FUNCTION NAMES
void 	asm_scroll_basics(const char input, struct asm_args * const p_self);
void 	show_asm(void *p);
void 	load_asm(void *p);
void 	deinit_asm(void *p);
void 	asm_run(void *p);
void 	asm_run_handler(const char input, struct asm_args * const p_self);
int 	asm_get_op(const char * const op, map_t h_map);
void 	asm_mnemonics_map_populate(map_t *p_map);
int 	asm_get_op_count(const char *p_op1, const char *p_op2);
//------------------------

/**
 * @brief      Sets up, and then calls the scrolling file viewer. Makes the input hanlder the asm_run_handler
 *
 * @param      p     A pointer to the asm_args struct
 */
void asm_run(void *p){
	((struct asm_args*)p)->handle_input = asm_run_handler;
	show_asm(p);
}

/**
 * @brief      Populates the hashmap with the instruction set
 *
 * @param      p_map  Pointer to the map to populate
 */
void asm_mnemonics_map_populate(map_t *p_map){

	const struct instruction MNEMONICS[] = { 
		{ "ADDWF", 	2, instruction_test }, 
		{ "ANDWF", 	2, instruction_test }, 
		{ "CLRF", 	1, instruction_test }, 
		{ "CLRW", 	0, instruction_test }, 
		{ "COMF", 	2, instruction_test },
		{ "DECF", 	2, instruction_test },
		{ "DECFSZ", 2, instruction_test },
		{ "INCF", 	2, instruction_test },
		{ "INCFSZ", 2, instruction_test },
		{ "IORWF", 	2, instruction_test },
		{ "MOVF", 	2, instruction_movf },
		{ "MOVWF", 	1, instruction_movwf },
		{ "NOP", 	0, instruction_nop },
		{ "RLF", 	2, instruction_test },
		{ "RRF", 	2, instruction_test },
		{ "SUBWF", 	2, instruction_test },
		{ "SWAPF", 	2, instruction_test },
		{ "XORWF", 	2, instruction_test },
		{ "COMF", 	2, instruction_test },
		{ "BCF", 	2, instruction_test },
		{ "BSF", 	2, instruction_test },
		{ "BTFSC", 	2, instruction_test },
		{ "BTFSS", 	2, instruction_test },
		{ "ANDLW", 	1, instruction_test },
		{ "ADDLW", 	1, instruction_test },
		{ "CALL", 	1, instruction_test },
		{ "CLRWDT", 0, instruction_test },
		{ "GOTO", 	1, instruction_test },
		{ "IORLW", 	1, instruction_test },
		{ "MOVLW", 	1, instruction_movlw },
		{ "RETFIE", 0, instruction_test },
		{ "RETLW", 	1, instruction_test },
		{ "RETURN", 0, instruction_test },
		{ "SLEEP", 	0, instruction_test },
		{ "SUBLW", 	1, instruction_test },
		{ "XORLW", 	1, instruction_test }
	};

	for(int i = 0; i < (sizeof(MNEMONICS)/sizeof(*MNEMONICS)); i++){
		//printf("%d = %s going into %p\n", i, MNEMONICS[i].name, *p_map);
		assert(MAP_OK == hashmap_put(*p_map, MNEMONICS[i].name, (void*)(MNEMONICS + i)));
	}
}

/**
 * @brief      Count the valid operands
 *
 * @param[in]  p_op1  Pointer to the first operand token
 * @param[in]  p_op2  Pointer to the second operand token
 *
 * @return     The number of valid operands
 */
int asm_get_op_count(const char *p_op1, const char *p_op2){
	if(!p_op1){ return 0; }
	if(!p_op2){ return 1; }
	return 2;
}

/**
 * @brief      Get the operad value from the string
 *
 * @param[in]  op     The operand string
 * @param[in]  h_map  The hashmap containing the device memory map
 *
 * @return     0 if not valid, else the operand value
 */
int asm_get_op(const char * const op, map_t h_map){
	int 		op_int = 0;
	struct reg 	*op_struct;
	if(op){
		op_int = (int)strtol(op, NULL, 16);
		  if(hashmap_get(h_map, op, (void**)&op_struct) == MAP_OK){
			op_int = op_struct->address;
		}
	}
	return op_int;
}
/**
 * @brief      Handles input when the asm_run function has been called. Gets the current line and splits it into it's opcode and operands, then does that to the device
 * @warning    Not completed yet.
 * @param[in]  input   The input character from the user
 * @param      p_self  Pointer to the asm_args struct.
 */
void asm_run_handler(const char input, struct asm_args * const p_self){
	
	if(input == ASM_SCROLLER_DEBUG_KEY){
		menu_cls();
		device_show_registers(p_self->p_device);
		// block to prevent the menu from being displayed again
		getch();
	}
	else if (input == ASM_SCROLLER_RUN_KEY){
		char 				*p_line 	= p_self->p_file->file_contents[p_self->p_file->current_line];
		struct instruction 	*map_out;
		static char 		temp_str[ASM_FILENAME_LEN];
		strcpy(temp_str, p_line);

		char 				*mnemonic 	= strtok(temp_str, 	MNEMONIC_SEPERATOR);
		char 				*op1 		= strtok(NULL, 		OP_SEPERATOR);
		char 				*op2	 	= strtok(NULL, 		OP_SEPERATOR);
		//int 				operand_num = asm_get_op_count(op1, op2);

		printf("[%s][%s][%s]\n", mnemonic, op1, op2);

		// no mnemonic found
		if(NULL == mnemonic){ return; }
		if(MAP_OK != hashmap_get(p_self->mnemonic_map, mnemonic, (void**)&map_out)){
			// not in map not sure what i want it to do here, so for now just ignore it.
		}
		else{
			int op1_int = asm_get_op(op1, p_self->p_device->memory_map);
			int op2_int = asm_get_op(op2, p_self->p_device->memory_map);

			printf("Performing: %s with ops [%d][%d]\n", mnemonic, op1_int, op2_int);
			map_out->perform(p_self->p_device, op1_int, op2_int);
		}

		memset(temp_str, 0, ASM_FILENAME_LEN * sizeof(*temp_str));
	}

}

/**
 * @brief      Shows the assembly file on the screen. Allows for basic scrolling up/down and quitting. any additional functionality should be specified in the asm_args struct's input handler.
 *
 * @param      p     Pointer to the asm_args struct
 */
void show_asm(void* p){
	const int 				rows			= menu_get_height() - 2; // one line for the header, and one blank line at the end
	struct 	asm_args		*p_args			= (struct asm_args*)p;
	struct 	asm_file 		*p_loaded_file	= p_args->p_file;
	int 					start_line		= 0;
	int 					half_screen		= rows/2;
	char 					c 				= 'k';
	//p_loaded_file->current_line 			= 0; // Uncomment to make it start at the top of the file each time, else it will remember where the user left off
	p_args->quit 							= 0;
	
	while(!p_args->quit){
		menu_cls();
		start_line = p_loaded_file->current_line - half_screen;

		// allow the cursor to go down when near the button of the file
		if(start_line + rows > p_loaded_file->max_lines){ start_line = p_loaded_file->max_lines - rows; }
		// when near the top of the file start from line 0
		if(start_line < 0){ start_line = 0; }

		// Header Line
		printf("File:%s\tTotal Lines:%d\tCurrent Line:%d\t'%c' to quit\t'%c' to debug\t'%c' to run instruction\n", p_loaded_file->file_name, p_loaded_file->max_lines, p_loaded_file->current_line+1, ASM_SCROLLER_QUIT_KEY, ASM_SCROLLER_DEBUG_KEY, ASM_SCROLLER_RUN_KEY);


		for(int display_line = 0; display_line < rows && start_line < p_loaded_file->max_lines; display_line++, start_line++){
			// show the current selected line with an asterisc
			printf("%.2d %c> %s\n", start_line + 1, start_line == p_loaded_file->current_line? '*' : ' ', p_loaded_file->file_contents[start_line]);
		}

		c = getch();
		asm_scroll_basics(c, p_args);
		if(p_args->handle_input) { p_args->handle_input(c, p_args); }
		
		// prevent wrap around/out of limits
		if(p_loaded_file->current_line < 0){ p_loaded_file->current_line = 0; }
		if(p_loaded_file->current_line >= p_loaded_file->max_lines){ p_loaded_file->current_line = p_loaded_file->max_lines-1; }

	}
}

/**
 * @brief      Loads an assembly file into the struct
 *
 * @param      p     A pointer to the asm_args struct
 */
void load_asm(void* p){

	struct 	asm_args* p_args 			= (struct asm_args*)p;
	struct 	asm_file *p_loaded_file 	= p_args->p_file;

	if(p_loaded_file->max_lines > 0){ deinit_asm(p); }

	printf("Enter path to .asm file\n>\t");
	scanf(" %s", p_loaded_file->file_name);

	p_loaded_file->fp = fopen(p_loaded_file->file_name, "r");
	if(!(p_loaded_file->fp)){
		printf("Sorry, I can't open %s\n", p_loaded_file->file_name);
		return;
	}

	char temp_line[ASM_FILENAME_LEN] = { 0, };

	while(fgets(temp_line, sizeof(temp_line), p_loaded_file->fp)){

		temp_line[strlen(temp_line)-1] = '\0'; //remove \n 
		p_loaded_file->file_contents = realloc(p_loaded_file->file_contents, ++(p_loaded_file->max_lines) * sizeof(p_loaded_file->file_contents));
		p_loaded_file->file_contents[p_loaded_file->current_line] = calloc(strlen(temp_line), sizeof(char));
		strcpy(p_loaded_file->file_contents[p_loaded_file->current_line], temp_line);
		p_loaded_file->current_line++;
		memset(temp_line, 0, ASM_FILENAME_LEN * sizeof(char));
	}
	fclose(p_loaded_file->fp);
	printf("There are %d [%d] lines in %s \n", p_loaded_file->max_lines, p_loaded_file->current_line, p_loaded_file->file_name);

	/**
	 * Each time a new file is loaded start at it's first line
	 */
	p_loaded_file->current_line = 0;

}

/**
 * @brief      Cleans up the loaded asm file and cache contents
 *
 * @param      p     A pointer to the asm_args struct
 */
void deinit_asm(void* p){
	struct asm_args* p_args = (struct asm_args*)p;
	struct asm_file *p_loaded_file = p_args->p_file;

	for(int l = 0; l < p_loaded_file->max_lines; l++){
		free(p_loaded_file->file_contents[l]);
		p_loaded_file->file_contents[l] = 0;
	}
	free(p_loaded_file->file_contents);

	p_loaded_file->file_contents 	= 0;
	p_loaded_file->max_lines 		= 0;
	p_loaded_file->current_line 	= 0;
	memset(p_loaded_file->file_name, 0, ASM_FILENAME_LEN);

	hashmap_free(p_args->mnemonic_map);

}


/**
 * @brief      Setup, display and handle the assembly language file submenu
 *
 * @param      p     A pointer to the device struct
 */
void asm_menu(void*p){
	struct asm_file contents 		= { { 0, }, 0, 0, 0, 0 };
	struct asm_args args 			= { &contents, p, 0, 0, 0 };
	struct menu_options sub_menu[] 	= {
		{ "Return",			menu_quit 	},
		{ "Load New File",  load_asm 	},
		{ "Browse File", 	show_asm	},
		{ "Run File",		asm_run		}
	};
	
	args.mnemonic_map = hashmap_new();
	asm_mnemonics_map_populate(&(args.mnemonic_map));
	load_asm(&args);
	menu_loop(sub_menu, menu_length(sub_menu), "ASM SUBMENU", &args);
	deinit_asm(&args);

}

/**
 * @brief      Handles the scrolling and quitting of the file viewer only.
 *
 * @param[in]  input   The input
 * @param      p_self  The p self
 */
void asm_scroll_basics(const char input, struct asm_args * const p_self){
	struct 	asm_file 	*p_loaded_file	= p_self->p_file;

	 // An arrow sends \033[A or \033[B
	if(input =='\033'){
		getch();
		switch(getch()){
			case 0x42: 	/* printf("Down Pressed\n"); */	p_loaded_file->current_line++;	break;
			case 0x41: 	/* printf("Up Pressed\n");   */	p_loaded_file->current_line--;	break;
			default:	break;
		}
	}
	else{
		switch(input){
			// upper or lowercase quit keyp
			case ASM_SCROLLER_QUIT_KEY:
			case ASM_SCROLLER_QUIT_KEY ^ (1 << 5): p_self->quit = 1; break;
			default: break;
		}
	}
}