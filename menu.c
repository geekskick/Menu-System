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
 * @brief      Gets the height of the current STDOUT console
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
	char input[10];
	while(!quit_flag){
		menu_print(p_menu, len, msg);
		scanf(" %s", input);
		//printf("Read a %c[%X]\n", c, c);
		int idx = strtol(input, NULL, 10);
		if(p_menu[idx].perform == &menu_quit){ p_menu[idx].perform(&quit_flag);}
		else{ p_menu[idx].perform(chip); }
	}
}

/**
 * @brief      Change the foreground colour of the terminal
 *
 * @param[in]  new_colour  The new colour
 */
void menu_change_colour(const enum font_colour new_colour){
	printf("\033[%dm", (int)new_colour);
}

/**
 * @brief      Resets the colour to default
 */
void menu_reset_colour(void){
	printf("\033[0m");
}