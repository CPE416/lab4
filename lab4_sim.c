// Name:  Jared Peter-Contesse & Spencer Shaw
// Simulator
// Description: 

#include <stdio.h>

#include "monte.h"
#include "defs.h"
#include "block_layout.h"

#define PARTICLE_NUM 100
#define STANDARD_DEVIATION 1

int checkarg (int argc, char *argv[], block_layout_t *block);
int read_prox_sensor(block_layout_t layout, float x);

int main(int argc, char *argv[]){

	block_layout_t block;

	int check = checkarg(argc,argv, &block);
	if(check == -1){
		return 0;
	}


	float location = 0;
    u08 distance = read_prox_sensor(block, location);
	localize(block, distance);
}

/* checks for correct number of arguments */
int checkarg (int argc, char *argv[], block_layout_t *block){
	//Check argument length
	if(argc < 6){
		printf("usage: simulation [block num (3-5)] [target block (1-block num)]\
				[block positon (1)] ... [block positon (block num)]\n");
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
			|| ((int) block->block_locations[i] % 45) != 0){
			printf("usage: block position must be between 0 and 315 in multiples of 45");
			return -1;
		}
	}
	print_block_layout(block);
	return 1;
}

int read_prox_sensor(block_layout_t layout, float x){
	for (int i = 0; i < layout.num_blocks; i++){
		if ((x > (layout.block_locations[i] - BLOCK_FUDGE_FACTOR)) && 
		    ( x < (layout.block_locations[i] + BLOCK_FUDGE_FACTOR))){
				return TRUE;
		}
	}
	return FALSE;
}
