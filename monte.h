#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"

#define TICKS (10)
// Probability Functions for block
#define BLOCK_VAL_A 50
#define BLOCK_VAL_B 90
#define BLOCK_VAL_C 250
#define BLOCK_VAL_D 255

// Probability Function for Space
#define SPACE_VAL_A 0
#define SPACE_VAL_B 1
#define SPACE_VAL_C 40
#define SPACE_VAL_D 60


void localize(block_layout_t layout, u08 distance){

    particle_t particle_array[NUM_PARTICLES];

    init_particle_array(particle_array, NUM_PARTICLES);
    // move_robot(TICKS);
    run_motion_model(particle_array, TICKS);

    u08 robot_has_block = prox_has_block(distance);

    recalculate_weights(layout, particle_array, robot_has_block);

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