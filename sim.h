#ifndef SIM_H
#define SIM_H

#include <stdio.h>

#include "block_layout.h"
#include "monte.h"

float increment_location(float location, int ticks){
    float new_location = location + ticks;
    if (new_location > 360.0){
        new_location -= 360.0;
    }
    return new_location;
}

int read_prox_sensor(block_layout_t layout, float location){
	// for (int i = 0; i < layout.num_blocks; i++){
	// 	if ((x > (layout.block_locations[i] - BLOCK_FUDGE_FACTOR)) && 
	// 	    ( x < (layout.block_locations[i] + BLOCK_FUDGE_FACTOR))){
	// 			return TRUE;
	// 	}
	// }
	// return FALSE;
	u08 sensor_val = is_block(layout, location) * RAND * 255;
	printf("Generated sensor value: %d   @ location: %5.1f\n", sensor_val, location);
	return sensor_val;
}

#endif
