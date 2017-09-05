#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);

#define AddrTalto 0x10002010
#define AddrTbajo 0x10002014
static int i = 0;

//Pins
#define AddrFIO0DIR 0x2009C000
#define AddrFIO0PIN 0x2009C014
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C
#define AddrPINSEL4 0x4002C010
//NVIC
#define AddrISER0   0xE000E100	//Habilitar las interrupciones
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
//ExtINT
#define AddrISER0   0xE000E100
#define AddrEXTMODE 0x400FC148
#define AddrEXTPOLAR 0x400FC14C
#define AddrEXTINT 0x400FC140

unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const PINSEL4 = (unsigned int*) AddrPINSEL4;
unsigned int volatile *const T0MR0 = (unsigned int*) AddrT0MR0;
unsigned int volatile *const T0MR1 = (unsigned int*) AddrT0MR1;
unsigned int volatile *const T0IR = (unsigned int*) AddrT0IR;
unsigned int volatile *const T0MCR = (unsigned int*) AddrT0MCR;
unsigned int volatile *const T0TCR = (unsigned int*) AddrT0TCR;
unsigned int volatile *const ISER0 = (unsigned int*) AddrISER0;
unsigned int volatile *const EXTMODE = (unsigned int*) AddrEXTMODE;
unsigned int volatile *const EXTPOLAR = (unsigned int*) AddrEXTPOLAR;
unsigned int volatile *const EXTINT = (unsigned int*) AddrEXTINT;


unsigned int volatile *const Talto = (unsigned int*) AddrTalto;
unsigned int volatile *const Tbajo = (unsigned int*) AddrTbajo;


void Init(void);

int main(void) {
	Init();
	*Talto = 25000000;
	*Tbajo = 50000000;

	while(1){};
    return 0;
}

void Init(){
	*FIO0DIR |= (1 << 22);		//LED2 como salida
	//EXTINT
	*PINSEL4 |= (1<<20);	//10 en posiciones 21 y 20 respectivamente para aceptar interrupciones externas
	*EXTMODE |= 1;			//0 - Por nivel / 1 - Por flanco
	*EXTINT  |= 1;
	*EXTPOLAR|= 1;			//0 - Por flanco de bajada / 1 - Por flanco de subida
	*EXTINT  |= 1;
	//Timer
 	*T0MR0 = *Tbajo;			//MR0 en 25 Mega
	*T0MR1 = *Talto;			//MR1 en 75 Mega
	*T0MCR = 0x00000000;		//Borro el registro entero porque no se resetea el HDMP cuando debuggeas un nuevo programa
 	*T0MCR |= (1<<0);			//Genera una interrupción cuando hay match entre MR0 y TC
 	*T0MCR |= (1<<3);			//Genera una interrupción cuando hay match entre MR1 y TC
	*T0MCR |= (1<<4);			//Resetea TC en el match con MR1
	*T0IR  |= (1<<0);			//Limpia la interrupcion de TMR0 MR0
	*T0IR  |= (1<<1);			//Limpia la interrupcion de TMR0 MR1
	*T0TCR |= 1;				//Cuando es 1, habilita el contador del Timer y del Prescaler
	//NVIC
	*ISER0 |= (1<<1);			//Habilita las interrupciones (el bit 1 es para timer 0)
	*ISER0 |= (1<<18);			//Habilitar EINT0 en NVIC
}

void TIMER0_IRQHandler (void){
	if (*T0IR & 1){				//INTRP por MR0
		*FIO0CLR = (1 << 22);	//Apagar LED
		*T0IR |= (1<<0);		//Bajar INTRP de MR0
	}else if (*T0IR & (1<<1)){
		*FIO0SET = (1 << 22);	//Prender LED
		*T0IR = (1<<1);			//Bajar INTRP de MR1
	}
}

void EINT0_IRQHandler(void){
	if(i%3){
		*T0MR0 = (*T0MR0>>1);
		*T0MR1 = (*T0MR1>>1);
	}else{
		*T0MR0 = *Tbajo;
		*T0MR1 = *Talto;
	}
	*EXTINT |= 1;			//Bajo la INTRP
}
