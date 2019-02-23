#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"


void localize(block_layout_t layout, particle_t *particle_array, int movement_distance, u08 proximity){
    run_motion_model(particle_array, movement_distance);
    u08 robot_has_block = prox_has_block(proximity);

    recalculate_weights(layout, particle_array, robot_has_block);
    resample_particles(layout, particle_array);
}

#endif 