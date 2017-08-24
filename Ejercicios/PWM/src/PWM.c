/*
===============================================================================
 Name        : PWM.c
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

void Init(void);

#define AddrFIO2DIR 0x2009C040
#define AddrFIO2PIN 0x2009C054
#define AddrFIO2SET 0x2009C058
#define AddrFIO2CLR 0x2009C05C

unsigned int volatile *const FIO2DIR = (unsigned int*) AddrFIO2DIR;
unsigned int volatile *const FIO2PIN = (unsigned int*) AddrFIO2PIN;
unsigned int volatile *const FIO2SET = (unsigned int*) AddrFIO2SET;
unsigned int volatile *const FIO2CLR = (unsigned int*) AddrFIO2CLR;


int main(void) {

	int porc = 0;
	int k = 100;
	Init();

	while(1){
		*FIO2SET = 1;
		for(int i = 0; i < k*porc; i++);
		*FIO2CLR = 1;
		for(int i = 0; i < k*(1-porc); i++);
		if(!(*FIO2PIN & 1 << 1)){
			porc = (porc + 25)%125;
		}
	}

    return 0 ;
}

void Init(){
	*FIO2DIR = 1;
}
