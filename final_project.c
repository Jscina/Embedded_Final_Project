#include <stdint.h>
#include "tm4c123gh6pm.h"

// Port F
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTF_LOCK_R (*((volatile uint32_t *)0x40025520))
#define GPIO_PORTF_CR_R 	(*((volatile uint32_t *)0x40025524))
#define GPIO_PORTF_AMSEL_R (*((volatile uint32_t *)0x40025528))
#define GPIO_PORTF_PCTL_R	(*((volatile uint32_t *)0x4002552C))
#define GPIO_PORTF_DIR_R (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_PUR_R (*((volatile uint32_t *)0x40025510))
#define GPIO_PORTF_DEN_R (*((volatile uint32_t *)0x4002551C))

// Port A
#define GPIO_PORTA_AMSEL_R (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_PCTL_R	(*((volatile uint32_t *)0x4000452C))
#define GPIO_PORTA_DIR_R (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R (*((volatile uint32_t *)0x4000451C))

// Port E
#define GPIO_PORTE_AMSEL_R (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_AFSEL_R (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_DEN_R (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_DIR_R (*((volatile uint32_t *) 0x40024400))
#define GPIO_PORTE_PCTL_R (*((volatile uint32_t *) 0x4002552C))
#define GPIO_PORTE_PUR_R (*((volatile uint32_t *) 0x40024510))
#define GPIO_PORTE_DATA_R (*((volatile uint32_t *) 0x400243FC))


// System Clock
#define NVIC_ST_CTRL_R (*((volatile uint32_t *) 0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile uint32_t *) 0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile uint32_t *) 0xE000E018))
	
#define CLOCK_TICK_MS (1000000/12.5-1)

// Outputs
// Main St Lights
#define PA2 (*((volatile uint32_t *)0x40004010)) // Red LED
#define PA3 (*((volatile uint32_t *)0x40004020)) // Yellow LED
#define PA4 (*((volatile uint32_t *)0x40004040)) // Green  LED

// Side St  Lights
#define PA5 (*((volatile uint32_t *)0x40004080)) // Red  LED
#define PA6 (*((volatile uint32_t *)0x40004100)) // Yellow  LED
#define PA7 (*((volatile uint32_t *)0x40004200)) // Green  LED

// Input Crosswalk
#define PF4 (*((volatile uint32_t *)0x40025040)) // SW1
#define PF0 (*((volatile uint32_t *)0x40025004)) // SW2

// Distance Sensor
#define PE4 (*((volatile uint32_t *)0x40024040)) // Pin for sensor IN?
#define PE5 (*((volatile uint32_t *)0x40024050)) // Pin for sensor OUT?

// Main Street Lights
#define MainSt_Red PA2        // Red LED at PA2
#define MainSt_Yellow PA3     // Yellow LED at PA3
#define MainSt_Green PA4      // Green LED at PA4

// Side Street Lights
#define SideSt_Red PA5        // Red LED at PA5
#define SideSt_Yellow PA6     // Yellow LED at PA6
#define SideSt_Green PA7      // Green LED at PA7

// Main St Crosswalk
#define MainSt_Crosswalk PF4 // Switch at PF4

// Side St Crosswalk
#define SideSt_Crosswalk PF0 // Switch at PF0

void SysTick_Init(void) {
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = CLOCK_TICK_MS;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}


void PortF_Init(void){ 
    volatile uint32_t delay;

    SYSCTL_RCGCGPIO_R |= 0x20;      // 1) activate clock for Port F
    delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to start

    GPIO_PORTF_LOCK_R = 0x4C4F434B; // 3) unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;         // allow changes to port F registers

    GPIO_PORTF_AMSEL_R &= ~0x11;     // 4) clear bits 2, 3 in GPIO_PORTF_AMSEL_R to disable analog on PF0, PF4

    GPIO_PORTF_PCTL_R &= ~0xF000F;   // 5) clear bits 0-3, 16-19 in GPIO_PORTF_PCTL_R for GPIO on PF0, PF4

    GPIO_PORTF_DIR_R &= ~0x10;       // 6) Clear bit 4 in GPIO_PORTF_DIR_R to make PF4 an IN (switch)
		GPIO_PORTF_DIR_R &= ~0x1;        // 6) Clear bit 0 inn GPIO_PORTF_DIR_R to make PF0 an IN (switch)

    GPIO_PORTF_AFSEL_R &= ~0x11;      // 7) clear bits 0 and 4 in GPIO_PORTF_AFSEL_R to disable alt funct on PF0, PF4

    GPIO_PORTF_PUR_R |= 0x11;        // 8) Set bit 0-4 in GPIO_PORTF_PUR_R to enable pull-up on PF4, PF0 (switch)

    GPIO_PORTF_DEN_R |= 0x11;        // 9) Set the bits 0-4 in GPIO_PORTF_DEN_R to enable digital I/O on PF0, PF4
}


