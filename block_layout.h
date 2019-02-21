// number of blocks (from 3 to 5)
// block locations (counterclockwise in order)
// target block (block number to knock over)



#ifndef BLOCK_LAYOUT_H
#define BLOCK_LAYOUT_H

#include "defs.h"

#define ASCII_POS_1 (5)
#define ASCII_POS_2 (6)
#define ASCII_POS_3 (8)
#define ASCII_POS_4 (11)
#define ASCII_POS_5 (12)

#define B ('B')

#define MAX_BLOCKS (5)
#define BLOCK_FUDGE_FACTOR (10.56)

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

void print_block_layout(block_layout_t *block){
	printf("\n Block Count: %i", block->num_blocks);
	printf("\n Target Block: %i", block->target_block);
	for(int i = 0; i < block->num_blocks; i++){
		printf("\n Block Position %i: %d", (i+1), block->block_locations[i]);
	}
	printf("\n\n");
}

void print_block_art(block_layout_t layout){
    char *row0 = "       90     \n";
    char *row1 = "  135  __  45 \n";
    char *row2 = "     /    \\   \n";
    char *row3 = "180 |      | 0\n";
    char *row4 = "     \\ __ /   \n";
    char *row5 = "  225      315\n";
    char *row6 = "       270    \n";
    for (int i = 0; i < layout.num_blocks; i++){
        switch (layout.block_locations[i]){
            case 0:
                row3[ASCII_POS_5] = B;
                break;
            case 45:
                row2[ASCII_POS_4] = B;
             case 90:
                row1[ASCII_POS_3] = B;
                break;
            case 135:
                row2[ASCII_POS_2] = B;
            case 180:
                row3[ASCII_POS_1] = B;
                break;
            case 225:
                row4[ASCII_POS_2] = B;
            case 270:
                row5[ASCII_POS_3] = B;
                break;
            case 315:
                row4[ASCII_POS_4] = B;
                break;
            default:
                printf("Could not read block locations to print.\nExiting.\n");
        }
    }
    printf("%s%s%s%s%s%s%s", row0, row1, row2, row3, row4, row5, row6);
}

#endif