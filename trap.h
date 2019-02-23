#ifndef TRAP_H
#define TRAP_H

// Probability Functions for block
#define BLOCK_VAL_A (50.0)
#define BLOCK_VAL_B (120.0) //(90.0)
#define BLOCK_VAL_C (210.0) //(250.0)
#define BLOCK_VAL_D (255.0)

// Probability Function for Space
#define SPACE_VAL_A (0.0)
#define SPACE_VAL_B (15.0) //(1.0)
#define SPACE_VAL_C (35) //(40.0)
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

float calc_trap(trap_prob_t trap, float x){
	if(x < trap.a){
		return 0.0;
	}else if(x < trap.b){
		float slope = (x - trap.a)/(trap.b - trap.a);
		return (trap.max_height * slope);
	}else if(x < trap.c){
		return trap.max_height;
	}else if(x < trap.d){
		float slope = (trap.d - x)/(trap.d - trap.c);
		return (trap.max_height * slope); 
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