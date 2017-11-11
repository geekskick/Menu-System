#include <stdio.h>
#include "instructions.h"
//----------------------


/**
 * @brief      To be used as a place holder. Prints the operand values
 *
 * @param[in]  op1   The operand 1
 * @param[in]  op2   The operand 2
 */
void instruction_test(INSTRUCTION_ARGS){
	(void)p_chip;
	printf("op1 = %d\top2 = %d\n", op1, op2);
}

void instruction_nop(INSTRUCTION_ARGS){

}

void instruction_movwf(INSTRUCTION_ARGS){
	p_chip->registers[op1] = p_chip->w_reg;
}

void instruction_movf(INSTRUCTION_ARGS){
	p_chip->registers[op2] = p_chip->registers[op1];
}

void instruction_movlw(INSTRUCTION_ARGS){
	p_chip->w_reg = op1;
}

