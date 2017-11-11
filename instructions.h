#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include "device.h"

#define INSTRUCTION_ARGS struct device_settings* const p_chip, const int op1, const int op2
/**
 * @brief      The instructions each have a name and what to do when it occurs.
 */
struct instruction{
	char* 	name;
	int 	num_operands;
	void 	(*perform)(INSTRUCTION_ARGS);
};

/**
 * @brief      Test function only will print the values of the operands to the screen
 *
 * @param[in]  op1   The operand 1
 * @param[in]  op2   The operand 2
 */
void instruction_test(INSTRUCTION_ARGS);
void instruction_nop(INSTRUCTION_ARGS);
void instruction_movf(INSTRUCTION_ARGS);
void instruction_movwf(INSTRUCTION_ARGS);
void instruction_movlw(INSTRUCTION_ARGS);


#endif