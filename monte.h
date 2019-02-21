#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"

#define TICKS (10)



void localize(block_layout_t layout, particle_t *particle_array, u08 distance){

    // move_robot(TICKS);
    run_motion_model(particle_array, TICKS);

    u08 robot_has_block = prox_has_block(distance);

    recalculate_weights(layout, particle_array, robot_has_block);

}



#endif 