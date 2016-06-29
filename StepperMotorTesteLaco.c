/*
	Universidade Federal Rural de Pernambuco
	Unidade Acadêmica de Garanhuns
	Bacharelado em Ciência da Computação
	Projeto de Sistemas Embarcados - Motor de Passos
	
*/

#include "lpc2103.h"

int segundos = 0;

void delay(int count)
{
	int j = 0,i = 0;
	for(j = 0; j < count; j++)
	{
		for(i = 0; i < 200; i++);
	}
}

int is_pin_set(int pin)
{
	int pin_state = IOPIN & (1 << pin);
	return pin_state;
}

void step_one()
{
		IOCLR = 0x10000;	 // CLR P1.16
		IOSET = 0x80000;   // Set P1.19
}

void step_two()
{
		IOCLR = 0x80000;  //CLR P1.19
		IOSET = 0x40000;  //Set P1.18
}

void step_three()
{
		IOCLR = 0x40000;  //CLR P1.18
		IOSET = 0x20000;  //Set P1.17
}

void step_four()
{
		IOCLR = 0x20000;  //CLR P1.17
		IOSET = 0x10000;   //SET P1.16		
}


void main()
{
	IODIR = 0x000F0000;  //set direction from P1.16 to P1.19 - 45º, 135º, 180º, 225º
	IOCLR = 0x000F0000;
	
	while(1)
	{				
		step_one();
		delay(3000);
		step_two();
		delay(3000);
		step_three();
		delay(3000);
		step_four();
		delay(3000);
	}
}