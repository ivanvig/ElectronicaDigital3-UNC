#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void Init(void);

#define AddrFIO2DIR 0x2009C040
#define AddrFIO2PIN 0x2009C054 //Definido como salida en la funcion Init()
#define AddrFIO2SET 0x2009C058
#define AddrFIO2CLR 0x2009C05C

unsigned int volatile *const FIO2DIR = (unsigned int*) AddrFIO2DIR;
unsigned int volatile *const FIO2PIN = (unsigned int*) AddrFIO2PIN;
unsigned int volatile *const FIO2SET = (unsigned int*) AddrFIO2SET;
unsigned int volatile *const FIO2CLR = (unsigned int*) AddrFIO2CLR;


int main(void){
	Init();

unsigned int tabla[]= {
					0x000000c0,		//0 - 11000000
			  	  	0x000000f9,		//1 - 11111001
					0x000000a4,		//2 - 10100100
					0x000000b0,		//3 - 10110000
					0x00000099,		//4 - 10011001
					0x00000012,		//5 - 00010010
					0x00000082,		//6 - 10000010
					0x000000f8,		//7 - 11111000
					0x00000080,		//8 - 10000000
					0x00000090};	//9 - 10010000


	unsigned int num=90;		//Variable a mostrar
	unsigned int display1=~tabla[0];	//LS Display
	unsigned int display2=~tabla[0];	//MS Display


	display1=~tabla[num%10];
	display2=~tabla[(num/10)%10];

	while(1){
		*FIO2PIN = display1;
		*FIO2CLR = (1<<8);
	for(unsigned int i=0;i<1000;i++);
		*FIO2PIN = display2;
		*FIO2CLR = (1<<10);
	for(unsigned int i=0;i<1000;i++);
	}
}

void Init (void)
{
	*FIO2DIR = 0x000005ff; 		//Setea GPIO - P0_0:P0_7 como salidas del display y PO_8//PO_9 para MPX
}
