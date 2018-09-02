#ifndef delays_h__
#define delays_h__

#include <avr/io.h>

#define NOP	{ asm volatile("nop"); }

#define DELAY_1us	{ NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; }

#define DELAY_10us	{ DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; DELAY_1us; }

void delay_us(uint8_t us);

void delay_10x_us(uint8_t us);

#endif // delays_h__
