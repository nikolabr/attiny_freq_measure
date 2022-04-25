# attiny_freq_measure
Uses Timer 1 to count the number of rising edges in a certain time period.

## IMPORTANT NOTES:
- The software serial baud rate is tied to the CPU frequency. It assumes the speed of 1MHz, but if the CPU is running at 8 or 16MHz, the baud rate is 8 or 16 times slower.
- The logic voltage level is defined by the input voltage! So if using with STM or other 3.3V chips, make sure the voltage source is 3.3V. 
- Use internal crystal. 
- Do not run at over 8MHz when powering it on 3.3V. Datasheet: 0 – 10 MHz @ 2.7 - 5.5V, 0 - 20 MHz @ 4.5 - 5.5V
