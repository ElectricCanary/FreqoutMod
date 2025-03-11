/* 
 * File:   main.c
 * Author: Antoine for Electric Canary
 * 
 * This is a CV generator for the Attiny1606.
 * The CV is created by a PWM timer.
 * It has 4 footswitches to select between different CV levels.
 * This has been created for a CV mod to the Digitech FreqOut.
 * 
 * Pinout:
 * 
 * 1: VDD
 * 2: LED1
 * 3: LED2
 * 4: LED3
 * 5: 
 * 6: Footswitch 1
 * 7: Footswitch 2
 * 8: Footswitch 3
 * 9:
 * 10: 
 * 11: CV Out
 * 12: 
 * 13: 
 * 14:
 * 15:
 * 16: RESET/UPDI
 * 17:
 * 18:
 * 19:
 * 20: GND
 * 
 * Created on 13 février 2025, 22:41
 */

#include <avr/io.h>
#define F_CPU 20000000UL
#include <util/delay.h>

//standard fuses
FUSES = {
	.WDTCFG = 0x00, // WDTCFG {PERIOD=OFF, WINDOW=OFF}
	.BODCFG = 0x00, // BODCFG {SLEEP=DIS, ACTIVE=DIS, SAMPFREQ=1KHZ, LVL=BODLEVEL0}
	.OSCCFG = 0x02, // OSCCFG {FREQSEL=20MHZ, OSCLOCK=CLEAR}
	.SYSCFG0 = 0xF6, // SYSCFG0 {EESAVE=CLEAR, RSTPINCFG=UPDI, CRCSRC=NOCRC}
	.SYSCFG1 = 0x07, // SYSCFG1 {SUT=64MS}
	.APPEND = 0x00, // APPEND {APPEND=User range:  0x0 - 0xFF}
	.BOOTEND = 0x00, // BOOTEND {BOOTEND=User range:  0x0 - 0xFF}
};

LOCKBITS = 0xC5; // {LB=NOLOCK}

#define CV_PIN 0
#define CV_VPORT VPORTB
#define CV_PORT PORTB
#define CV_PINCTRL PORTB.PIN0CTRL
#define LED_VPORT VPORTA
#define LED_PORT PORTA
#define LED1_PIN 4
#define LED1_PINCTRL PORTA.PIN4CTRL
#define LED2_PIN 5
#define LED2_PINCTRL PORTA.PIN5CTRL
#define LED3_PIN 6
#define LED3_PINCTRL PORTA.PIN6CTRL
#define FOOT_VPORT VPORTB
#define FOOT_PORT PORTB
#define FOOT1_PIN 5
#define FOOT1_PINCTRL PORTB.PIN5CTRL
#define FOOT2_PIN 4
#define FOOT2_PINCTRL PORTB.PIN4CTRL
#define FOOT3_PIN 3
#define FOOT3_PINCTRL PORTB.PIN3CTRL

#define DEBOUNCE_TIME 800
#define TIMER_TOP 0xFF
#define NB_SWITCH 3
#define TYPE_SUB 0
#define TYPE_1ST 0.167
#define TYPE_2ND 0.333
#define TYPE_3RD 0.5
#define TYPE_5TH 0.667
#define TYPE_NATLOW 0.88
#define TYPE_NATHI 1
#define NB_TYPE 7
#define TYPE_DEFAULT TYPE_SUB
#define LED_DEFAULT_PIN LED1_PIN

uint8_t debounce(VPORT_t VPORT, uint8_t PIN)   //tells with certainty if button is pressed
{
	if (VPORT.IN & (1<<PIN))
	{
        return(0);
	}
	else                                    //if button pressed
	{
        _delay_us(DEBOUNCE_TIME);           //wait debounce time
		if (VPORT.IN & (1<<PIN))           
		{
			return(0);
		}
		else                                //if still pressed return 1
		{
			return(1);
		}
	}
}

void setType(float type)
{       
    TCA0.SINGLE.CMP0 = TIMER_TOP * type;
}

void Timer_Init(void)
{
    TCA0.SINGLE.CNT = 0;
    TCA0.SINGLE.PER = TIMER_TOP;
    setType(TYPE_DEFAULT);
    TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void IO_Init(void)
{
    //LED & CV as output
    LED_VPORT.DIR |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN);
    CV_VPORT.DIR |= (1 << CV_PIN);
    //Pull-up for footswitches
    FOOT1_PINCTRL |= PORT_PULLUPEN_bm;
    FOOT2_PINCTRL |= PORT_PULLUPEN_bm;
    FOOT3_PINCTRL |= PORT_PULLUPEN_bm;
    //Default LED on
    LED_VPORT.OUT |= (1<<LED_DEFAULT_PIN);
}

void clock_init(void)
{
    CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLB = 0;
}

void main(void) 
{
    clock_init();
    IO_Init();
    Timer_Init();
    
    uint8_t footpins[NB_SWITCH] = {FOOT1_PIN, FOOT2_PIN, FOOT3_PIN};
    uint8_t ledpins[NB_SWITCH] = {LED1_PIN, LED2_PIN, LED3_PIN};
    uint8_t state = 1;
    uint8_t previousstate;
    float types[] = {TYPE_SUB, TYPE_1ST, TYPE_2ND, TYPE_NATHI, TYPE_3RD, TYPE_5TH, TYPE_NATLOW};
    
    while(1)
    {
        for(uint8_t i = 0; i < NB_SWITCH; i++)
        {
            state &= ~(1 << i);
            state |= debounce(FOOT_VPORT, footpins[i]) << i;
            
            if((state & (1 << i)) && !((previousstate & (1 << i)) >> i)) //button pressed
            {
                setType(types[state - 1]);
                
                for(uint8_t j = 0; j < NB_SWITCH; j++)
                {
                    LED_VPORT.OUT &= ~(1 << ledpins[j]);
                    LED_VPORT.OUT |= (((state & (1 << j)) >> j) << ledpins[j]);
                }
            }
            
            previousstate = state;
        }
    }
}