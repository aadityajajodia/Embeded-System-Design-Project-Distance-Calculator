//ESD mini project to calculate distance
#include<lpc17xx.h>
#include<stdio.h>
#include<stdlib.h>
void lcd_init();//function to initialize lcd
void lcd_cmd_wrt(int);//function for commands
void lcd_data_wrt(int );//function for data
void delay();//delay
void lcd_string(char *);//function to write char array
void clear_ports();//for clearing ports
char dismessage[]={"DISTANCE in cm"};
char message1[]={"abcdefgh"};
char welcome[]={"Welcome RAO SIR"};
unsigned int x,i=0,j=0,flag=1;
double value,distance;
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL1=0x0;
	LPC_GPIO0->FIODIR=0x3F<<23;
	lcd_init();
	
	
	// External Interrupt at P2.11 (for ECHO ) and GPIO at P2.10 (for TRIGGER)
	LPC_PINCON->PINSEL4=1<<22; // Configuring P2.11 as Fun1 and P2.10 as GPIO
	LPC_GPIO2->FIODIR=0x1<<10;
	
	
	//External Interrupt Configuration
	 //LPC_SC->EXTINT
	 LPC_SC->EXTMODE=0x2;
	 LPC_SC->EXTPOLAR=0x02;
	
	//Timer configuration
	LPC_TIM0->TCR=0x02;
	LPC_TIM0->CTCR=0x00;
	LPC_TIM0->MCR=0x1;
	LPC_TIM0->PR=0x2;
	lcd_cmd_wrt(0x01);
	lcd_cmd_wrt(0x80);
	lcd_string(welcome);
	for(i=0;i<800000;i++);
	
	lcd_cmd_wrt(0x01);
	lcd_cmd_wrt(0x80);
	lcd_string(dismessage);
	delay();
	lcd_cmd_wrt(0xC0);
	 
	
	while(1)
	{
		
		
	LPC_GPIO2->FIOPIN=0;
	for(i=0;i<10000;i++);
	LPC_GPIO2->FIOPIN=0x1<<10;
	for(i=0;i<25000;i++);
	LPC_GPIO2->FIOPIN=0;
	
	NVIC_EnableIRQ(EINT1_IRQn);
	while(flag!=3); // Waiting for the interrupt
	
	delay();
	//value = LPC_TIM0->TC;
	//distance = (value/1000000.0)*340;
	distance=value;
	sprintf(message1,"%3.2f",distance);
	lcd_cmd_wrt(0xC0);
	lcd_string("       ");
	lcd_cmd_wrt(0xC0);
	lcd_string(message1);
	flag=1;
	for(i=0;i<25000;i++);
	}
	
}

void EINT1_IRQHandler(void)
{
	
	
	LPC_SC->EXTPOLAR=~(LPC_SC->EXTPOLAR);
	
	if(flag==1)
	{

	  flag++;
          LPC_TIM0->TCR=0x1;
	  
	}
        else if(flag==2)
	{
		
		LPC_TIM0->TCR=0x00;
		flag++;
		value=LPC_TIM0->TC/58.0;
		LPC_TIM0->TCR=0x02;
		//LPC_SC->EXTMODE=0x2;
		
        }
				
				
	LPC_SC->EXTINT=0xFF;
	
}


void lcd_string(char *buff)
{
	i=0;
	
	while(buff[i]!='\0')
	{
		//lcd_data_wrt(0x34);
		lcd_data_wrt(buff[i]);
		delay();
		i++;
	}

}

void lcd_init()
{
	for(i=0;i<3;i++)
	{
		LPC_GPIO0->FIOCLR=0x1<<27;
		LPC_GPIO0->FIOPIN=0x03<<23;
		for(j=0;j<30000;j++);
		LPC_GPIO0->FIOSET=0x1<<28;
		for(j=0;j<100;j++);
		LPC_GPIO0->FIOCLR=0x1<<28;
		for(j=0;j<30000;j++);

	}
	LPC_GPIO0->FIOPIN=0x2<<23;
	LPC_GPIO0->FIOSET=0x1<<28;
	for(j=0;j<100;j++);
	LPC_GPIO0->FIOCLR=0x1<<28;
	delay();
	lcd_cmd_wrt(0x28);
	delay();
	lcd_cmd_wrt(0x06);
	delay();
	lcd_cmd_wrt(0x0E);
	delay();
	lcd_cmd_wrt(0x80);
	delay();
}
void lcd_cmd_wrt(int y)
{
	clear_ports();
	LPC_GPIO0->FIOCLR=0x1<<27;
	x=y&0xF0;
	LPC_GPIO0->FIOPIN=x<<19;
	LPC_GPIO0->FIOSET=0x1<<28;
	for(j=0;j<100;j++);
	LPC_GPIO0->FIOCLR=0x1<<28;
	delay();
	x=y&0x0F;
	LPC_GPIO0->FIOPIN=x<<23;
	LPC_GPIO0->FIOSET=0x1<<28;
	for(j=0;j<100;j++);
	LPC_GPIO0->FIOCLR=0x1<<28;
	delay();

}
void lcd_data_wrt(int y)
{
	clear_ports();
	LPC_GPIO0->FIOSET=0x1<<27;
	x=y&0xF0;
	LPC_GPIO0->FIOPIN|=x<<19;
	LPC_GPIO0->FIOSET=0x1<<28;
	for(j=0;j<100;j++);
	LPC_GPIO0->FIOCLR=0x1<<28;
	delay();
	clear_ports();
	LPC_GPIO0->FIOSET=0x1<<27;
	x=y&0x0F;
	LPC_GPIO0->FIOPIN|=x<<23;
	LPC_GPIO0->FIOSET=0x1<<28;
	for(j=0;j<100;j++);
	LPC_GPIO0->FIOCLR=0x1<<28;
	delay();
}
void delay()
{
	for(j=0;j<800;j++);
}
void clear_ports()
{
	LPC_GPIO0->FIOPIN=0x0;
}