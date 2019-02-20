// Name:  Jared Peter-Contesse & Spencer Shaw
// Simulator
// Description: 

#include <stdio.h>

// Custum delay lib
#include "hardware.h"
#include "line_follow_pid.h"
#include "monte.h"
#include "kill.h"

#define PARTICLE_NUM 100
#define STANDARD_DEVIATION 1

typedef struct {
	float a;
	float b;
	float c;
	float d;
} trap_prob_t;

trap_prob_t init_trap_prob(float a, float b, float c, float d){
	trap_prob_t trap;
    trap.a = a;
    trap.b = b;
    trap.c = c;
    trap.d = d;
    return trap;
}

float calc_prob(trap_prob_t trap, float x){
	float u = 2/(trap.d + trap.c - trap.b - trap.a);
	if(x < trap.b){
		float b
	}else if(x < trap.c){

	}else{

	}
}

int main(void){

}
