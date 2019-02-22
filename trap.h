#ifndef TRAP_H
#define TRAP_H

// Probability Functions for block
#define BLOCK_VAL_A (50.0)
#define BLOCK_VAL_B (90.0)
#define BLOCK_VAL_C (250.0)
#define BLOCK_VAL_D (255.0)

// Probability Function for Space
#define SPACE_VAL_A (0.0)
#define SPACE_VAL_B (1.0)
#define SPACE_VAL_C (40.0)
#define SPACE_VAL_D (60.0)

// Struct and Functions for the Trapezoid Probability Density Function
typedef struct {
	float a;
	float b;
	float c;
	float d;
	float width;
	float max_height;
} trap_prob_t;

float calc_particle_width(trap_prob_t trap, const int num_particles){
	float particle_width = trap.width / (float) num_particles;
	// printf("Particle width: %f from trap width %f\n", particle_width, trap.width);
	return  particle_width; 
}

float calc_trap(trap_prob_t trap, float x, int num_particles){
	float particle_width = calc_particle_width(trap, num_particles);
	printf("Max trap height: %f, particle_width: %f\n", trap.max_height, particle_width);
	if(x < trap.a){
		return 0.0;
	}else if(x < trap.b){
		float slope = (x - trap.a)/(trap.b - trap.a);
		return (trap.max_height * slope * particle_width);
	}else if(x < trap.c){
		return trap.max_height * particle_width;
	}else if(x < trap.d){
		float slope = (trap.d - x)/(trap.d - trap.c);
		return (trap.max_height * slope * particle_width);
	}else{
		return 0.0;
	}
}

trap_prob_t block_trap(){
    trap_prob_t trap;
    trap.a = BLOCK_VAL_A;
    trap.b = BLOCK_VAL_B;
    trap.c = BLOCK_VAL_C;
    trap.d = BLOCK_VAL_D;
	trap.width =  trap.d - trap.a;
	trap.max_height = 2.0 / (trap.width + trap.c - trap.b);
    return trap;
}

trap_prob_t space_trap(){
    trap_prob_t trap;
    trap.a = SPACE_VAL_A;
    trap.b = SPACE_VAL_B;
    trap.c = SPACE_VAL_C;
    trap.d = SPACE_VAL_D;
	trap.width =  trap.d - trap.a;
	trap.max_height = 2.0 / (trap.width + trap.c - trap.b);
    return trap;
}

#endif