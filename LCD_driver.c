/*
 * Name:	LCD_driver.c
 * Introduction: the driver codes for LCD NHD-C0216CZ-NSW-BBW-3V3
 *  - Created on: Dec 11, 2012
 *	- modification data: 31 Nov. 2012
 *	- author: Wayne Xu (a0219294)
 *	- version: v1.2
 *
 *	September 2014 -- Modified to work with the MSP430FR5969 and Launchpad
 *		 -- by Dr. C. L. Fullmer
 *
 */

//#include <msp430g2553.h>
//#include <ti/mcu/msp430/csl/CSL.h>
//#include "UART_TxRx.h"

#include <msp430fr5969.h>
#include <driverlib.h>
#include "LCD_driver.h"


/******************************************************************************
function: void LCD_init(void)
introduction: initialize LCD
parameters:
return value:
*******************************************************************************/
void LCD_init(void)
{
	LCD_delay_Nms(40);	// waiting LCD to power on.
	LCD_CS_HIGH;		//set CS high
	LCD_RS_HIGH;		//set RS high

	LCD_RST_LOW;     	//RESET
	LCD_delay_Nms(2);
	LCD_RST_HIGH;    	//end reset
	LCD_delay_Nms(20);

	LCD_writecom(0x30);	//wake up
	LCD_delay_Nms(2);
	LCD_writecom(0x30);	//wake up
	LCD_writecom(0x30);	//wake up

	LCD_writecom(0x39);	//function set
	LCD_writecom(0x14);	//internal osc frequency
	LCD_writecom(0x56);	//power control
	LCD_writecom(0x6D);	//follower control
	LCD_delay_Nms(10);

	LCD_writecom(0x70);	//contrast
	LCD_writecom(0x0C);	//display on
	LCD_writecom(0x01);	//clear
	LCD_delay_Nms(5);
	LCD_writecom(0x06);	//entry mode
	LCD_delay_Nms(10);

/*
	LCD_writecom(0x38);
	LCD_writecom(0x40);
	LCD_writedata(0x00);//address 0 = up/left arrow
	LCD_writedata(0x1E);
	LCD_writedata(0x18);
	LCD_writedata(0x14);
	LCD_writedata(0x12);
	LCD_writedata(0x01);
	LCD_writedata(0x00);
	LCD_writedata(0x00);
	LCD_writedata(0x00);//address 1 = down/left arrow
	LCD_writedata(0x00);
	LCD_writedata(0x01);
	LCD_writedata(0x12);
	LCD_writedata(0x14);
	LCD_writedata(0x18);
	LCD_writedata(0x1E);
	LCD_writedata(0x00);
	LCD_writedata(0x00);//address 2 = right arrow
	LCD_writedata(0x04);
	LCD_writedata(0x06);
	LCD_writedata(0x1F);
	LCD_writedata(0x06);
	LCD_writedata(0x04);
	LCD_writedata(0x00);
	LCD_writedata(0x00);
	LCD_writecom(0x39);
	LCD_delay_Nms(100);
*/
}

/******************************************************************************
function: void LCD_display_string(unsigned char L ,unsigned char *ptr)
introduction: display a string on singal line of LCD.
parameters:L is the display line, 0 indicates the first line, 1 indicates the second line
return value:
*******************************************************************************/
void LCD_display_string(unsigned char L, char *ptr)
{
//	unsigned char i;

	if(L==0)		//first line
	{
		LCD_writecom(0x80);
	}
	else if(L==1)	//second line
	{
		LCD_writecom(0xc0);
	}

	while (*ptr)
	{
		LCD_writedata(*ptr++);
	}

/*
	for(i=0;i<16;i++)
	{
		LCD_writedata(ptr[i]);
		//LCD_delay_Nms(100);
	}
*/
}

/******************************************************************************
function: void LCD_display_number(unsigned char H,unsigned char L, unsigned int num)
introduction: Display a four digit number on a position.
parameters: H( 0~1 ) indicates display line, H( 0~15) indicates display row, num is the number to display
return value:
*******************************************************************************/
void LCD_display_number(unsigned char H,unsigned char L,unsigned int num)
{
  unsigned char i,j,k,l;
  i = num/1000%10;
  j = num%1000/100;
  k = num%100/10;
  l = num%10;
  if(H==0)		//first line
  {
	  LCD_writecom(0x80+L);// row L
  }
  else if(H==1)	//second line
  {
	  LCD_writecom(0xc0+L);// row L
  }
  LCD_writedata(i+0x30);
  LCD_writedata(j+0x30);
  LCD_writedata(k+0x30);
  LCD_writedata(l+0x30);
}

void LCD_display_HEX(unsigned char H,unsigned char L,unsigned int num)
{
  char i;
  char data[4];

  data[0] = (num >> 12)& 0x0F;
  data[1] = (num >> 8) & 0x0F;
  data[2] = (num >> 4) & 0x0F;
  data[3] = num & 0x0F;

  if(H==0)		//first line
  {
	  LCD_writecom(0x80+L);// row L
  }
  else if(H==1)	//second line
  {
	  LCD_writecom(0xc0+L);// row L
  }

  for (i=0;i<4;i++)
  {
	  if(data[i] > 9)
		  LCD_writedata(0x0037+data[i]);
	  else
		  LCD_writedata(0x0030+data[i]);
  }
}

