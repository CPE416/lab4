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
#include <math.h>
#include <time.h>

#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "block_layout.h"
#include "encoder.h"
#include "particle.h"

// Settings
#define DELAY_MS (100) // Delay time for control loop
// #define DRIVE_FOR_ENCODER_COUNT (15)
#define MOVEMENT_TICKS (8)

#define DISTANCE_SENSOR (5)

#define STD_DEVIATION_THRESHOLD (30.0)
#define RAND_SEED (10)


u08 set_mode(u08 mode, int *flag);

void read_accel(u08 *horizontal);
void print_target_block(u08 num);
void print_num_block(u08 num);
void print_block_info(block_layout_t layout);
void print_block_positions(block_layout_t layout);
void print_position_block(int num, int count);

void move_distance_on_line(int ticks);
float calc_distance_from_target(block_layout_t layout, float average_position);
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
    float std_dev;
    float average_position;
    int total_ticks = 0;
    init_encoder();
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
    delay_ms(2000);
    print_block_positions(layout);
    delay_ms(2000);

    clear_screen();
    print_string("Press 2");
    lcd_cursor(0, 1);
    print_string("Start");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }

    //Loop until finished
    while (1){
        move_distance_on_line(MOVEMENT_TICKS);

        total_ticks += MOVEMENT_TICKS;
        float movement_degrees = ticks_to_degrees(MOVEMENT_TICKS);
        run_motion_model(particle_array, movement_degrees);
        
        u08 prox_reading = analog(DISTANCE_SENSOR);
        recalculate_weights(layout, particle_array, prox_reading);
        resample_particles(layout, particle_array);

        std_dev = compute_std_deviation(particle_array, &average_position);
        print_std_dev_pos(std_dev, average_position);

        if((total_ticks > FULL_RING_ENCODER_COUNT) && (std_dev < STD_DEVIATION_THRESHOLD)){
            float target_distance = calc_distance_from_target(layout, average_position);
            float encoder_distance = degrees_to_ticks(target_distance);
            
            clear_screen();
            print_string("Distance:");
            lcd_cursor(0, 1);
            print_num(encoder_distance);

            move_distance_on_line(encoder_distance);
            rotate_90();
            forward(100);
            halt();
            return 0;
        }
    }
}

void move_distance_on_line(int ticks){
    //Motor Variables
    motor_command_t motors;
    line_data_t line_data;

    while(right_encoder < ticks){ 
        line_data = read_line_sensor();
        motors = compute_proportional(line_data.left, line_data.right);
        set_motors(motors);
    }
    halt();
    reset_encoders();
}

float calc_distance_from_target(block_layout_t layout, float average_position){
    float target_distance;
    float target_position = layout.block_locations[layout.target_block - 1];
    if(average_position > target_position){
        target_distance = 360 - average_position + target_position;
    }else{
        target_distance = target_position - average_position;
    }
    return target_distance;
}

// void drive_for_encoder(line_data_t line_data, motor_command_t motors, float encoder_count){
//     while(right_encoder < encoder_count){
//         line_data = read_line_sensor();
//         motors = compute_proportional(line_data.left, line_data.right);
//         set_motors(motors);
//     }
//     halt();
//     reset_encoders();
// }  

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
