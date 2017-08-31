/*EXTINT External Interrupt Flag Register R/W 0 0x400F C140
EXTMODE External Interrupt Mode register R/W 0 0x400F C148
EXTPOLAR External Interrupt Polarity Register R/W 0 0x400F C14C
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#define AddrFIO0DIR 0x2009C000
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C
#define AddrFIO0PIN 0x2009C014
#define AddrPINSEL4 0x4002C010

#define AddrISER0   0xE000E100
#define AddrEXTMODE 0x400FC148
#define AddrEXTPOLAR 0x400FC14C
#define AddrEXTINT 0x400FC140

unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const PINSEL4 = (unsigned int*) AddrPINSEL4;
unsigned int volatile *const ISER0 = (unsigned int*) AddrISER0;
unsigned int volatile *const EXTMODE = (unsigned int*) AddrEXTMODE;
unsigned int volatile *const EXTPOLAR = (unsigned int*) AddrEXTPOLAR;
unsigned int volatile *const EXTINT = (unsigned int*) AddrEXTINT;


void Init();

int main(void) {

	Init();
    while(1) {
    }
    return 0 ;
}

void Init(){
	*PINSEL4 |= (1<<20);	//10 en posiciones 21 y 20 respectivamente para aceptar interrupciones externas
	*FIO0DIR |= (1<<22);	//LED 2 como salida
	*EXTMODE |= 1;			//0 - Por nivel / 1 - Por flanco
	*EXTINT |= 1;
	*EXTPOLAR |= 1;			//0 - Por flanco de bajada / 1 - Por flanco de subida
	*EXTINT |= 1;
	*ISER0 |= (1<<18);		//Habilitar EINT0 en NVIC
}

void EINT0_IRQHandler(void){
	for(int i=0;i<1000000;i++);
	if(*FIO0PIN & (1<<22)){
		*FIO0CLR = (1<<22);
	}else{
		*FIO0SET = (1<<22);
	}
	*EXTINT |= 1;			//Bajo la INTRP

}
