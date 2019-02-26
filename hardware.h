#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "delay.h"
#include "globals.h"
#include "defs.h"

// Functions for external LEDs on digital pins
void init_led(u08 num){
	digital_dir(num, 1);
}

void led_high(u08 num){
	digital_out(num, 1);
}

void led_low(u08 num){
	digital_out(num, 0);
}

void print_std_dev_pos(float std_dev, float pos){
    clear_screen();
    lcd_cursor(0, 0);
    print_string("s: ");
    print_num((int) std_dev);
    lcd_cursor(0, 1);
    print_string("p: ");
    print_num((int) pos);
}

light_data_t read_light_sensor(){
    light_data_t data;
    data.left = analog(LIGHT_SENSOR_PIN_LEFT);
    data.right = analog(LIGHT_SENSOR_PIN_RIGHT);
    return data;
}

line_data_t read_line_sensor(){
    line_data_t data;
    data.left = analog(LINE_SENSOR_LEFT);
    data.right = analog(LINE_SENSOR_RIGHT);
    return data;
}


void motor(u08 num, int speed){
    if (num == MOTOR_RIGHT){
        set_servo(MOTOR_RIGHT, ((0 - speed) * 0.3333 ) + 127);
    }else{
        set_servo(MOTOR_LEFT, ( speed * 0.3333 ) + 127);
    }
}

void set_motors(motor_command_t motors){
    set_servo(MOTOR_LEFT, ( motors.left * 0.3333 ) + 127);
    set_servo(MOTOR_RIGHT, ((0 - motors.right) * 0.3333 ) + 127);
}
void motors(int left, int right){
    set_servo(MOTOR_LEFT, ( left * 0.3333 ) + 127);
    set_servo(MOTOR_RIGHT, ((0 - right) * 0.3333 ) + 127);
}

void halt(){
    motor(MOTOR_LEFT, 0);
    motor(MOTOR_RIGHT, 0);
}

u08 get_btn2(){
    return !digital(BTN2_PIN);
}

void start_screen(char *str){
    halt();
    lcd_cursor(0, 0);
    print_string(str);
    lcd_cursor(0, 1);
    print_string(str + 8);
    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }
}

u08 poll_analog_pin(u08 pin_num){
    return analog(pin_num);
}


void print_4(u08 a, u08 b, u08 c, u08 d){
    clear_screen();
    lcd_cursor(0, 0);
    print_num(a);
    lcd_cursor(4, 0);
    print_num(b);
    lcd_cursor(0, 1);
    print_num(c);
    lcd_cursor(4, 1);
    print_num(d);
    
}
void print_2(u08 a, u08 b){
    clear_screen();
    lcd_cursor(0, 0);
    print_num(a);
    lcd_cursor(0, 1);
    print_num(b); 
}
void print_1(u08 a){
    clear_screen();
    lcd_cursor(0, 0);
    print_num(a);
}

#endif
