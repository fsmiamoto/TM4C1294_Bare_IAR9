## Lab 01

The objective of this lab is to switch LED D4 - available at GPIO Port F Pin 0 - every second, without using time interrupts.

Thefore, the switching frequency should be provided by _time delay loops_.

## Solution

First, we can defined a macro that should do the trick for a given number of cycles:

```c
#define waitFor(cycles) for (int i = 0; i < cycles; i++)
```

One thing to keep in mind is that the system clock was set to 120 MHz:
```c
#define SYS_CLK_CONFIG                                                         \
  SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480
#define SYS_CLK_FREQ (120000000UL)
```

Therefore, we need to find an optimal number of cycles to call the macro `waitFor` with that should yield a wait of about 1 second.

For that, I've performed some experimentation with adding a logging breakpoint and observing the generated logs.

![LogBreakpoint](https://user-images.githubusercontent.com/20388082/122680959-508cc800-d1c8-11eb-8b29-082bb61ef34c.png)

![Timings](https://user-images.githubusercontent.com/20388082/122680811-8aa99a00-d1c7-11eb-939d-82a6c96d9fdb.png)

As we can see above, the logs are separated by about a second so I've tuned the value
until a satisfactory result was obtained.

```c
#define ONE_SEC 4500000
```