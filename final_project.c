#include <stdint.h>
#include "tm4c123gh6pm.h"

#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTF_LOCK_R (*((volatile uint32_t *)0x40025520))
#define GPIO_PORTF_CR_R 	(*((volatile uint32_t *)0x40025524))
#define GPIO_PORTF_AMSEL_R (*((volatile uint32_t *)0x40025528))
#define GPIO_PORTF_PCTL_R	(*((volatile uint32_t *)0x4002552C))
#define GPIO_PORTF_DIR_R (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_PUR_R (*((volatile uint32_t *)0x40025510))
#define GPIO_PORTF_DEN_R (*((volatile uint32_t *)0x4002551C))
	
#define GPIO_PORTA_AMSEL_R (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_PCTL_R	(*((volatile uint32_t *)0x4000452C))
#define GPIO_PORTA_DIR_R (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R (*((volatile uint32_t *)0x4000451C))

// System Clock
#define NVIC_ST_CTRL_R (*((volatile uint32_t *) 0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile uint32_t *) 0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile uint32_t *) 0xE000E018))
	
#define CLOCK_TICK_MS (1000000/12.5-1)

// Outputs
#define PF2 (*((volatile uint32_t *)0x40025010)) // Blue LED
#define PF3 (*((volatile uint32_t *)0x40025020)) // Green LED
#define PA2 (*((volatile uint32_t *)0x40004020)) // Red LED

// Input
#define PF4 (*((volatile uint32_t *)0x40025040)) // Switch on Port F

void SysTick_Init(void) {
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = CLOCK_TICK_MS;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}


void delay(uint32_t ms) {
    uint32_t flag_cealring = NVIC_ST_CTRL_R;
		uint32_t i;
		NVIC_ST_RELOAD_R = CLOCK_TICK_MS;
		NVIC_ST_CURRENT_R = 0;
		for(i = 0; i<= ms; i++) {
			while(!(NVIC_ST_CTRL_R & 0x00010000));
		}
		
}

// Will need to redo the port initialization here
void PortF_Init(void){ 
    volatile uint32_t delay;

    SYSCTL_RCGCGPIO_R |= 0x20;      // 1) activate clock for Port F
    delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to start

    GPIO_PORTF_LOCK_R = 0x4C4F434B; // 3) unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;         // allow changes to port F registers

    GPIO_PORTF_AMSEL_R &= ~0xC;     // 4) clear bits 2, 3 in GPIO_PORTF_AMSEL_R to disable analog on PF2, PF3

    GPIO_PORTF_PCTL_R &= ~0xFF00;   // 5) clear bits 8-15 in GPIO_PORTF_PCTL_R for GPIO on PF2, PF3

    GPIO_PORTF_DIR_R |= 0x04;        // 6) Set bit 2 in GPIO_PORTF_DIR_R to make PF2 as OUT (blue LED)
		GPIO_PORTF_DIR_R |= 0x08;				 // 6) Set bit 3 in GPIO_PORTF_DIR_R to make PF3 as OUT (green LED)
    GPIO_PORTF_DIR_R &= ~0x10;       // 6) Clear bit 4 in GPIO_PORTF_DIR_R to make PF4 as IN (switch)

    GPIO_PORTF_AFSEL_R &= ~0x1C;      // 7) clear bits 2-4 in GPIO_PORTF_AFSEL_R to disable alt funct on PF2, PF3, PF4

    GPIO_PORTF_PUR_R |= 0x10;        // 8) Set bit 4 in GPIO_PORTF_PUR_R to enable pull-up on PF4 (switch)

    GPIO_PORTF_DEN_R |= 0x1C;        // 9) Set the bits 2-4 in GPIO_PORTF_DEN_R to enable digital I/O on PF2, PF3, PF4
}
// Will need to redo the port initialization
void PortA_Init(void) {
    volatile uint32_t delay;
    SYSCTL_RCGCGPIO_R |= 0x01;      // 1) activate clock for Port A
    delay = SYSCTL_RCGCGPIO_R;       // 2) allow time for clock to start

    GPIO_PORTA_AMSEL_R &= ~0x04;     // 3) disable analog on PA2
    GPIO_PORTA_PCTL_R &= ~0x00000F00; // 4) configure PA2 as GPIO
    GPIO_PORTA_DIR_R |= 0x04;        // 5) set PA2 to output
    GPIO_PORTA_AFSEL_R &= ~0x04;      // 6) disable alt funct on PA2
    GPIO_PORTA_DEN_R |= 0x04 ;        // 7) enable digital I/O on PA2
}



int main(void){
    PortF_Init();
    PortA_Init();
    PF2 |= 0x04;    // Turn on the blue LED
    PA2 &= ~0x04;   // Turn off the red LED

    while(1){       
			// Main Logic Here
    }
}
