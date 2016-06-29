/*

	Universidade Federal Rural de Pernambuco
	Unidade Acadêmica de Garanhuns
	Bacharelado em Ciência da Computação
	Projeto de Sistemas Embarcados - Motor de Passos
	
*/


#include "lpc2103.h"

int segundos = 0;
int angle = 0;

int is_pin_set(int pin)
{
	int pin_state = IOPIN & (1 << pin);
	return pin_state;
}

void step_one()
{
		IOCLR = 0x10000;	 // CLR P1.16
		IOSET = 0x80000;   // Set P1.19
		angle = 45;
		
}

void step_two()
{
		IOCLR = 0x80000;  //CLR P1.19
		IOSET = 0x40000;  //Set P1.18
		angle = 135;
		
}

void step_three()
{
		IOCLR = 0x40000;  //CLR P1.18
		IOSET = 0x20000;  //Set P1.17
		angle = 140;
}

void step_four()
{
		IOCLR = 0x20000;  //CLR P1.17
		IOSET = 0x10000;   //SET P1.16
		angle = 225;
		
}

int getCurrentAngle()
{
		return angle;
}

void timer0_isr() __irq
{
	if(is_pin_set(19) && segundos == 2){
		step_one();
		segundos = 0;
	}else if(is_pin_set(18) && segundos == 2){
		step_two();
		segundos = 0;
	}else if(is_pin_set(17) && segundos == 2){
		step_three();
		segundos = 0;
	}else if(is_pin_set(16) && segundos == 2){
		step_four();
		segundos = 0;
	}
	segundos++;
	
	T0IR = 0x00000001;
	VICVectAddr = 0x00000000;
}

void timer0_init()
{
	T0CTCR = 0x00000000; 
	T0PR = 0x000003E8; 
	T0MCR = 0x00000003;
	
	T0MR0 = 0x00004E20;
		
	VICVectCntl0 = 1 << 5 | 0x04; 
	VICVectAddr0 = (unsigned long) timer0_isr;
	VICIntEnable = 1 << 4; 
	
	T0TCR = 1;
}

void main()
{
	IODIR = 0x000F0000;  //set direction from P1.16 to P1.19 - 45º, 135º, 180º, 225º
	IOCLR = 0x000F0000;
	IOSET = 0x80000;
	timer0_init();
	
	while(1);
	
}
