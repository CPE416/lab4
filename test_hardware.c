// Name:  Jared Peter-Contesse & Spencer Shaw
// Description: Hardware Test Program

#include <avr/io.h>
#include <avr/interrupt.h>

// Custom libs
#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "globals.h"
#include "encoder.h"

#define PHOTO_DIODE_RIGHT 0
#define PHOTO_DIODE_LEFT 1
#define LINE_SENSOR_RIGHT 3
#define LINE_SENSOR_LEFT 2
#define DISTANCE_SENSOR 5
#define ENCODER_SENSOR 4

void poll_linesensor(u08 pin_num);
void poll_photodiode(u08 pin_num);
void print_data(u08 count, u08 vals);
void poll_distance(u08 pin_num);
void poll_encoder(u08 pin_num);
void print_distance_encoders(int sensor, int count1, int count2);

int main(void)
{
	init();
    motor(0, 0);
    motor(1, 0);

    
    
 //    int _error[HISTORY_LENGTH] = {0};	
	// struct pid _pid = {_error, P_TERM, I_TERM, D_TERM};
	// int _sensors[2] = {0,0};
	// int _motors[2] = {0,0};
	// motor_command_t motors;

    clear_screen();
    print_string("Test");

	while((get_btn() == 0) && (get_btn2() == 0)){
		delay_ms(1);
	}

	// Test Encoders Forward
	// init_encoder();
	// while(1){
	// 	forward(50);
	// 	delay_ms(500);
	// }

	// Test Encoders Turn
	// init_encoder();
	// while(1){
	// 	rotate_90();
	// 	delay_ms(2000);
	// }

	// Test Distance Sensor
	// while(1){
	// 	poll_distance(DISTANCE_SENSOR);
	// 	delay_ms(200);
	// }

	// test Encoder Sensor with PID
	// motor_command_t motors;
	// line_data_t line_data;
	// init_encoder();

	// delay_ms(500);
	// while(1){
	// 	print_encoders();
	// 	line_data = read_line_sensor();
	// 	motors = compute_proportional(line_data.left, line_data.right);
	// 	set_motors(motors);
	// 	if((get_btn() == 1) || (get_btn2() == 1)){
 	//    	halt();
 	//      delay_ms(500);
 	//      while((get_btn() == 0) && (get_btn2() == 0)){
 	//        	delay_ms(1);
 	//   	}
 	//  }
	// }

	// test Distance Sensor with PID
	motor_command_t motors;
	line_data_t line_data;
	init_encoder();

	delay_ms(500);
	while(1){
		
		u08 dist_sensor = analog(DISTANCE_SENSOR);
		print_distance_encoders(dist_sensor, left_encoder, right_encoder);
		line_data = read_line_sensor();
		motors = compute_proportional(line_data.left, line_data.right);
		set_motors(motors);
		if((get_btn() == 1) || (get_btn2() == 1)){
 	   	halt();
 	     delay_ms(500);
 	     while((get_btn() == 0) && (get_btn2() == 0)){
 	       	delay_ms(1);
 	  	}
 	 }
	}
	

	while(1){
		// clear_screen();
		// lcd_cursor(0, 0);
		// poll_photodiode(PHOTO_DIODE_RIGHT);
		// poll_photodiode(PHOTO_DIODE_LEFT);
		// lcd_cursor(0, 1);
		// poll_linesensor(LINE_SENSOR_RIGHT);
		// poll_linesensor(LINE_SENSOR_LEFT);
		// delay_ms(500);
		// _sensors[1] = poll_analog_pin(LINE_SENSOR_RIGHT);
		// _sensors[0] = poll_analog_pin(LINE_SENSOR_LEFT);

		// pid_control(&_pid, &_sensors[0], &_motors[0]);

		// motors.right = _motors[1];
		// motors.left = _motors[0];

		// print_4(_sensors[0], motors.left, _sensors[1], motors.right);

		// set_motors(motors);

		// delay_ms(ERROR_CHECK_DELAY);

		

		

	}
}

void poll_photodiode(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void poll_linesensor(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void print_data(u08 count, u08 vals){
	clear_screen();
	print_num(count);
	print_string(":");
	print_num(vals);
}

void poll_distance(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void poll_encoder(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void print_distance_encoders(int sensor, int count1, int count2){
    clear_screen();
    print_num(sensor);
    lcd_cursor(0, 1);
    print_num(count1);
    lcd_cursor(4, 1);
    print_num(count2);
}
