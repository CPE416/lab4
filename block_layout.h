// number of blocks (from 3 to 5)
// block locations (counterclockwise in order)
// target block (block number to knock over)



#ifndef BLOCK_LAYOUT_H
#define BLOCK_LAYOUT_H

#include "defs.h"

#define MAX_BLOCKS (5)
#define BLOCK_FUDGE_FACTOR (10.56)

typedef struct { 
    u08 num_blocks;             // 3 to 5 blocks
    u08 target_block;           // 1 to num_blocks position
    float block_locations[5];   // 
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
        return FALSE;
    }
}

#endif