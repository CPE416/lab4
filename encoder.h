#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h"
#include "hardware.h"

#define ROTATE_TICKS 30

volatile uint16_t left_encoder = 0;
volatile uint16_t right_encoder = 0;

void reset_left_encoder(){
    left_encoder = 0;
}
void reset_right_encoder(){
    right_encoder = 0;
}

void reset_enocders(){
    reset_left_encoder();
    reset_right_encoder();
}

void init_encoder() {

    // enable encoder interrupts
    EIMSK = 0;
    EIMSK |= _BV(PCIE1) | _BV(PCIE0);
    PCMSK1 |= _BV(PCINT13); //PB5 - digital 5
    PCMSK0 |= _BV(PCINT6);  //PE6 - digital 4

    // enable pullups
    PORTE |= _BV(PE6);
    PORTB |= _BV(PB5);

    reset_enocders();
}

void forward(int ticks){
    motors(20, 20);
    reset_enocders();
    while(left_encoder < ticks && right_encoder < ticks){}
    motors(0, 0);
}

void rotate_90(){
    motors(10, -10);
     reset_enocders();
    while(left_encoder < ROTATE_TICKS){}
    motors(0, 0);
}


ISR(PCINT0_vect) {
   left_encoder++;  //increment left encoder
}

ISR(PCINT1_vect) {
   right_encoder++;  //increment right encoder
}

#endif