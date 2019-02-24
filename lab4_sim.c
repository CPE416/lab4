// Name:  Jared Peter-Contesse & Spencer Shaw
// Simulator
// Description: 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "block_layout.h"
#include "sim.h"
#include "particle.h"

#define RAND_SEED (time(NULL))

#define STANDARD_DEVIATION (1)
#define ITERATIONS (2)

#define STARTING_LOCATION (22.5)
#define MOVEMENT_TICKS (22.5)

int checkarg (int argc, char *argv[], block_layout_t *block);

int main(int argc, char *argv[]){
	srand(RAND_SEED);

	block_layout_t layout;

	int check = checkarg(argc,argv, &layout);
	if(check == -1){
		return 0;
	}

	float location = STARTING_LOCATION;

	print_block_art(layout);

    particle_t particle_array[NUM_PARTICLES];
    init_particle_array(particle_array);

	for (int i = 0; i < ITERATIONS; i++){
		u08 prox_reading = generate_prox_value(layout, location);
		printf("Read physical sensor value %d from location %4.1f\n", prox_reading, location);

		run_motion_model(particle_array, MOVEMENT_TICKS);
    	recalculate_weights(layout, particle_array, prox_reading);
    	resample_particles2(layout, particle_array);

		location = increment_location(location, MOVEMENT_TICKS);
	}
	print_particle_array(particle_array);
	printf("End location: %4.1f\n", location);
}

/* checks for correct number of arguments */
int checkarg (int argc, char *argv[], block_layout_t *block){
	//Check argument length
	if(argc < 6){
		printf("usage: simulation [block num (3-5)] [target block (1-block num)] [block positon (1)] ... [block positon (block num)]\n");
		return -1;
	}
	//Check block num number
	if((block->num_blocks = atoi(argv[1])) < 3 || block->num_blocks > 5){
		printf("usage: block num must be between 3 and 5");
		return -1;
	}
	if(argc != 3 + block->num_blocks){
		printf("usage: simulation [block num (3-5)] [target block (1-block num)]\
				[block positon (1)] ... [block positon (block num)]\n");
		return -1;
	}
	//Check target block number
	if((block->target_block = atoi(argv[2])) < 1 || block->target_block > block->num_blocks){
		printf("usage: target block must be between 1 and block num");
		return -1;
	}

	for(int i = 0; i < block->num_blocks; i++){
		if((block->block_locations[i] = atoi(argv[3 + i])) < 0 || block->block_locations[i] > 315 
			|| (block->block_locations[i] % 45) != 0){
			printf("usage: block position must be between 0 and 315 in multiples of 45");
			return -1;
		}
	}
	print_block_layout(block);
	return 1;
}


