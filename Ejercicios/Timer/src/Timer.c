#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);

#define AddrFIO0DIR 0x2009C000
#define AddrFIO0PIN 0x2009C014
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C
#define AddrT0MR0   0x40004018
#define AddrT0MR1   0x4000401C
#define AddrT0IR    0x40004000
#define AddrT0MCR   0x40004014
#define AddrT0TCR   0x40004004
#define AddrISER0   0xE000E100

unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const MR0 = (unsigned int*) AddrT0MR0;
unsigned int volatile *const MR1 = (unsigned int*) AddrT0MR1;
unsigned int volatile *const IR = (unsigned int*) AddrT0IR;
unsigned int volatile *const MCR = (unsigned int*) AddrT0MCR;
unsigned int volatile *const TCR = (unsigned int*) AddrT0TCR;
unsigned int volatile *const ISER0 = (unsigned int*) AddrISER0;

void Init(void);
void TIMER0_IRQHandler (void);

int main(void) {

	Init();

	while(1){

	}
    return 0 ;
}

void Init(){
	*FIO0DIR |= (1 << 22);
 	*MR0 = 25000000;
//	*MR1 = 50000000;
 	*MCR= 0;
 	*MCR= 1;
	*MCR |=1<<1;
	*IR =1;
	*ISER0= 1<<1;
	*TCR=1;
	}

void TIMER0_IRQHandler (void)
 {

	*IR =1;
	if (*FIO0PIN&(1<<22)){

   	   *FIO0CLR = (1 << 22);

    	   }
	else{
		   *FIO0SET = (1 << 22);
          	   }

   }
