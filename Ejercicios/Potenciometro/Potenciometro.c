
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
//Pins
#define AddrFIO0DIR 0x2009C000
#define AddrFIO0PIN 0x2009C014
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C
#define AddrPINSEL1 0x4002C004
unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const PINSEL1 = (unsigned int*) AddrPINSEL1;

//ADC
#define AddrPCLKSEL0	0x400FC1A8
#define AddrPCONP		0x400FC0C4
#define AddrAD0CR 		0x40034000
#define AddrAD0DR0 		0x40034010
#define AddrPINMODE1 	0x4002C044
#define AddrAD0INTEN  	0x4003400C
#define AddrAD0GDR 		0x40034004
unsigned int volatile *const PCLKSEL0 = (unsigned int*) AddrPCLKSEL0;
unsigned int volatile *const PCONP = (unsigned int*) AddrPCONP;
unsigned int volatile *const AD0CR = (unsigned int*) AddrAD0CR;
unsigned int volatile *const AD0DR0 = (unsigned int*) AddrAD0DR0;
unsigned int volatile *const PINMODE1 = (unsigned int*) AddrPINMODE1;
unsigned int volatile *const AD0INTEN = (unsigned int*) AddrAD0INTEN;
unsigned int volatile *const AD0GDR = (unsigned int*) AddrAD0GDR;

void init(void);


int main(void) {
	init();

  static char read;

    while(1) {
    	read = 0x03 & (*AD0GDR >> 14); //leo los 4 bits mas significativos

    	if(read == 0){
    		*FIO0PIN=(1<<15);
    	}
    	else if(read == 1){
    		*FIO0PIN=(1<<16);
    	}
    	else if(read == 2){
    		*FIO0PIN=(1<<17);
    	}
    	else if(read == 3){
    		*FIO0PIN=(1<<18);
    	}
    }
    return 0 ;
}

void init(void){
	*FIO0DIR|=(1<<18);  //pines 15,16,17 y 18 del puerto 0 como salida.
	*FIO0DIR|=(1<<17);
	*FIO0DIR|=(1<<16);
	*FIO0DIR|=(1<<15);
	*PCONP|= (1<<12);   //power
	*PCLKSEL0|= (3<<24); //selecciona pclock_adc
	*PINSEL1|= (1<<14); // P0.23 = 01  selecciona AD0.0
	*AD0CR|= (1<<0);    //selecciona el canal AD0.0
	*AD0CR|= (1<<21);	//habilita el ADC
	//*PINMODE1|= (2<<14); //inhabilita resistencias pull up y pull down
	*AD0CR|= (1<<16);	//habilita modo burst
	*AD0INTEN&=~(1<<8); //si burst=1, AD0INTEN debe estar en 0.
}
