#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);
static unsigned int i = 1;
static unsigned int por = 0;
static unsigned int flag = 0;

#define offset 150
#define tiempo 5000
//Pins
#define AddrFIO0DIR 0x2009C000
#define AddrFIO0PIN 0x2009C014
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C
#define AddrPINSEL4 0x4002C010
//GpioInt
#define AddrIO0IntEnF		0x40028094
#define AddrIO0IntStatF 	0x40028088
#define AddrIO0IntClr		0x4002808C
#define AddrIO2IntEnF		0x400280B4
#define AddrIO2IntStatF 	0x400280A8
#define AddrIO2IntClr		0x400280AC
#define AddrIOIntStatus		0x40028080
//NVIC
#define AddrISER0   0xE000E100	//Habilitar las interrupciones
//Timer
#define AddrT0TC 	0x40004008
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
unsigned int volatile *const IO0IntEnF = (unsigned int*) AddrIO0IntEnF;
unsigned int volatile *const IO0IntStatF = (unsigned int*) AddrIO0IntStatF;
unsigned int volatile *const IO0IntClr = (unsigned int*) AddrIO0IntClr;
unsigned int volatile *const IO2IntEnF = (unsigned int*) AddrIO2IntEnF;
unsigned int volatile *const IO2IntStatF = (unsigned int*) AddrIO2IntStatF;
unsigned int volatile *const IO2IntClr = (unsigned int*) AddrIO2IntClr;
unsigned int volatile *const IOIntStatus = (unsigned int*) AddrIOIntStatus;
unsigned int volatile *const T0TC = (unsigned int*) AddrT0TC;


void Init(void);

int main(void) {
	Init();

	while(1){
		while(flag){
			flag--;
			*EXTINT |= 1;
			*T0TCR |= 1;
		}
		*ISER0 |= (1<<18);			//INTRP por EINT0
		*ISER0 |= (1<<21);			//INTRP por EINT3 (GPIO)
	};
    return 0;
}

void Init(){
	*FIO0DIR |= (1 << 22);		//LED2 como salida
	//GPIO
	*IO0IntEnF |= 1;			//GPIO Interrupt por P0.0
	*IO2IntEnF |= 1;			//GPIO Interrupt por P2.0
	//EINT 0
	*PINSEL4 |= (1<<20);		//10 en posiciones 21 y 20 respectivamente para aceptar interrupciones externas
	*EXTMODE |= 1;				//0 - Por nivel / 1 - Por flanco
	*EXTINT  |= 1;				//Baja INTRP
	*EXTPOLAR|= 1;				//0 - Por flanco de bajada / 1 - Por flanco de subida
	*EXTINT  |= 1;				//Baja INTRP
	//Timer
	*T0MCR = 0x00000000;		//Borro el registro entero porque no se resetea el HDMP cuando debuggeas un nuevo programa
 	*T0MCR |= 1;				//Genera una interrupción cuando hay match entre MR0 y TC
 	*T0MCR |= (1<<3);			//Genera una interrupción cuando hay match entre MR1 y TC
	*T0MCR |= (1<<4);			//Resetea TC en el match con MR1
	*T0IR  |= 1;				//Limpia la interrupcion de TMR0 MR0
	*T0IR  |= (1<<1);			//Limpia la interrupcion de TMR0 MR1
	*T0MR1 = tiempo;			//Cargo MR0
	*T0TCR |= 1;				//Cuando es 1, habilita el contador del Timer y del Prescaler
	//NVIC
	*ISER0 |= (1<<1);			//Habilita las interrupciones (el bit 1 es para timer 0)
	*ISER0 |= (1<<18);			//INTRP por EINT0
	*ISER0 |= (1<<21);			//INTRP por EINT3 (GPIO)
}

void TIMER0_IRQHandler (void){
	if (*T0IR & 1){				//INTRP por MR0
		*FIO0SET = (1 << 22);	//Prender LED
		*T0IR |= 1;		//Bajar INTRP de MR0
	}else if (*T0IR & (1<<1)){
		*FIO0CLR = (1 << 22);	//Apagar LED
		*T0IR |= (1<<1);		//Bajar INTRP de MR1
	}
}

void EINT3_IRQHandler(void){
	*ISER0 &= ~(1<<21);			//INTRP por EINT3 (GPIO)
	flag = 1000000000;


	*T0TCR &= ~1;					//Deshabilitar timer
	*T0TCR |= (1<<1);				//Resetea timer
	//for(int i=0;i<2000000;i++);
	if(*IOIntStatus & 1){			//Interrupcion GPIO P0
		if(por<100){
			por+=25;
		}
		*IO0IntClr |= 1;			//Bajar flag INRP GPIO P0.0
	}else if(*IOIntStatus & (1<<2)){//Interrupcion GPIO P2
		if(por>0){
			por-=25;
		}
		*IO2IntClr |= 1;			//Bajar flag INRP GPIO P2.0
	}
	*T0MR0 = por*(*T0MR1/100)+offset;
	*T0TCR |= 1;
	*T0TCR &= ~(1<<1);
}

void EINT0_IRQHandler(void){
	*ISER0 &= ~(1<<18);			//INTRP por EINT0
	flag = 1000000000;


	*T0TCR &= ~1;					//Deshabilitar timer
	*T0TCR |= (1<<1);				//Resetea timer
	//for(int i=0;i<2000000;i++);
	if(*EXTINT & 1){
		if(i & 1){					//Parpadear
			*ISER0 &= ~(1<<21);		//Bajar INTRP por GPIO en ISER
			*T0MR0 = 25000000;
			*T0MR1 = 2*(*T0MR0);
			i=~i;
		}else{						//PWM
			*ISER0 |= (1<<21);		//habilitar INTRP de GPIO en ISER
			*T0MR0 = por*(*T0MR1/100)+offset;
			*T0MR1 = tiempo;
			i=~i;
		}
		*T0TCR |= 1;
		*T0TCR &= ~(1<<1);
		*EXTINT |= 1;
	}
}

