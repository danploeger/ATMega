
/**This is a so-called "naked" interrupt and thus the compiler
 * creates no additional assembler code when entering/ leaving
 * the function. Note that a "reti" (return from interrupt)
 * becomes necessary.
 *
 */
ISR(TIMER1_COMPA_vect, ISR_NAKED) {
	asm volatile (
			"reti \n\t"
	);

}

//------------------------------

/**In preemptive multi-tasking each
 * task needs an own stack.
 */
#define THREAD_STACK_SIZE	256


//------------------------------


/**The state of a task must be stored in
 * some struct
 */
struct context_t {
	uint8_t stack[THREAD_STACK_SIZE];
	void *pstack;
	struct context_t *next;
};

//------------------------------

/**Push things on the stack,
 * and then back immediately.
 */
asm volatile (
		"push r0 \n\t"
		"push r1 \n\t"
		"pop r1 \n\t"
		"pop r0 \n\t"
);


//------------------------------

/**Storing the current stack pointer
 * in a C variable.
 */
void *pstack;

asm volatile (
		"in r0, __SP_L__ \n\t"
		"in r1, __SP_H__ \n\t"
		"st  %a0+,  r0	\n\t"
		"st  %a0,  r1	\n\t"
		:
		: "e" (&pstack)
);

