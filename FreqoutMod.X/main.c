/* 
 * File:   main.c
 * Author: Antoine for Electric Canary
 * 
 * This is a CV generator for the Attiny84A.
 * The CV is created by a PWM timer.
 * It has 4 footswitches to select between different CV levels.
 * This has been created for a CV mod to the Digitech FreqOut.
 * 
 * Pinout:
 * 
 * 1: VCC
 * 2: LED1
 * 3: LED2
 * 4: LED3
 * 5: LED4
 * 6: CV Out
 * 7:
 * 8:
 * 9:
 * 10: Footswitch 4
 * 11: Footswitch 3
 * 12: Footswitch 2
 * 13: Footswitch 1
 * 14: GND
 * 
 * Created on 13 février 2025, 22:41
 */

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_DDR DDRB
#define LED1_PIN PINB0
#define LED2_PIN PINB1
#define LED3_PIN PINB3
#define LED4_PIN PINB2
#define FOOT_DDR DDRA
#define FOOT_SFR PINA
#define FOOT_PORT PORTA
#define FOOT1_PIN PINA0
#define FOOT1_BV 0
#define FOOT2_PIN PINA1
#define FOOT2_BV 1
#define FOOT3_PIN PINA2
#define FOOT3_BV 2
#define FOOT4_PIN PINA3
#define FOOT4_CV 3
#define CV_DDR DDRA
#define CV_PIN PINA7

#define DEBOUNCE_TIME 800


uint8_t debounce(uint8_t BV, uint8_t SFR)   //tells with certainty if button is pressed
{
	if (bit_is_clear(SFR,BV))               //if button pressed
	{
		_delay_us(DEBOUNCE_TIME);           //wait debounce time
		if (bit_is_clear(SFR,BV))           //if still pressed return 1
		{
			return(1);
		}
		else                                //if not return 0
		{
			return(0);
		}
	}
	else
	{
		return(0);
	}
}

void Timer_Init(void)
{
    
}

void IO_Init(void)
{
    LED_DDR |= (1<<LED1_PIN) | (1<<LED2_PIN) | (1<<LED3_PIN) | (1<<LED4_PIN); //LED as output
    CV_DDR |= (1<<CV_PIN); //CV as output
    FOOT_PORT |= (1<<FOOT1_PIN) | (1<<FOOT2_PIN) | (1<<FOOT3_PIN) | (1<<FOOT4_PIN); //Pull-up for footswitches
}

void main(void) 
{
    IO_Init();
    Timer_Init();
    
}

