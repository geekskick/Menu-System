// main.c
//-------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "menu.h"
#include "getch.h"

void show_var(void *p){
	printf(" p = [%p]%x\n", p, *(int*)p);
}

void increment(void *p){
	int *ip = (int*)p;
	*ip += 1;
}

void decrement(void *p){
	int *ip = (int*)p;
	*ip -= 1;
}

void colour_change(void *p){
	(void)p; // stop warnings
	static int c = black;
	if (c > white) { c = black; }
	menu_change_colour(c++);
}

void colour_reset(void *p){
	(void)p;
	menu_reset_colour();
}

void some_other_sub_menu(void *p){
	struct menu_options sub_menu[] = {
		{"go back", 		menu_quit 				},
		{"place holder", 	menu_not_implemented 	},
		{"Show other_var",	show_var 				},
		{"inc", 			increment 				}, 
		{"dec", 			decrement  				}
	};

	int other_var = 700;
	menu_loop(sub_menu, menu_length(sub_menu), "SUB MENU CAN USE A DIFFERENT VARIABLE", &other_var);
}
//-------------------------------
int main(const int argc, const char ** argv){

	struct menu_options menu[] = {
		{"Exit", 				menu_quit 			},
		{"Show some_variable",	show_var 			},
		{"inc", 				increment 			}, 
		{"dec", 				decrement  			},
		{"submenu",				some_other_sub_menu },
		{"change colour", 		colour_change      	}, 
		{"reset colour", 		colour_reset  		},
		{"clear screen", 		menu_cls			}
	};
	int some_variable = 0;

	menu_loop(menu, menu_length(menu), "MENU EXAMPLE", &some_variable);
	menu_reset_colour();
	return EXIT_SUCCESS;
}
