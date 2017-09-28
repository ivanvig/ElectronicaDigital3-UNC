#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);

#define AddrFIO0DIR 0x2009C000
#define AddrFIO0PIN 0x2009C014
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C

unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0PIN = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;

int main(void) {

	Init();
	int porc = 0;			*T0MR0 = 1;
	*T0MR1 = 1000;
	int k = 50;
	int x;
	while(1){
		*FIO0SET = (1 << 22);
		for(int i = 0; i < k*porc; i++);
		*FIO0CLR = (1 << 22);
		for(int i = 0; i < k*(100-porc); i++);
		if(!(*FIO0PIN & 1 << 1) && x > 10){
			porc = (porc + 25)%125;
			x = 0;
		}
		x++;
	}
    return 0 ;
}

void Init(){
	*FIO0DIR |= (1 << 22);
}
