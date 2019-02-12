#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define DELAY_MS 1
#define DELAY_US 1
#define DELAY_1K 1000
#define CLOCK_RATE 10000

void delay_s(int s);

int main(void)
{
   init(); //initialize board hardware

   delay_s(2);
   print_string("Hello, ");
   delay_s(1);
   lcd_cursor(0, 1);
   print_string("Jared!");
   delay_s(2);
   return 0;
}

void delay_s(int s){
   _delay_ms(DELAY_1K);
}
