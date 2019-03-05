// number of blocks (from 3 to 5)
// block locations (counterclockwise in order)
// target block (block number to knock over)



#ifndef BLOCK_LAYOUT_H
#define BLOCK_LAYOUT_H

#include <math.h>

#include "defs.h"

#define ASCII_POS_1 (5)
#define ASCII_POS_2 (6)
#define ASCII_POS_3 (8)
#define ASCII_POS_4 (11)
#define ASCII_POS_5 (12)

#define B ('B')

#define MAX_BLOCKS (5)
#define BLOCK_FUDGE_FACTOR (15.0)

typedef struct { 
    u08 num_blocks;             // 3 to 5 blocks
    u08 target_block;           // 1 to num_blocks position
    int block_locations[5];   // 
} block_layout_t;

block_layout_t generate_layout(const u08 num_blocks, float *block_locations, const u08 target_block){
    block_layout_t l;
    l.num_blocks = num_blocks;

    for  (int i = 0; i < num_blocks; i++){
        l.block_locations[i] = block_locations[i];
    }

    l.target_block = target_block;
    return l;
}

u08 is_block(block_layout_t layout, const float location){
    float low_end = location - BLOCK_FUDGE_FACTOR;
    float high_end = location + BLOCK_FUDGE_FACTOR;
    for (int i = 0; i < layout.num_blocks; i++){
        if (layout.block_locations[i] > low_end && layout.block_locations[i] < high_end){
            return TRUE;
        }
    }
    return FALSE;
}

float distance_to_block(block_layout_t layout, float location){
    float low_end = location - BLOCK_FUDGE_FACTOR;
    float high_end = location + BLOCK_FUDGE_FACTOR;
    for (int i = 0; i < layout.num_blocks; i++){
        if (layout.block_locations[i] > low_end && layout.block_locations[i] < high_end){
            return abs(layout.block_locations[i] - location);
        }
    }
    return FALSE;
}

void print_block_layout(block_layout_t *block){
	printf("\n Block Count: %i", block->num_blocks);
	printf("\n Target Block: %i", block->target_block);
	for(int i = 0; i < block->num_blocks; i++){
		printf("\n Block Position %i: %d", (i+1), block->block_locations[i]);
	}
	printf("\n\n");
}

void print_block_art(block_layout_t layout){
    char c90char = '_';
    char c45char = '\\';
    char c0char = '|';
    char c135char = '/';
    char c180char = '|';
    char c225char = '\\';
    char c270char = '_';
    char c315char = '/';

    for(int i = 0; i < layout.num_blocks; i++){
        int location = layout.block_locations[i];
        switch(location){
            case 0:
                c0char = 'B';
                break;
            case 45:
                c45char = 'B';
                break;
            case 90:
                c90char = 'B';
                break;
            case 135:
                c135char = 'B';
                break;
            case 180:
                c180char = 'B';
                break;
            case 225:
                c225char = 'B';
                break;
            case 270:
                c270char = 'B';
                break;
            case 315:
                c315char = 'B';
                break;
            default:
                break;
        }
    }
    printf("        90        \n");
    printf("   135  %c  45    \n", c90char);
    printf("      %c   %c    \n", c135char, c45char);
    printf(" 180 %c     %c 0 \n", c180char, c0char);
    printf("      %c %c %c    \n", c225char, c270char, c315char);
    printf("   225      315   \n");
    printf("       270     \n");
    
    printf("\n");
}

#endif