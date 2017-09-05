#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);

//Pins
#define AddrFIO0DIR 0x2009C000
#define AddrFIO0PIN 0x2009C014
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C

#define AddrISER0   0xE000E100	//Habilita las interrupciones por Timer0
//Timer
#define AddrT0MR0   0x40004018	//Match Register 0. Se habilita vía MCR para resetear el TC, parar el TC y PC y/o generar una INTRP cada vez que MR0=TC
#define AddrT0MR1   0x4000401C	//Match Register 1. Ver MR0.
#define AddrT0IR    0x40004000	//Interrupt Register. Se escribe para limpiar INTRP. Se puede leer para identificar INTRP pendientes (8 posibles)
#define AddrT0MCR   0x40004014	//Match Control Register. Controla si se genera una interrupcion y si se resetea el TC cuando ocurre un match.
#define AddrT0TCR   0x40004004	//Timer Control Register. Controla la funcion del TC. El TC se puede deshabilitar o resetear por medio del TCR.
#define AddrT0CCR	0x40004028	//Capture Control Register. Controla que flancos de input de captura cargan los Capture Registers y si se genera una INTRP.
#define AddrT0CR0	0x4000402C	//Capture Register 0. CR0 se carga con el valor de TC cuando ocurre un evento en CAPn.0 (CAP0.0 o CAP1.0 respectivamente).
#define AddrT0EMR	0x4000403C	//External Match Register. Controla los pines externos de match MATn.0-3.
#define T0CTCR		0x40004070	//Count Control Register. Selecciona entre modos Timer y Counter y en modo Counter selecciona la señal y los flancos.


unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const T0MR0 = (unsigned int*) AddrT0MR0;
unsigned int volatile *const T0MR1 = (unsigned int*) AddrT0MR1;
unsigned int volatile *const T0IR = (unsigned int*) AddrT0IR;
unsigned int volatile *const T0MCR = (unsigned int*) AddrT0MCR;
unsigned int volatile *const T0TCR = (unsigned int*) AddrT0TCR;
unsigned int volatile *const ISER0 = (unsigned int*) AddrISER0;

void Init(void);
void TIMER0_IRQHandler (void);

int main(void) {
	Init();

	while(1){};
    return 0;
}

void Init(){
	*FIO0DIR |= (1 << 22);		//LED2 como salida
 	*T0MR0 = 25000000;			//MR0 en 25 Mega
	*T0MR1 = 75000000;			//MR1 en 75 Mega
	*T0MCR = 0x00000000;		//Borro el registro entero
 	*T0MCR |= (1<<0);			//Genera una interrupción cuando hay match entre MR0 y TC
 	*T0MCR |= (1<<3);			//Genera una interrupción cuando hay match entre MR1 y TC
	*T0MCR |= (1<<4);			//Resetea TC en el match con MR1
	*T0IR  |= (1<<0);			//Limpia la interrupcion de TMR0 MR0
	*T0IR  |= (1<<1);			//Limpia la interrupcion de TMR0 MR1
	*ISER0 |= (1<<1);			//Habilita las interrupciones (el bit 1 es para timer 0)
	*T0TCR |= 1;				//Cuando es 1, habilita el contador del Timer y del Prescaler
}

void TIMER0_IRQHandler (void){
	if (*T0IR & 1){				//INTRP por MR0
		//*FIO0CLR = (1 << 22);	//Apagar LED
		*T0IR |= (1<<0);				//Bajar INTRP de MR0
	}else if (*T0IR & (1<<1)){
		*FIO0SET = (1 << 22);	//Prender LED
		*T0IR = (1<<1);			//Bajar INTRP de MR1
	}
}
