## Lab 01

The objective of this lab is to switch LED D4 - available at GPIO Port F Pin 0 - every second, without using time interrupts.

Therefore, the switching frequency should be provided by _time delay loops_.

The compiler optimization should also be set to `low`.

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

## Experiments

## Different system clock frequencies

As expected, altering the the system clock will affect the timing provided
by the delay loop.

Therefore, if we want to keep the one second timing we should update the constant
`ONE_SECOND` with an appropriate value for the new system clock.

After going back to 24 MHz, the empirically verified value for the constant `ONE_SECOND` is about `1500000`

### Increase compiler optimization

As a first experiment, turning on `Static Clustering` has yield a fast switching frequency:

![opt1](https://user-images.githubusercontent.com/20388082/122682656-34d9ef80-d1d1-11eb-907e-974cfd4dc5c4.png)

As shown above, there's even cases where logs were generated within the same second - #6 to #8

Similar results were obtained when also turning on `Code motion` and 
`Common subexpression elimination`:

![opt2](https://user-images.githubusercontent.com/20388082/122682793-e416c680-d1d1-11eb-960d-65d3662e767e.png)

When using the preset `High: Balanced`, the delay loop is completely ineffective

![image](https://user-images.githubusercontent.com/20388082/122682843-293af880-d1d2-11eb-92ad-9cb0a5dc4c84.png)

More noticeably, with these optimizations the LED doesn't ever appear to light up.

Thinking it through, it makes sense that a more 'clever' compiler could notice that the
delay loops are essentially 'useless' - at least terms of performing a computation - and
therefore optimize it out. 

Digging through the disassembly, we can notice that the compiler completely ignored the `waitFor` 'calls' and simply jumps back to the loop without any delay: 

![disassembly](https://user-images.githubusercontent.com/20388082/122683168-f42fa580-d1d3-11eb-9ff0-2dbf92663243.png)

Therefore the code simply turns the LED on and off continuously without any delay.

With all of that in mind, we can conclude that using a high optimization level might be dangerous since the optimizations **might alter the correctness** of our solution.

## Final thoughts

In the name of code readability and following a **personal guideline**, 
comments were avoided since they tend to mask non-ideal naming of 
variables/functions and also might get outdated as the code evolves.

As a first experiment on that direction, I've preferred defining macros with meaningful
names rather than using comments to describe the functionality. 

I'm not sure if this process scales as the project grows but might be an alternative.