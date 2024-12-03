 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 *
 *
 *******************************************************************************/

#include "keypad.h"
#include "lcd.h"
/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/
#if (N_col == 3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number);
#elif (N_col == 4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x4 keypad
 */
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_number);
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
uint8 KeyPad_getPressedKey(void){
	uint8 col,row;
	while(1)
	{
		for(col=0;col<N_col;col++) /* loop for columns */
		{
			/* 
			 * each time only one of the column pins will be output and 
			 * the rest will be input pins include the row pins 
			 */ 
			KEYPAD_PORT_DIR = (0b00010000<<col); 
			
			/* 
			 * clear the output pin column in this trace and enable the internal 
			 * pull up resistors for the rows pins
			 */ 
			KEYPAD_PORT_OUT = (~(0b00010000<<col)); 
			for(row=0;row<N_row;row++) /* loop for rows */
			{
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row)) /* if the switch is press in this row */ 
				{
					#if (N_col == 3)  
						return KeyPad_4x3_adjustKeyNumber((row*N_col)+col+1); 
					#elif (N_col == 4)
						return KeyPad_4x4_adjustKeyNumber((row*N_col)+col+1);
					#endif				
				}
			}
		}
	}	
}
static int x=0,result=0,y=0;
#if (N_col == 3) 

static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number)
{
	switch(button_number)
	{
		case 10: return '*'; // ASCII Code of =
				 break;
		case 11: return 0;
				 break;		
		case 12: return '#'; // ASCII Code of +
				 break;
		default: return button_number;								 						
	}
} 

#elif (N_col == 4)
 
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_number)
{
	switch(button_number)
	{
		case 1: x=x*10+7;
			return '7';
		break;
		case 2:x=x*10+8;
			return '8';
				break;
		case 3: x=x*10+9;
		return '9';
				break;
		case 5: x=x*10+4;
		return '4';
				break;
		case 6: x=x*10+5;
		return '5';
				break;
		case 7: x=x*10+6;
		return '6';
				break;
		case 9: x=x*10+1;
		return '1';
				break;
		case 10: x=x*10+2;
		return '2';
				break;
		case 11: x=x*10+3;
		return '3';
				break;			
		case 14: x*=10;
		return '0';
				break;
		case 8: LCD_clearScreen();
		y=1;
		result+=x;
		x=0;
			return '*'; /* ASCII Code of '*' */
						break;
		case 4: LCD_clearScreen();
		y=2;
		result+=x;
		x=0;
		return '%'; // ASCII Code of %
				break;
		case 12:LCD_clearScreen();
		y=3;
		result+=x;
		x=0;
			return '-'; /* ASCII Code of '-' */
			break;
		case 15:LCD_clearScreen();
			switch(y){
			case 1:
				result*=x;
				break;
			case 2:
				result/=x;
				break;
			case 3:
				result-=x;
				break;
			case 4:
				result+=x;
				break;
			}
			char buff[16]; /* String to hold the ascii result */
			   itoa(result,buff,10); /* 10 for decimal */
			   LCD_displayString(buff);
		return 0; /* ASCII Code of '=' */
				break;
		case 16: LCD_clearScreen();
		y=4;
		result+=x;
		x=0;
			return '+'; /* ASCII Code of '+' */
				break;
		case 13:LCD_clearScreen();
			    x=0;result=0;/* ASCII of Enter */
				return 0;
		break;
		default: return button_number;
	}
} 

#endif