void PortA_Init(void) {
    volatile uint32_t delay;
    SYSCTL_RCGCGPIO_R |= 0x01;      // 1) activate clock for Port A
    delay = SYSCTL_RCGCGPIO_R;       // 2) allow time for clock to start

    GPIO_PORTA_AMSEL_R &= ~0xFC;     // 3) disable analog on PA2-PA7
    GPIO_PORTA_PCTL_R &= ~0xFFFFFF00; // 4) configure PA2-PA7 as GPIO
    GPIO_PORTA_DIR_R |= 0xFC;        // 5) set PA2-PA7 to output
    GPIO_PORTA_AFSEL_R &= ~0xFC;      // 6) disable alt funct on PA2 - PA7
    GPIO_PORTA_DEN_R |= 0xFC;        // 7) enable digital I/O on PA2-PA7
}

void PortE_Init(void) {
    volatile uint32_t delay;

    // 1) activate clock for Port E
    SYSCTL_RCGCGPIO_R |= 0x10;
    delay = SYSCTL_RCGCGPIO_R;       // allow time for clock to start

    // 2) disable analog function on PE4 and PE5
    GPIO_PORTE_AMSEL_R &= ~0x30;

    // 3) disable alternate function on PE4 and PE5
    GPIO_PORTE_AFSEL_R &= ~0x30;

    // 4) configure PE4 and PE5 as GPIO
    GPIO_PORTE_PCTL_R &= ~0x00FF0000;

    // 5) set the direction of PE4 and PE5 (input/output)
    GPIO_PORTE_DIR_R &= ~0x10;  // Clear bit 4 to make PE4 input
    GPIO_PORTE_DIR_R |= 0x20;   // Set bit 5 to make PE5 output

    // 6) enable pull-up on PE4 if it is an input
    // GPIO_PORTE_PUR_R |= 0x10;

    // 7) enable digital I/O on PE4 and PE5
    GPIO_PORTE_DEN_R |= 0x30;
}


void delay(uint32_t ms) {
    uint32_t flag_clearing = NVIC_ST_CTRL_R;
		uint32_t i;
		NVIC_ST_RELOAD_R = CLOCK_TICK_MS;
		NVIC_ST_CURRENT_R = 0;
		for(i = 0; i<= ms; i++) {
			while(!(NVIC_ST_CTRL_R & 0x00010000));
		}
		
}


void SetMainStreetLight(int red, int yellow, int green) {
    MainSt_Red = red;
    MainSt_Yellow = yellow;
    MainSt_Green = green;
}

void SetSideStreetLight(int red, int yellow, int green) {
    SideSt_Red = red;
    SideSt_Yellow = yellow;
    SideSt_Green = green;
}

int main(void){
    SysTick_Init();
    PortF_Init();
    PortA_Init();
    PortE_Init();

    // Initial State: Main St Green, Side St Red
    SetMainStreetLight(0, 0, 1);
    SetSideStreetLight(1, 0, 0);

    while(1){
        // Check if the Main St crosswalk button is pressed or car is detected
        if (MainSt_Crosswalk == 0 || PE4 == 1) {
            // Change Main St to Yellow
            SetMainStreetLight(0, 1, 0);
            delay(2000); // Delay for Yellow

            // Change Main St to Red and Side St to Green
            SetMainStreetLight(1, 0, 0);
            SetSideStreetLight(0, 0, 1);
            delay(5000); // Delay for Red/Green

            // Change Side St to Yellow before turning Red
            SetSideStreetLight(0, 1, 0);
            delay(2000); // Delay for Yellow

            // Change back to initial state
            SetMainStreetLight(0, 0, 1);
            SetSideStreetLight(1, 0, 0);
				}

        // Check if the Side St crosswalk button is pressed
        if (SideSt_Crosswalk == 0) {
            // Ensure Side St goes through yellow before turning red
            SetSideStreetLight(0, 1, 0);
            delay(2000); // Delay for Yellow

            // Revert to initial state
            SetMainStreetLight(0, 0, 1);
            SetSideStreetLight(1, 0, 0);
        }

			}
}

