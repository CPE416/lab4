#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"
#include "monte.h"

void localize(){
    particle_t particle_array[NUM_PARTICLES];
    init_particle_array(particle_array, NUM_PARTICLES);

    run_motion_model(particle_array, ticks);

    u08 distnace = read_prox_sensor();
    u08 robot_has_block = prox_has_block(distance);

    recalculate_weights(particle_array, robot_has_block);

    
}

// Struct and Functions for the Trapezoid Probability Density Function
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
	if(x < trap.a){
		return 0;
	}else if(x < trap.b){
		float slope = (x - trap.a)/(trap.b - trap.a);
		return (u * slope);
	}else if(x < trap.c){
		return u;
	}else if(x < trap.d){
		float slope = (trap.d - x)/(trap.d - trap.c);
		return (u * slope);
	}else{
		return 0;
	}
}


#endif 