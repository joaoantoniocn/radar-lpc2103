#include "lpc2103.h"

//#define PIN_19 1 << 19
#define PIN_14 1 << 14
#define PIN_16 1 << 16
#define TRIGGER 1 << 19
#define PIN_17 1 << 17
#define PIN_22 1 << 22
#define PIN_23 1 << 23

long unsigned contador = 0;
//int aux_mr; //saber qual eh a interrupcao do mr1
long unsigned aux_contador;
int distancia;

void extintfalling_isr() __irq
{
	if(EXTPOLAR == 0x02){	// verificando se eh falling edge
		T0TCR = 0x02; //Reset Time
		T0TCR = 0x01; //Enable time
		aux_contador = 0;
		IOSET = PIN_22;
		IOCLR = PIN_23;
		VICIntEnClr = 1 << 15;
		EXTPOLAR = 0x00;
	}else if(EXTPOLAR == 0x00){ //verificando se eh rising edge
		aux_contador = T0TC;
		T0TCR = 0x02; //Reset Time
		T0TCR = 0x00; //Disable time
		IOSET = PIN_23;
		IOCLR = PIN_22;
		VICIntEnClr = 1 << 15;
		EXTPOLAR = 0x02;
	}
	
	
	EXTINT = 0x00000002; //limpa a interrupcao gerada pelo EXINT1
	VICIntEnable = 1 << 15;
	VICVectAddr = 0x00000000;
	
}

/** De 1 em 1 segundo seta o trigger e um curto periodo depois da um clear no trigger 
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
**/
void extint_falling_init()
{
	EXTMODE = 0x02; //EINT1
	EXTPOLAR = 0x00; //EINT1 - falling edge
	VICVectCntl0 = 1 << 5 | 0x0F; //HABILITAR INTERRUPÇÃO E USAR A INTERRUPCAO EXINT1 (MAIOR PRIORIDADE)
	VICVectAddr0 = (unsigned long) extintfalling_isr; //ENDEREÇO DA ROTINA DE TRATAMENTO
	VICIntEnable = 1 << 15; //ATIVANDO INTERRUPCAO EXINT1; //ENABLE TIMER1
}

void timer0_init()
{
	T0CTCR = 0x0;
	T0PR = 0x0000000F; // 15 = 0x0000000F 15000 = 0x00003A98
	T0TCR = 0x02; //Reset Timer
}
/* //TRIGGER
void timer1_init()
{
	
	T1CTCR = 0x00000000; //MODE TIMER
	T1PR = 0x00003A98; // 15000
	T1MCR =  (1 << 4) | (1 << 3) | 1;//0x00000003; //MR
	
	T1MR0 = 0x00000009; //
	T1MR1 = 0x000007D0; //2000
	
	VICVectCntl1 = 1 << 5 | 0x05; //HABILITAR INTERRUPÇÃO E USAR TIMER1 = 0x05
	VICVectAddr1 = (unsigned long) timer1_isr; //ENDEREÇO INTERRUPÇÃO
	VICIntEnable = 1 << 5; //ATIVANDO INTERRUPCAO EXINT1; //ENABLE TIMER1
	T1TCR = 1;
}
*/


int main(){
	
	int count = 0;
	IODIR = TRIGGER | PIN_23 | PIN_22; //ENDEREÇO DAS PIN -- 19
	PINSEL0 = 0x10000000; //modificando o pin 12 de GIOP para modo EXINT1
//	aux_mr = 0;
	IOSET = PIN_23;
	
	//timer0_init();
	//timer1_init(); TRIGGER;
	extint_falling_init(); //configurando interrupcao externa na subida edge
	
	/*
	while(1)
	{
		if(count == (4500000)){
			IOSET = TRIGGER;
		}else if(count > (4550000)){
			IOCLR = TRIGGER;
			distancia = (int) (((double)aux_contador/58)/15);
			count = 0;
		}
		count++;
			
			
	}	 
		*/
		
		while(1)
	{
		if(count == (10500000)){
			IOSET = TRIGGER;
		}else if(count > (10550000)){
			IOCLR = TRIGGER;
			distancia = (int) (((double)aux_contador/58)/15);
			count = 0;
		}
		count++;
		
	}	 
	
	
	
	
	return 0;
	
}