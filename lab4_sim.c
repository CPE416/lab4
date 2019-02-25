// Name:  Jared Peter-Contesse & Spencer Shaw
// Simulator
// Description: 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "block_layout.h"
#include "sim.h"

#define RAND_SEED (time(NULL))

#define STANDARD_DEVIATION_THRESHHOLD (50.0)
#define MAX_ITERATIONS (5)
#define THETA STANDARD_DEVIATION_THRESHHOLD

#define STARTING_LOCATION (0)
#define MOVEMENT_TICKS (20.0)

int check_args (int argc, char *argv[], block_layout_t *block);

int main(int argc, char *argv[]){
	srand(RAND_SEED);

	block_layout_t layout;

	int check = check_args(argc,argv, &layout);
	if(check == -1){
		return 0;
	}
	print_block_art(layout);

	sim_input_t sim_input;
	sim_input.layout = layout;
	sim_input.max_iterations = MAX_ITERATIONS;
	sim_input.movement_ticks = MOVEMENT_TICKS;
	sim_input.starting_location = STARTING_LOCATION;
	sim_input.std_dev_threshhold = STANDARD_DEVIATION_THRESHHOLD;

	sim_output_t sim_output = simulate(sim_input);

	if(is_success(sim_output, THETA)){
		printf("SUCCESS:\tstd dev: %4.1f, Guess: %4.1f, actual: %4.1f, iterations: %d\n",
			   sim_output.std_dev, sim_output.end_guess, sim_output.location, sim_output.iterations);
	}
}

/* checks for correct number of arguments */
int check_args (int argc, char *argv[], block_layout_t *block){
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


