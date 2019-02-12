// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 3 Part 2
// Description: 

#include <stdio.h>

#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
//#include "prop.h"
#include "neural.h"

// Settings
#define DELAY_MS 100 // Delay time for 
#define DELAY_DATA_COLLECTION 50
#define DELAY_TRAINING_INCREASE 10
#define DELAY_NEURAL 10

#define MODE_PROP 0
#define MODE_DATA 1
#define MODE_TRAINING 2 
#define MODE_NEURAL 3

#define LEARNING_RATE 0.8

#define CACHE_SIZE 500

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
    line_data_t line_data;
    motor_command_t motors;
    int data_counter = 0;
    int current_data_counter = 0;
    int training_iteration_count = 0;
    int flag_iterate = 0;
    motors.left = 0;
    motors.right = 0;
    halt();

    line_data_t cache[CACHE_SIZE];

    neural_net_t net;
    init_net(&net, LEARNING_RATE);


    u08 mode = MODE_PROP;

    clear_screen();
    print_string("Part 2");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }

    clear_screen();
    print_string("Proport");
    lcd_cursor(0, 1);
    print_string("ional");

    //Button Press Delay before start
    delay_ms(200);

    while (1){

        // Check for Button Press and change 
        // Modes and Display
        mode = set_mode(mode, &flag_iterate);

        // Main Loop
        switch(mode){
            case MODE_PROP:
                // Proportional Control
                line_data = read_line_sensor();
                motors = compute_proportional(line_data.left, line_data.right);
                set_motors(motors);

                // Loop Delay
                delay_ms(ERROR_CHECK_DELAY);

                break;
            case MODE_DATA:
                // Record Data
                cache[data_counter] = read_line_sensor();
                print_data(cache[data_counter], data_counter);
                data_counter++;
                
                // Check if cache is full
                if(data_counter >= CACHE_SIZE){
                    mode = MODE_TRAINING;
                    halt();
                    clear_screen();
                    print_string("Training");
                }
                // Loop Delay
                delay_ms(DELAY_DATA_COLLECTION);

                break;
            case MODE_TRAINING:
                // Train that bumbleboy!!!
                // Wait for button press to determine training iterations
                if(flag_iterate == 0){
                    delay_ms(200);
                    training_iteration_count = 0;
                    while((get_btn() == 0) && (get_btn2() == 0)){
                        print_training(training_iteration_count);
                        training_iteration_count++;
                        delay_ms(DELAY_TRAINING_INCREASE);
                    }
                    delay_ms(500);
                    flag_iterate = 1;
                }
                if(training_iteration_count >= 0){
                    current_data_counter = 0;
                    while(current_data_counter < data_counter - 1){
                        
                        line_data_t line = cache[current_data_counter];
                        motor_command_t motors1 = compute_proportional(line.left, line.right);
                        //print_training3(motors1.left, motors1.right);
                        //print_training2(current_data_counter);
                        train_net(line, &net, motors1);
                        
                        current_data_counter++;
                    }
                    delay_ms(100);
                    print_training(training_iteration_count);
                    training_iteration_count--;
                }else{
                    mode = MODE_NEURAL;
                    halt();
                    clear_screen();
                    print_string("Neural");
                }
                
                break;
            case MODE_NEURAL:
                line_data = read_line_sensor();
                motors = compute_neural_network(line_data, net);
                set_motors(motors);

                print_training4(motors.left, motors.right);
                // Loop Delay
                // delay_ms(DELAY_NEURAL);

                break;
            default:
                mode = MODE_PROP;
        }

    	// line_data = read_line_sensor();
     //    motors = compute_neural_network(line_data, net);
     //    set_motors(motors);

     //    print_4(line_data.left, motors.left, line_data.right, motors.right);
     //    delay_ms(DELAY_MS);
    }
    return 27;
}

// Switch Modes on Button Press
u08 set_mode(u08 mode, int *flag){
    u08 pressed = ((get_btn() == 1) | (get_btn2() == 1));
    if(pressed){
        switch (mode){
        case MODE_PROP:
            mode = MODE_DATA;
            halt();
            clear_screen();
            print_string("Data");
            print_num(0);
            break;
        case MODE_DATA:
            mode = MODE_TRAINING;
            halt();
            clear_screen();
            print_string("Training");
            *flag = 0;
            break;
        case MODE_TRAINING:
            mode = MODE_NEURAL;
            halt();
            clear_screen();
            print_string("Neural");
            break;
        case MODE_NEURAL:
            mode = MODE_TRAINING;
            halt();
            clear_screen();
            print_string("Training");
            *flag = 0;
            break;
        default:
            mode = MODE_PROP;
        }
        delay_ms(200);
    }
    return mode;
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
