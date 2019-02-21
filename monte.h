#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"

#define TICKS (10)



void localize(block_layout_t layout, u08 distance){

    particle_t particle_array[NUM_PARTICLES];

    init_particle_array(particle_array, NUM_PARTICLES);
    // move_robot(TICKS);
    run_motion_model(particle_array, TICKS);

    u08 robot_has_block = prox_has_block(distance);

    recalculate_weights(layout, particle_array, robot_has_block);

}



#endif 