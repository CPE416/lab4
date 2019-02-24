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

#define STANDARD_DEVIATION (2.0 * BLOCK_FUDGE_FACTOR)
#define MAX_ITERATIONS (100000)
#define NUM_SIMULATIONS (200)

#define STARTING_LOCATION (0)
#define MOVEMENT_TICKS (10)

int checkarg (int argc, char *argv[], block_layout_t *block);

int main(int argc, char *argv[]){
	srand(RAND_SEED);

	block_layout_t layout;

	int check = checkarg(argc,argv, &layout);
	if(check == -1){
		return 0;
	}


	print_block_art(layout);

    particle_t particle_array[NUM_PARTICLES];
    init_particle_array(particle_array);

	for (int i_sim = 0; i_sim < NUM_SIMULATIONS; i_sim++){
		float end_location = -1;
		float end_guess = -1;


		float location = STARTING_LOCATION;
		for (int i = 0; i < MAX_ITERATIONS; i++){
			u08 prox_reading = generate_prox_value(layout, location);
			// printf("Read physical sensor value %d from location %4.1f\n", prox_reading, location);

			run_motion_model(particle_array, MOVEMENT_TICKS);
			recalculate_weights(layout, particle_array, prox_reading);
			resample_particles(layout, particle_array);

			float avg = -1.0;
			float std_dev = compute_std_deviation(particle_array, &avg);
			if (std_dev < STANDARD_DEVIATION){
				//  Localitization complete
				// print_particle_array(particle_array);
				printf("\tLocalized at %4.1f after %d iterations with standard deviation of %3.1f\n", avg, i, std_dev);
				
				end_location = location;
				end_guess = avg;
				break;
			}
			if (i >= MAX_ITERATIONS - 1){
				// Reached max iterations
				print_particle_array(particle_array);
				printf("FAILED to localize after %d iterations with standard deviation of %3.1f for avg at %4.1f \n", i, std_dev, avg);
				
				end_location = location;
				end_guess = avg;
				break;
			}else{
				// Continue simulation
				// printf("Iteration: %d, continuing. Standard deviation of %3.1f for avg at %4.1f \n\n", i + 1, std_dev, avg);
				location = increment_location(location, MOVEMENT_TICKS);
			}
		}
	if (abs(end_guess - end_location) < STANDARD_DEVIATION){
		printf("SUCCESS: ");
	}
	else{
		printf("\t");
	}
	printf("Sim: %4d\tGuess: %4.1f, actual: %4.1f\n", i_sim + 1, end_guess, end_location);
	}
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


