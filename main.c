#include "lpc2103.h"

#define TRIGGER 1 << 19
#define PIN_14 1 << 14
#define PIN_22 1 << 22
#define PIN_23 1 << 23

//UART
#define TEMT 1 << 6


long unsigned contador = 0;

long unsigned aux_contador;
int distancia;


void extintfalling_isr() __irq //INTERRUPÇÃO
{
	int i = 0;
	if(EXTPOLAR == 0x02){  //VERIFICAR SE EH FALLING EDGE
		
		T0TCR = 0x02; //RESET TIMER
		T0TCR = 0x01; //ENABLE TIMER
		
		aux_contador = 0;
		
		IOSET = PIN_22;
		IOCLR = PIN_23;
		
		VICIntEnClr = 1 << 15;
		EXTPOLAR = 0x00;
		
	}else if(EXTPOLAR == 0x00){ //VERIFICAR RASING EDGE
		
		aux_contador = T0TC;
		distancia = (int)(((double)aux_contador/58)/15);
			//*start_measurement = distancia;
			i = 400;
			//for(i = 0; i < 8; i++){
				U0THR = distancia;
				while(!(U0LSR & TEMT)){}
	//		}
		
		T0TCR = 0x02; //RESET TIMER
		T0TCR = 0x00; //DISABLE TIMER
				
		IOSET = PIN_23;
		IOCLR = PIN_22;
		
		
		VICIntEnClr = 1 << 15;
		EXTPOLAR = 0x02;
			
	}
	
	EXTINT = 0x02; //LIMPA INTERRUPÇÃO DO EXINT1
	VICIntEnable = 1<<15;
	VICVectAddr = 0x00;
	
}

void extint_falling_init(){

	EXTMODE = 0x02; //EINT1
	EXTPOLAR = 0x00; //EINT1 - falling EDGE
	
	VICVectCntl0 = 1 << 5 | 0x0F;
	VICVectAddr0 = (unsigned long) extintfalling_isr;
	VICIntEnable = 1<<15;

}

void timer0_init(){
	
	T0CTCR = 0x00;
	T0PR = 0x0F;
	T0TCR = 0x02;
	
}

int main(){
	
	int count = 0;
	
	//uart
	char *start_measurement = "S\x0D\x0A";
	int i = 0;
	
	IODIR = TRIGGER | PIN_23 | PIN_22; //EDEREÇO 
	
	PINSEL0 = 0x10000000;
	
	IOSET = PIN_23;
	
	extint_falling_init();
	
	//UART
	PINSEL0 |= 0x00000005;
	
	//CONFIGURACAO UART
	U0FCR = 0x00; //Ativar ou desativar fila
	U0LCR = 0x83; //usar 8 bits
	U0DLM = 0x00; //9600 bps
	U0DLL = 0x61; //9600 bps
	U0LCR = 0x03;
	U0TER = 1 << 7;
	
	
	while(1){	
		
		if (count == (10500000)){
			IOSET = TRIGGER;
		}else if(count > (10550000)){
			count=0;
			IOCLR = TRIGGER;
		}
		count++;
						
	}
	 
		
	

 return 0;

}
