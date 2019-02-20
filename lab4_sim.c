// Name:  Jared Peter-Contesse & Spencer Shaw
// Simulator
// Description: 

#include <stdio.h>

// Custum delay lib
//#include "hardware.h"
//#include "line_follow_pid.h"
#include "monte.h"
#include "defs.h"
#include "block_layout.h"
// #include "kill.h"

#define PARTICLE_NUM 100
#define STANDARD_DEVIATION 1

int checkarg (int argc, char *argv[], block_layout_t *block);

int main(int argc, char *argv[]){

	block_layout_t block;

	int check = checkarg(argc,argv, &block);
	if(check == -1){
		return 0;
	}
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
	//Check target block number
	if((block->target_block = atoi(argv[2])) < 1 || block->target_block > block->num_blocks){
		printf("usage: target block must be between 1 and block num");
		return -1;
	}
	for(int i = 0; i < block->num_blocks; i++){
		if((block->block_locations[i] = atoi(argv[i])))
	}
		if(atoi(argv[2])){
			
			currentarg = 3;
			return 0;
		}else{
			printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			return -1;
		}
}