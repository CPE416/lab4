// Name:  Jared Peter-Contesse & Spencer Shaw
// Description: Hardware Test Program

#include <avr/io.h>
#include <avr/interrupt.h>

// Custum delay lib
#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"

#define PHOTO_DIODE_RIGHT 0
#define PHOTO_DIODE_LEFT 1
#define LINE_SENSOR_RIGHT 3
#define LINE_SENSOR_LEFT 2

void poll_linesensor(u08 pin_num);
void poll_photodiode(u08 pin_num);
void print_data(u08 count, u08 vals);

int main(void)
{
	init();
    motor(0, 0);
    motor(1, 0);
    
    int _error[HISTORY_LENGTH] = {0};	
	struct pid _pid = {_error, P_TERM, I_TERM, D_TERM};
	int _sensors[2] = {0,0};
	int _motors[2] = {0,0};
	motor_command_t motors;

	while((get_btn() == 0) && (get_btn2() == 0)){
		delay_ms(1);
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
		_sensors[1] = poll_analog_pin(LINE_SENSOR_RIGHT);
		_sensors[0] = poll_analog_pin(LINE_SENSOR_LEFT);

		pid_control(&_pid, &_sensors[0], &_motors[0]);

		motors.right = _motors[1];
		motors.left = _motors[0];

		print_4(_sensors[0], motors.left, _sensors[1], motors.right);

		set_motors(motors);

		delay_ms(ERROR_CHECK_DELAY);

	}
}

void poll_photodiode(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void poll_linesensor(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void print_data(u08 count, u08 vals){
	//print_num(count);
	print_string(":");
	print_num(vals);
}