#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h"
#include "delay.h"
#include "hardware.h"

#define ROTATE_TICKS 32

volatile uint16_t left_encoder = 0;
volatile uint16_t right_encoder = 0;

void reset_left_encoder(){
    left_encoder = 0;
}
void reset_right_encoder(){
    right_encoder = 0;
}

void reset_encoders(){
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

    reset_encoders();
}

void print_encoders(){
    clear_screen();
    lcd_cursor(0, 0);
    print_2(left_encoder, right_encoder);
}

void forward(int ticks){
    motors(20, 20);
    reset_encoders();
    while(left_encoder < ticks && right_encoder < ticks){
        print_encoders();
        delay_ms(50);
    }
    motors(0, 0);
}

void rotate_90(){
    motors(30, -30);
    reset_encoders();
    while(left_encoder < ROTATE_TICKS && right_encoder < ROTATE_TICKS){
        print_encoders();
        delay_ms(50);
    }
    motors(0, 0);
}

ISR(PCINT0_vect) {
   left_encoder++;  //increment left encoder
}

ISR(PCINT1_vect) {
   right_encoder++;  //increment right encoder
}

#endif