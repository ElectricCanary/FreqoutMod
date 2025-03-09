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
#define FOOT_DDR DDRA
#define FOOT_SFR PINA
#define FOOT_PORT PORTA
#define FOOT1_PIN PINA0
#define FOOT1_BV 0
#define FOOT2_PIN PINA1
#define FOOT2_BV 1
#define FOOT3_PIN PINA2
#define FOOT3_BV 2
#define CV_DDR DDRA
#define CV_PIN PINA7

#define DEBOUNCE_TIME 800
#define TIMER_TOP 0xFFFF
#define NB_SWITCH 3
#define TYPE_SUB 0
#define TYPE_1ST 1
#define TYPE_2ND 2
#define TYPE_3RD 3
#define TYPE_5TH 4
#define TYPE_NATLOW 5
#define TYPE_NATHI 6
#define NB_TYPE 7
#define TYPE_DEFAULT TYPE_3RD

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
    TCNT1 = 0;
    ICR1 = TIMER_TOP;
    OCR1B = (TIMER_TOP / NB_TYPE) * TYPE_DEFAULT;
    TCCR1A |= (1<<COM1B0) | (1<<WGM11);
    TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS10);
}

void IO_Init(void)
{
    LED_DDR |= (1<<LED1_PIN) | (1<<LED2_PIN) | (1<<LED3_PIN); //LED as output
    CV_DDR |= (1<<CV_PIN); //CV as output
    FOOT_PORT |= (1<<FOOT1_PIN) | (1<<FOOT2_PIN) | (1<<FOOT3_PIN); //Pull-up for footswitches
}

void main(void) 
{
    IO_Init();
    Timer_Init();
    
    while(1)
    {
        
    }
}