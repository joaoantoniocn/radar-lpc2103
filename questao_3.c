#include <lpc2103.h>

/**
Questao 3 - Lista 1
led_1 = 25
led_2 = 26

led_3 = 5
led_4 = 4

**/

int state = 1;
int delay = 0;

int led_1 = 0x02000000; //endereco led_1
int led_2 = 0x04000000; //endereco led_2

int led_3 = 0x00000020; //endereco led_3
int led_4 = 0x00000010; //endereco led_4
	
void estados() 
{
	switch (state) 
	{
	case 1:
		
		
		if (delay >= 1500000)
		{
			delay = 0;	
			state = 2;
		}else
			delay++;
		break;
		
	case 2:		

		break;
		
	case 3:

		
		break;
		
	default:
		state = 1;
	
	}

}


int main()
{
	IODIR = 0x06000030; 
	IOCLR = 0x06000030; 

	//for(;;)
	//	estados();
	return 0;
}