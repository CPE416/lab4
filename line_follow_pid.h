// Description: PID Controller

#ifndef LINE_FOLLOW_PID_H
#define LINE_FOLLOW_PID_H

#include <stdio.h>

#include "defs.h"

#define THRESHOLD 200
#define BASE_MOTOR_SPEED 19
#define P_TERM 1
#define I_TERM 0
#define D_TERM 0
#define ERROR_CHECK_DELAY 50
#define HISTORY_LENGTH 8

typedef struct pid 
{ 
   int *error;
   u08 p_term;
   u08 i_term;
   u08 d_term;
}pid;

//Inserts most recent term and removes last term
void insert_error(pid *_pid, int _error){
	u08 i = HISTORY_LENGTH - 2;
	while(i < HISTORY_LENGTH - 1){
		_pid->error[i] = _pid->error[i+1];
		i--;
	}
	_pid->error[0] = _error;
}

//Calculates the derivative term based off past error values
int calc_derivative(pid *_pid){
	int derivative = 0;
	u08 i = HISTORY_LENGTH - 1;
	while(i < HISTORY_LENGTH - 1){
		derivative += _pid->error[i];
		i--;
	}
	derivative = derivative / HISTORY_LENGTH;
	return derivative;
}

// Takes in a PID struct, and a tuple of sensor values 
// sensor[0] = left, sensor[1] = right
// and calculates motor values 
// motors[0] = left, motors[1] = right
void pid_control(pid *_pid, int *sensor, int *motors){
	int actual = 0;
	int derivative;
	int output;
	
	int left_motor = BASE_MOTOR_SPEED;
	int right_motor = BASE_MOTOR_SPEED;
	//u08 right = poll_analog_pin(LINE_SENSOR_RIGHT);
	//u08 left = poll_analog_pin(LINE_SENSOR_LEFT);

	actual += sensor[1];
	actual -= sensor[0];

	insert_error(_pid, actual);

	output = _pid->error[0] * _pid->p_term;
	derivative = calc_derivative(_pid) * _pid->d_term;
	
	left_motor += output;
	right_motor -= output;

	left_motor += derivative;
	right_motor -= derivative;

	//print_4(left, left_motor, right, right_motor);

	//set_motors(left_motor, right_motor);
	motors[1] = right_motor;
	motors[0] = left_motor;
}


//takes in sensor values and returns a motor command
motor_command_t compute_proportional(u08 left, u08 right){
	motor_command_t motor;
	
	int actual = right - left;

	int output = actual * P_TERM;

	int left_motor = BASE_MOTOR_SPEED + output;
	int right_motor = BASE_MOTOR_SPEED - output;

	if(left_motor < 0){
		left_motor = 0;
	}
	if(right_motor < 0){
		right_motor = 0;
	}
	if(left_motor > 100){
		left_motor = 100;
	}
	if(right_motor > 100){
		right_motor = 100;
	}

	motor.left = left_motor;
	motor.right = right_motor;

	return motor;
}

// // Prints some values from a PID struct to the LCD
// void print_pid(struct pid _pid){
// 	print_4(_pid.error[0], _pid.p_term, _pid.i_term, _pid.d_term);
// }

#endif

