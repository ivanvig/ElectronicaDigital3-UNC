#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <cr_section_macros.h>

//putty.exe -serial <puerto> -sercfg 9600,8,n,1,N

#define AddrPCONP		0x400FC0C4	//
#define AddrPINSEL0		0x4002C000	//
#define AddrU3LCR 		0x4009C00C	//
#define AddrPINMODE0 	0x4002C040
#define AddrU3DLL 		0x4009C000	//
#define AddrU3DLM 		0x4009C004	//
#define AddrU3IER 		0x4009C004	//
#define AddrU3RBR  		0x4009C000	//
#define AddrU3THR  		0x4009C000	//
#define AddrISER0  		0xE000E100
#define AddrU3LSR 		0x4009C014	//

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

void init(void);

int main(void) {
init();
        while(1) {
    }
    return 0 ;
}



void init(void){
	//*PCONP |= (1<<3);		//Habilitar UART0
	*PCONP |= (1<<25);		//Prender UART3
	*U3LCR |= 11;			//Palabra de 8 bits
	*U3LCR |= (1<<7);		//Activar DLAB (Enable access to division latches para config de BR)
	*U3DLL |= 0xA1; 		//DLL= 161 ->9585 baudios
	*U3DLM |= 0;			//DLM=0
	*U3LCR &=~(1<<7);		//Habilita la interrupcion
	*PINSEL0 |= 0xA;		//p0.0 TX y p0.1 RX
	*PINMODE0 |= 0x8;		//Dejar al aire RX
	*U3IER |= 0x1;			//Setea que se produzca una interrupcion
	*ISER0 |= (1<<8);		//Habilita la interrupcion en NVIC
}

void UART3_IRQHandler(void){
	char k=*U3RBR;
	while((*U3LSR&(1<<5))==0){}		//Esta transmitiendo, entonces esperar
	*U3THR=k;

}
