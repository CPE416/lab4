// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 4 Part 2
// Description: 

// Full Ring Encoder Values:
// Right: 480
// Left: 240

// Block Length
// Right 40
// Left 20

#include <stdio.h>

#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "block_layout.h"

// Settings
#define DELAY_MS 100 // Delay time for control loop
#define DRIVE_FOR_ENCODER_COUNT 15

u08 set_mode(u08 mode, int *flag);
void print_data(line_data_t sensor, int count);
void print_training(int count);
void print_training2(int count);
void print_training3(int count1, int count2);
void print_training4(int count1, int count2);

void read_accel(u08 *horizontal);
void print_target_block(u08 num);
void print_num_block(u08 num);
void print_block_info(block_layout_t layout);
void print_block_positions(block_layout_t layout);
void print_position_block(int num, int count);
u08 get_target_block(block_layout_t layout);
u08 get_num_block();
int get_position();

int main(void)
{
    init();

    // Variables
    block_layout_t layout;

    //Particle Variables
    srand(RAND_SEED);
    particle_t particle_array[NUM_PARTICLES];
    init_particle_array(particle_array);
    
    //line_data_t line_data;
    //motor_command_t motors;
    //motors.left = 0;
    //motors.right = 0;
    halt();


    clear_screen();
    print_string("Part 2");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }
    //Button Press Delay before start
    delay_ms(500);

    //Choose Number of Blocks
    layout.num_blocks = get_num_block();

    //Button Press Delay before start
    delay_ms(500);

    //Choose Target Block
    layout.target_block = get_target_block(layout);

    //Button Press Delay before start
    delay_ms(500);

    //Get Block Positions
    for(int i = 0; i < layout.num_blocks; i++){
        layout.block_locations[i] = get_position(i);
        delay_ms(500);
    }

    //Print User input
    print_block_info(layout);
    delay_ms(1000);
    print_block_positions(layout);
    delay_ms(1000);

    //Loop until finished
    while (1){

    }
}



//Choose Number of Blocks
u08 get_num_block(){
    u08 block_num = 3;
    u08 accel_data = 0;
    while((get_btn() == 0) && (get_btn2() == 0)){
        print_num_block(block_num);
        read_accel(&accel_data);
        if(accel_data < 245){
            if(accel_data > 128){
                if(block_num < 5){
                    block_num++;
                }
            }else if(accel_data > 10){
                if(block_num > 3){
                    block_num--;
                }
            }
        }
        delay_ms(500);
    }
    return block_num;
}

//Choose Target Blocks
u08 get_target_block(block_layout_t layout){
    u08 block_num = 1;
    u08 accel_data = 0;
    while((get_btn() == 0) && (get_btn2() == 0)){
        print_target_block(block_num);
        read_accel(&accel_data);
        if(accel_data < 245){
            if(accel_data > 128){
                if(block_num < layout.num_blocks){
                    block_num++;
                }
            }else if(accel_data > 10){
                if(block_num > 1){
                    block_num--;
                }
            }
        }
        delay_ms(500);
    }
    return block_num;
}

//Choose Block Positions
int get_position(int i){
    int position_num = 0;
    u08 accel_data = 0;
    
    while((get_btn() == 0) && (get_btn2() == 0)){
        print_position_block(position_num, i+1);
        read_accel(&accel_data);
        if(accel_data < 245){
            if(accel_data > 128){
                if(position_num < 315){
                    position_num += 45;
                }
            }else if(accel_data > 10){
                if(position_num > 0){
                    position_num -= 45;
                }
            }
        }
        delay_ms(500);
    }
    return position_num;
}

void print_num_block(u08 num){
    clear_screen();
    print_string("BlockNum");
    lcd_cursor(0, 1);
    print_num(num);
}

void print_target_block(u08 num){
    clear_screen();
    print_string("Target");
    lcd_cursor(0, 1);
    print_num(num);
}
void print_position_block(int num, int count){
    clear_screen();
    print_string("Block ");
    print_num(count);
    print_string(":");
    lcd_cursor(0, 1);
    print_num(num);
}

void print_block_info(block_layout_t layout){
    clear_screen();
    print_string("Blocks:");
    print_num(layout.num_blocks);
    lcd_cursor(0, 1);
    print_string("Target:");
    print_num(layout.target_block);
}

void print_block_positions(block_layout_t layout){
    clear_screen();
    for (int i = 0; i < layout.num_blocks; i++){
        if (i == 2){
            lcd_cursor(0, 1);
        }
        print_num(layout.block_locations[i]);
        print_string("-");
    }
}

void read_accel(u08 *horizontal)
{
    // Directions when looking at the screen
    horizontal[0] = get_accel_y(); // Left and right
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
