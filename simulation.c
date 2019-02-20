// Name:  Jared Peter-Contesse & Spencer Shaw
// Simulator
// Description: 

#include <stdio.h>

// Custum delay lib
//#include "hardware.h"
//#include "line_follow_pid.h"
#include "monte.h"
// #include "kill.h"

#define PARTICLE_NUM 100
#define STANDARD_DEVIATION 1

int checkarg (int argc, char *argv[]);

int main(int argc, char *argv[]){
	int check = checkarg(argc,argv);
	if(check == -1){
		return 0;
	}
}

/* checks for correct number of arguments */
int checkarg (int argc, char *argv[]){
	if(argc < 6){
		printf("usage: simulation [block num 3-5] [target block 0-block num] 
				[block positon 1] ... [block positon block num]\n");
		return -1;
	}
		if(atoi(argv[2])){
			print_top_val = atoi(argv[2]);
			currentarg = 3;
			return 0;
		}else{
			printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			return -1;
		}
}