/******************************************************************************
function: void LCD_display_number(unsigned char H,unsigned char L, unsigned int num)
introduction: Display a four digit number on a position.
parameters: H( 0~1 ) indicates display line, H( 0~15) indicates display row, num is the number to display
return value:
*******************************************************************************/
void LCD_display_temp(unsigned char H,unsigned char L,unsigned int num)
{
  unsigned char i,j,k,l;
  i = num/1000%10;
  j = num%1000/100;
  k = num%100/10;
  l = num%10;
  if(H==0)		//first line
  {
	  LCD_writecom(0x80+L);// row L
  }
  else if(H==1)	//second line
  {
	  LCD_writecom(0xc0+L);// row L
  }
  if (i == 0)
	  LCD_writedata(0x20);
  else
	  LCD_writedata(i+0x30);

  LCD_writedata(j+0x30);
  LCD_writedata(k+0x30);
  LCD_writedata('.');
  LCD_writedata(l+0x30);
}
/******************************************************************************
function: LCD_display_time(unsigned char H,unsigned char L,unsigned int seconds)
introduction: Display current time on a position.
parameters: H( 0~1 ) indicates display line, H( 0~15) indicates display row, second is the seconds to display
return value:
*******************************************************************************/
void LCD_display_time(unsigned char H,unsigned char L,unsigned long seconds)
{
	  unsigned char hr,mn,sec;
	  unsigned char d,b;
	  hr = seconds/3600; 	    //get hours
	  mn = (seconds%3600)/60;	//get minutes
	  sec = (seconds%3600)%60;  //get seconds

	  if(H==0)		//first line
	  {
		  LCD_writecom(0x80+L);// row L
	  }
	  else if(H==1)	//second line
	  {
		  LCD_writecom(0xc0+L);// row L
	  }

	  d = hr/10;
	  b = hr%10;
	  LCD_writedata(d+0x30);
	  LCD_writedata(b+0x30);
	  LCD_writedata(':');
	  d = mn/10;
	  b = mn%10;
	  LCD_writedata(d+0x30);
	  LCD_writedata(b+0x30);
	  LCD_writedata(':');
	  d = sec/10;
	  b = sec%10;
	  LCD_writedata(d+0x30);
	  LCD_writedata(b+0x30);
}

/******************************************************************************
function: void LCD_display_char(unsigned char L,unsigned char H,char ch)
introduction: Display a signal character on a position
parameters:H( 0~1 ) indicates display line, H( 0~15) indicates display row, ch indicates the character to display.
return value:
*******************************************************************************/
void LCD_display_char(unsigned char L,unsigned char H,char ch)
{
	 if(L==0)		//first line
	  {
		  LCD_writecom(0x80+H);// row H
	  }
	  else if(L==1)	//second line
	  {
		  LCD_writecom(0xC0+H);// row H
	  }
	  LCD_writedata(ch);
}

/******************************************************************************
function: void LCD_clear(void)
introduction: clean the screen of the LCD
parameters:
return value:
*******************************************************************************/
void LCD_clear(void)
{
	LCD_writecom(0x01);//
	LCD_delay_Nms(1);
	LCD_writecom(0x02);//
	LCD_delay_Nms(1);
}

/******************************************************************************
function: void LCD_writecom(unsigned char c)
introduction: write command to the LCD
parameters: c indicates the command to be written.
return value:
*******************************************************************************/
void LCD_writecom(unsigned char c)	//write command
{
	LCD_CS_LOW;		//set CS low -- activate chip
	LCD_RS_LOW;		//set RS low for transmitting command


	//USCI_B0 TX buffer ready?
	while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
										EUSCI_B_SPI_TRANSMIT_INTERRUPT)) ;

	//Transmit Data to slave
	EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, c);

	// wait until done
	//USCI_B0 RX buffer done?
	while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
										   EUSCI_B_SPI_RECEIVE_INTERRUPT)) ;
	//Receive garbage Data from slave, and throw away
	// just to clear interrupts and overflow fags
	c  = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);

	LCD_CS_HIGH;	//set CS high -- deactivate chip
}

/******************************************************************************
function: void LCD_writedata(unsigned char d)
introduction: write data to the LCD
parameters:	d indicates the data to be written.
return value:
*******************************************************************************/
void LCD_writedata(unsigned char d)	//write data
{
	LCD_CS_LOW;		//set CS low -- activate chip
	LCD_RS_HIGH;	//set RS low for transmitting command
					//set RS high for transmitting data

	//USCI_B0 TX buffer ready?
	while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
										   EUSCI_B_SPI_TRANSMIT_INTERRUPT)) ;

	//Transmit Data to slave
	EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, d);

	// wait until done
	//USCI_B0 RX buffer done?
	while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
										   EUSCI_B_SPI_RECEIVE_INTERRUPT)) ;
	//Receive garbage Data from slave, and throw away
	// just to clear interrupts and overflow fags
	d  = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);

	LCD_CS_HIGH;	//set CS high -- deactivate chip
}

/******************************************************************************
function: void LCD_delay_Nms(unsigned char i)
introduction:
parameters: i means delay i ms
return value:
*******************************************************************************/
void LCD_delay_Nms(unsigned char i)
{
	unsigned int t;
	for (t = i; t > 0; t--)
	{
		__delay_cycles(1000); // 1MHz clock
	}

}
