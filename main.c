#include "lpc2103.h"
//#define PIN_19 1 << 19
#define PIN_14 1 << 14
#define PIN_16 1 << 16
#define TRIGGER 1 << 19

long unsigned contador;
int aux_mr; //saber qual eh a interrupcao do mr1

void extintrising_isr() __irq
{
	int unsigned aux = (int unsigned) EXTPOLAR; //Para saber se a interrupcao foi gerada na subida ou na descida 
	
	if(aux == 0x00000010){
		T0TCR = 0x02; //Reset Time
		T0TCR = 0x01; //Enable time
		VICIntEnClr = 1 << 15; 
		EXTPOLAR = 0x00000000;
	}else if(aux == 0x00000000){
		contador = (long unsigned) T0TCR;
		T0TCR = 0x02; //Reset Time
		T0TCR = 0x00; //Disable time
		VICIntEnClr = 1 << 15; 
		EXTPOLAR = 0x00000010;
	}
	
	EXTINT = 0x00000001; //limpa a interrupcao gerada pelo EXINT0
	VICIntEnable = 1 << 15;
	VICVectAddr = 0x00000000;
}
/*
void extintfalling_isr() __irq
{
	
	contador = (long unsigned) T0TCR;
	T0TCR = 0x02; //Reset Time
	T0TCR = 0x00; //Disable time
	
		VICIntEnClr = 1 << 15; 
		EXTPOLAR = 0x00;
		EXTINT = 0x00000001; //limpa a interrupcao gerada pelo EXINT0
		VICIntEnable = 1 << 15;
		VICVectAddr = 0x00000000;
}
*/

/** De 1 em 1 segundo seta o trigger e um curto periodo depois da um clear no trigger **/
void timer1_isr() __irq 
{
	int aux = T1IR;
	
	if(aux == 1){ //quando mr0 gera a interrupcao
		if(aux_mr == 1){ //verifica se a interrupcao anterior foi do mr1
			aux_mr = 0;
			IOCLR = TRIGGER;
		}
	}else if(aux == 2){ //quando mr1 gera interrupcao
		aux_mr = 1;
		IOSET = TRIGGER;
	}
	
	T1IR = aux;
	VICVectAddr = 0x00000000;

}

void extint_rising_init()
{
	EXTMODE = 1 << 1; //EINT1
	EXTPOLAR = 1 << 1; //EINT1 - rising edge
	VICVectCntl0 = 1 << 5 | 0x0F; //HABILITAR INTERRUPÇÃO E USAR A INTERRUPCAO EXINT1 (MAIOR PRIORIDADE)
	VICVectAddr0 = (unsigned long) extintrising_isr; //ENDEREÇO DA ROTINA DE TRATAMENTO
	VICIntEnable = 1 << 15; //ATIVANDO INTERRUPCAO EXINT1
}
/*
void extint_falling_init()
{
	EXTMODE = 1 << 1; //EINT1
	EXTPOLAR = 0x00; //EINT1 - falling edge
	VICVectCntl0 = 1 << 5 | 0x0F; //HABILITAR INTERRUPÇÃO E USAR A INTERRUPCAO EXINT1 (MAIOR PRIORIDADE)
	VICVectAddr0 = (unsigned long) extintfalling_isr; //ENDEREÇO DA ROTINA DE TRATAMENTO
	VICIntEnable = 1 << 15; //ATIVANDO INTERRUPCAO EXINT1
}
*/

void timer0_init()
{
	T0CTCR = 0x0;
	T0PR = 0x00003A98; // 
	T0TCR = 0x02; //Reset Timer
}

void timer1_init()
{
	
	T1CTCR = 0x00000000; //MODE TIMER
	T1PR = 0x00003A98; // 15000
	T1MCR =  (1 << 4) | (1 << 3) | 1;//0x00000003; //MR
	
	T1MR0 = 0x00000009; //
	T1MR1 = 0x000007D0; //2000
	
	VICVectCntl1 = 1 << 5 | 0x05; //HABILITAR INTERRUPÇÃO E USAR TIMER1 = 0x05
	VICVectAddr1 = (unsigned long) timer1_isr; //ENDEREÇO INTERRUPÇÃO
	VICIntEnable = 1 << 5; //ENABLE TIMER1
	
	T1TCR = 1;

	
}

int main(){

	IODIR = TRIGGER; //ENDEREÇO DAS PIN -- 19
	PINSEL0 = 0x10000000; //modificando o pin 12 de GIOP para modo EXINT1
	aux_mr = 0;
	
	timer0_init();
	timer1_init();
//	extint_rising_init(); //configurando interrupcao externa na subida edge
//	extint_falling_init(); //configurando interrupcao externa na descida edge
	
	
	while(1){} 
		
	return 0;
	
}
