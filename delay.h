#ifndef DELAY_H
#define DELAY_H

#include <util/delay.h>

#include "globals.h"

#define ONE 1
#define DELAY_1K 1000

void delay_s(u16 s)
{
    while (s-- > 0)
    {
        _delay_ms(DELAY_1K);
    }
}
void delay_ms(u16 ms)
{
    while (ms-- > 0)
    {
        _delay_ms(ONE);
    }
}
void delay_us(u16 us)
{
    while (us-- > 0)
    {
        _delay_us(ONE);
    }
}
#endif