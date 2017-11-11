#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h> // term size
#include <unistd.h>

#include "menu.h"


/**
 * @brief      Prints a menu to the screen, with a prompt for user input
 *
 * @param[in]  p_menu  Pointer to the menu_options struct with the things to print
 * @param[in]  len     The length of the menu
 * @param[in]  msg     The message to display in the banner
 */
void menu_print(const struct menu_options* const p_menu, const int len, const char * const msg){

	printf("------------------------------------------\n");
	printf("%s\n", msg);
	printf("------------------------------------------\n");

	for(int i = 0; i < len; i++){
		printf("%2d - %s\n", i, p_menu[i].text);
	}
	printf(">\t");

}

/**
 * @brief      Gets the height of th current STDOUT console
 *
 * @return     The number of rows in the console
 */
int menu_get_height(void){
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

/**
 * @brief      Sets the quit flag
 *
 * @param      p     Pointer to the quit flag
 */
void menu_quit(void* p){
	*(int*)p = 1;
}

/**
 * @brief      Use this to display 'Not Implemented' as a placeholder in the menu_options struct
 *
 * @param      p     Not used
 */
void menu_not_implemented(void * p){
	(void)p;
	printf("Not implemented that function yet!\n");
}

/**
 * @brief      Clears the terminal by printing '\033[2J'
 */
void menu_cls(void){		
	printf("\033[2J");
}

/**
 * @brief      Enters a menu loop, and acts on the user selection
 *
 * @param[in]  p_menu  Pointer to the menu_options struct
 * @param[in]  len     The length of the menu
 * @param[in]  msg     The message to display
 * @param      chip    Pointer to the device
 */
void menu_loop(const struct menu_options* const p_menu, const int len, const char * const msg, void *chip){
	int quit_flag = 0;
	char c;
	while(!quit_flag){
		menu_print(p_menu, len, msg);
		scanf(" %c", &c);
		//printf("Read a %c[%X]\n", c, c);
		int idx = atoi(&c);
		if(p_menu[idx].perform == &menu_quit){ p_menu[idx].perform(&quit_flag);}
		else{ p_menu[idx].perform(chip); }
	}
}

/**
 * @brief      This is specifically for selecting a pin and doing something with it.
 *
 * @param[in]  msg   The message to display in the banner
 * @param[in]  fn    The function pointer describing what to do with that pin once selected
 * @param      p     Pointer to the device usually. Will be passed to the first argument of the fn.
 */
void menu_pin_selection_submenu(const char * const msg, device_pin_function fn, void* p){
	char pin;
	int pin_selection = 1;
	int function;
	while(pin_selection){
		printf("Choose pin, or 'q' to return\n>\t");
		scanf(" %c", &pin);
		//printf("Selected %c\n", pin);

		if(pin == 'q'){ break; }
		int pin_i = atoi(&pin);
		printf("%s on pin %d, or out of limits to ignore:\n", msg, pin_i);
		for(int f = 0; f < N_FUNCTIONS; f++){
			printf("%d - %s\n", f, pin_functions[f].text);
		}
		scanf(" %d", &function);
		//printf("Selected %d\n", function);
		fn(p, pin_i-1, function); 
	}
	
}