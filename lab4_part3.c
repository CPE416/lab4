// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 4 Part 3
// Description: 


// Custum delay lib
#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "neural.h"

// Settings
#define DELAY_MS 200 // Delay time for loop
#define DELAY_LOOP 500

#define LEARNING_RATE 0.8

#define MODE_PROP 0
#define MODE_NEURAL 1

void process_data_mode_a(u08* vals, u08* data);
void process_data_mode_b(u08* vals, u08* data);
u08 map_light_to_motor(u08 data);
u08 set_mode(u08 mode);
void print_neural(int count);
void print_prop(int count);
void print_training4(int count1, int count2, int count3, int count4);


int main(void)
{
    // Init board
    init();

    // Loop variables
    line_data_t line_data;
    motor_command_t motors;
    motor_command_t motors1;
    int data_counter = 0;

    halt();

    neural_net_t net;
    init_net(&net, LEARNING_RATE);

    u08 mode = MODE_PROP;

    clear_screen();
    print_string("Part 3");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }

    clear_screen();
    print_string("Prop");

    // Delay before starting state machine
    delay_ms(200);
    // Main loop
    while (1)
    {
        mode = set_mode(mode);

        // Print Mode and training iteration count
        // if(mode == MODE_PROP){
        //     print_prop(data_counter);
        // }else{
        //     print_neural(data_counter);
        // }

        // Read line data
        line_data = read_line_sensor();
        // Compute Proportional
        motors = compute_proportional(line_data.left, line_data.right);
        // Train Net
        train_net(line_data, &net, motors);
        // Count of iterations
        data_counter++;
        
        // Control motor based on mode
        switch (mode){
            case MODE_PROP:
                net.learning_rate = LEARNING_RATE;
                set_motors(motors);
                break;
            case MODE_NEURAL:   
                net.learning_rate = 0.0005;
                motors1 = compute_neural_network(line_data, net);
                set_motors(motors1);
                break;
        }
        print_training4(motors.left, motors.right, motors1.left, motors1.right);

        // Delay
        // delay_us(DELAY_LOOP);
    }
    return 1;
}


u08 set_mode(u08 mode){
    u08 pressed = ((get_btn() == 1) | (get_btn2() == 1));
    if(pressed){
        switch (mode){
            case MODE_PROP:
                mode = MODE_NEURAL;
                break;
            case MODE_NEURAL:
                mode = MODE_PROP;
                break;
            default:
                mode = MODE_PROP;
        }
        delay_ms(200);
    }
    return mode;
}

void print_prop(int count){
    if(!(count % 10 == 0)){
        return;
    }
    clear_screen();
    lcd_cursor(0, 0);
    print_string("Prop");
    lcd_cursor(0, 1);
    print_num(count);
}

void print_neural(int count){
    if(!(count % 10 == 0)){
        return;
    }
    clear_screen();
    lcd_cursor(0, 0);
    print_string("Neural");
    lcd_cursor(0, 1);
    print_num(count);
}

void print_training3(int count1, int count2){
    clear_screen();
    lcd_cursor(0, 0);
    print_string("Training");
    lcd_cursor(0, 1);
    print_num(count1);
    lcd_cursor(4, 1);
    print_num(count2);
}

void print_training4(int count1, int count2, int count3, int count4){
    clear_screen();
    lcd_cursor(0, 0);
    print_num(count1);
    lcd_cursor(4, 0);
    print_num(count2);
    lcd_cursor(0, 1);
    print_num(count3);
    lcd_cursor(4, 1);
    print_num(count4);
}

