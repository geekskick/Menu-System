#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

/**
 * @brief      The instructions each have a name and what to do when it occurs.
 */
struct instruction{
	char* 	name;
	int 	num_operands;
	void 	(*perform)(const int operand1, const int operand2);
};

/**
 * @brief      Test function only will print the values of the operands to the screen
 *
 * @param[in]  op1   The operand 1
 * @param[in]  op2   The operand 2
 */
void instruction_test_perform(const int op1, const int op2);


#endif