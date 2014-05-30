/*
 * pscheduler.c
 *
 *  Created on: May 28, 2014
 *      Author: tobias
 */


#include "pscheduler.h"
#include "ses_uart.h"
#include "ses_common.h"

struct context_t currentContext;

void taskA(void) {

	while(1) {
		fprintf(uartout, "A\n");
	};
}

void taskB(void) {

	while(1) {
		fprintf(uartout, "B\n");
	};
}

void taskC(void) {

	while(1) {
		fprintf(uartout, "C\n");
	};
}


void getStackpointer(void *pstack) {
	asm volatile (
		"in r0, __SP_L__ \n\t"
		"in r1, __SP_H__ \n\t"
		"st %a0+, r0     \n\t"
		"st %a0,  r1     \n\t"
		:
		: "e" (&pstack)
	);
}

void pscheduler_run( task_t *taskList, uint8_t numTasks) {

	/* Initialize the contexts */
	struct context_t contexts[numTasks];
	for(int i = 0; i < numTasks; i++) {
		contexts[i].task = taskList[i];

		/* setup context as linked list */
		if(i > 0) {
			contexts[i].next = contexts[i-1];
		}
		else{
			contexts[i].next = contexts[numTasks-1];
		}
	}


	/* Start with the first task */
	currentContext = contexts[0];


	/* Enter MAIN-LOOP */
	while(1) {
		// context switch is done in the ISR
	}

}




void timer2_init(void) {
	// Power on Timer 2
	PRR0 &=  ~(1 << PRTIM2);

	// Clear Timer on Compare Match (CTC) Mode
	TCCR2A |= (1 << WGM21);

	// Set Top Value
	OCR2A = 0xFA; // 250

	// Set the Prescaler to 64
	TCCR2B |= (1 << CS22);

	// Enable interrupts
	TIMSK2	|= (1 << OCIE2A);
	TIFR2	|= (1 << OCF2A);
}

ISR(TIMER2_COMPA_vect, ISR_NAKED) {

	/* Macro that explicitly saves the execution
	 * context. */
	portSAVE_CONTEXT();



	/* CONTEXT-SWITCH  */
	*currentContext = currentContext->next;


	/* Macro that explicitly restores the
	 * execution context */
	portRESTORE_CONTEXT();


	/* The return from interrupt call must also
	 * be explicitly added. */
	asm volatile ( "reti" );
}



task_t taskList[] = {taskA, taskB, taskC};

int main(void) {
	uart_init(5700);
	timer2_init();

	pscheduler_run(taskList, 3);
	return 0;
}
