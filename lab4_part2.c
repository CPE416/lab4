// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 4 Part 2
// Description: 

#include <stdio.h>

#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"

// Settings
#define DELAY_MS 100 // Delay time for control loop

u08 set_mode(u08 mode, int *flag);
void print_data(line_data_t sensor, int count);
void print_training(int count);
void print_training2(int count);
void print_training3(int count1, int count2);
void print_training4(int count1, int count2);

int main(void)
{
    init();

    // Variables
    u08 num_block;
    u08 target_block;
    line_data_t line_data;
    motor_command_t motors;
    motors.left = 0;
    motors.right = 0;
    halt();


    clear_screen();
    print_string("Part 2");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }

    //Choose Number of Blocks
    num_block = get_num_block();

    //Button Press Delay before start
    delay_ms(200);

    while (1){

    }
}

u08 get_num_block(){
    u08 block_num = 3;
    while((get_btn() == 0) && (get_btn2() == 0)){
        
    }
}

void print_num_block(u08 num){
    
}

void print_data(line_data_t sensor, int count){
    clear_screen();
    print_string("Data");
    lcd_cursor(4, 0);
    print_num(count);
    lcd_cursor(0, 1);
    print_num(sensor.left);
    lcd_cursor(4, 1);
    print_num(sensor.right);
}

void print_training(int count){
    clear_screen();
    print_string("Training");
    lcd_cursor(0, 1);
    print_num(count);
}
void print_training2(int count){
    clear_screen();
    print_string("Training");
    lcd_cursor(0, 1);
    print_num(count);
    lcd_cursor(4, 1);
    print_string("a");
}

void print_training3(int count1, int count2){
    clear_screen();
    print_string("Training");
    lcd_cursor(0, 1);
    print_num(count1);
    lcd_cursor(4, 1);
    print_num(count2);
}

void print_training4(int count1, int count2){
    clear_screen();
    print_string("Neural");
    lcd_cursor(0, 1);
    print_num(count1);
    lcd_cursor(4, 1);
    print_num(count2);
}
