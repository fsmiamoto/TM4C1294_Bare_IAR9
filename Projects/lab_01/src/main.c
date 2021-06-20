#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_memmap.h"

#define LED_D4 GPIO_PIN_0
#define ONE_SEC 12000000

void setUpGPIO() {
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);         
   while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
   GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_D4);
   GPIOPinWrite(GPIO_PORTF_BASE, LED_D4,0);
   GPIOPadConfigSet(GPIO_PORTF_BASE, LED_D4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
}

int writeToLed(uint8_t value) {
  GPIOPinWrite(GPIO_PORTF_BASE, LED_D4, value);
  return value;
}

void wait(int cycles){
   for(int i = 0; i < cycles; i++);
}

int main(void) {
   setUpGPIO();
   for (;;) {
      writeToLed(0);
      wait(ONE_SEC);
      writeToLed(1);
      wait(ONE_SEC);
  }
}