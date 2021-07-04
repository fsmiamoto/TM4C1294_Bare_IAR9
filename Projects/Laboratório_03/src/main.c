#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverleds.h" 

#define SYSTICK_ONE_SEC_24MHZ_CLK 12000000

typedef enum {
  COUNT_000 = 0,
  COUNT_001 = LED3,
  COUNT_011 = LED2 | LED3,
  COUNT_010 = LED2,
  COUNT_110 = LED1 | LED2,
  COUNT_111 = LED1 | LED2 | LED3,
  COUNT_101 = LED1 | LED3,
  COUNT_100 = LED1,
} state_t;

volatile uint8_t tick = 0;
state_t state;

void SysTick_Handler(void){
  tick = 1;
} 

void setUp(void) {
  LEDInit(LED1 | LED2 | LED3);
  SysTickPeriodSet(SYSTICK_ONE_SEC_24MHZ_CLK);
  SysTickIntEnable();
  SysTickEnable();
}

void main(void){
  setUp();

  state = COUNT_000; 

  for(;;){
    if(tick){
      tick = 0;
      switch(state){
        case COUNT_000:
          state = COUNT_001;
          break;
        case COUNT_001:
          state = COUNT_011;
          break;
        case COUNT_011:
          state = COUNT_010;
          break;
        case COUNT_010:
          state = COUNT_110;
          break;
        case COUNT_110:
          state = COUNT_111;
          break;
        case COUNT_111:
          state = COUNT_101;
          break;
        case COUNT_101:
          state = COUNT_100;
          break;
        case COUNT_100:
          state = COUNT_000;
          break;
      } 
      LEDOn(state);
      LEDOff(~state);
    } 
  } 
} 
