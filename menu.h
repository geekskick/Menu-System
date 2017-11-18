#ifndef MENU_H
#define MENU_H
#include "device.h"

/**
 * The function to be called when a menu option is selected
 */
typedef void (*menu_fn)(void*);

/**
 * @brief      Gets the length of the menu_options table 
 *
 * @param      m     The menu options table
 *
 * @return     It's length
 */
#define menu_length(m) 	sizeof(m)/sizeof(*m)

/**
 * @brief      The menu options have something to display, and something to do when selected.
 */
struct 	menu_options {
	char 	*text;
	menu_fn perform;
};

/**
 * @brief      The different menu text colours
 */
enum 	font_colour{ black = 30, red, green, yellow, blue, magenta, cyan, white };
void 	menu_cls(void);
void 	menu_print(const struct menu_options* const p_menu, const int len, const char * const msg);
void 	menu_not_implemented(void*);
void 	menu_quit(void*);
void 	menu_loop(const struct menu_options* const p_menu, const int len, const char * const msg, void *chip);
int 	menu_get_height(void);
void 	menu_change_colour(const enum font_colour new_colour);
void 	menu_reset_colour(void);

#endif //MENU_H
