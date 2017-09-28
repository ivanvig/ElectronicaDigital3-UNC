/*
===============================================================================
 Name        : uart.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <cr_section_macros.h>



#define AddrPCONP		0x400FC0C4
#define AddrPINSEL0		0x4002C000
#define AddrU3LCR 		0x4000C008
#define AddrPINMODE0 	0x4002C000
#define AddrU3DLL 		0x4009C000
#define AddrU3DLM 		0x4009C004
#define AddrU3IER 		0x4009C004
#define AddrU3RBR  		0x4009C000
#define AddrU3THR  		0x4009C000
#define AddrISER0  		0xE000E100
#define AddrU3LSR 		0x4009C014

unsigned int volatile *const PCONP = (unsigned int*) AddrPCONP;
unsigned int volatile *const PINSEL0 = (unsigned int*) AddrPINSEL0;
unsigned int volatile *const U3LCR = (unsigned int*) AddrU3LCR;
unsigned int volatile *const PINMODE0 = (unsigned int*) AddrPINMODE0;
unsigned int volatile *const U3DLL = (unsigned int*) AddrU3DLL;
unsigned int volatile *const U3DLM = (unsigned int*) AddrU3DLM;
unsigned int volatile *const U3IER = (unsigned int*) AddrU3IER;
unsigned int volatile *const U3RBR = (unsigned int*) AddrU3RBR;
unsigned int volatile *const U3THR = (unsigned int*) AddrU3THR;
unsigned int volatile *const U3U3THR = (unsigned int*) AddrU3THR;
unsigned int volatile *const ISER0 = (unsigned int*) AddrISER0; //bit8 ISE_UART3 UART3 Interrupt Enable.
unsigned int volatile *const U3LSR = (unsigned int*) AddrU3LSR;

int main(void) {
init();
        while(1) {

    }
    return 0 ;
}



void init(void){
	*PCONP |= (1<<25);
	*U3LCR |= 11;
	*U3LCR |= (1<<7);
	*U3DLL = 0b10100001; //DLL= 161 ->9585 baudios
	*U3DLM =0;
	*PINSEL0=0b1010;
	*PINMODE0=0b1000;
	*U3LCR &=~(1<<7);
	*U3IER = 1;
	*ISER0 = (1<<8);
		}

void UART3_IRQHandler(void){
	uint8_t k;
	k=*U3RBR;
	while((*U3LSR&(1<<5))==0){}
	*U3THR=k;

}
