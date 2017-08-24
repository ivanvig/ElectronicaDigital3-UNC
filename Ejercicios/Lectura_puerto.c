#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
void led2Init (void); // Set GPIO - P0_22 - to be output
// TODO: insert other definitions and declarations here
#define LED2 22

#define AddrFIO0DIR 0x2009C000
#define AddrFIO0SET 0x2009C018
#define AddrFIO0CLR 0x2009C01C
//Para leer el pin de control
#define AddrFIO1PIN 0x2009C034

unsigned int volatile *const FIO0DIR = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0SET = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const FIO1PIN = (unsigned int*) AddrFIO1PIN;


int main(void) {
    // TODO: insert code here
led2Init();
    // Force the counter to be placed into memory
    int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {

    	if(*FIO1PIN & 0x80000000){
			for(i=10000000;i>0;i--)
			{
			}
			*FIO0CLR = (1 << LED2);	//se apaga el led
			for(i=10000000;i>0;i--)
			{
			}
			*FIO0SET = (1 << LED2);	//se enciende el led
			i++;
    	}
    }
    return 0 ;
}

void led2Init (void) // Set GPIO - P0_22 - to be output
{
	*FIO0DIR |= (1 << LED2);
}
