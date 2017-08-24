#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);

#define AddrFIO0DIR 0x2009C000
//#define AddrFIO1DIR 0x2009C020

#define AddrFIO0PIN 0x2009C014 //Definido como salida en la funcion Init()
#define AddrFIO2PIN 0x2009C054 //Definido como entrada por defecto


unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
//unsigned int volatile *const FIO1DIR = (unsigned int*) AddrFIO1DIR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO2PIN = (unsigned int*) AddrFIO2PIN;

int main(void){
	Init();
	while (1){
		if (*FIO2PIN & 0x00001000){//Se hace la suma cuando el P1_31 esta en alto
			*FIO0PIN = (0x0000000f & *FIO2PIN) + ((0x000000f0 & *FIO2PIN)>>4);
		}
	}
}
void Init (void)
{
	*FIO0DIR = 0x0000000f; // Setea GPIO - P0_0:P0_3 como salidas
}
