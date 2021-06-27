#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_memmap.h"

#define LED GPIO_PIN_0
#define ONE_SEC 4500000

#define SYS_CLK_CONFIG                                                         \
  SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480
#define SYS_CLK_FREQ (120000000UL)

#define waitFor(cycles) for (int i = 0; i < cycles; i++)
#define writeToLed(value) GPIOPinWrite(GPIO_PORTF_BASE, LED, value & LED)

void setUp(void) {
  SysCtlClockFreqSet(SYS_CLK_CONFIG, SYS_CLK_FREQ);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    ;
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED);
  GPIOPinWrite(GPIO_PORTF_BASE, LED, 0);
  GPIOPadConfigSet(GPIO_PORTF_BASE, LED, GPIO_STRENGTH_12MA,
                   GPIO_PIN_TYPE_STD);
}

void main(void) {
  setUp();

  for (;;) {
    writeToLed(0);
    waitFor(ONE_SEC);
    writeToLed(1);
    waitFor(ONE_SEC);
  }
}