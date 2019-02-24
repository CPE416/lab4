#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"


void localize(block_layout_t layout, particle_t *particle_array, int movement_distance, u08 proximity){
    run_motion_model(particle_array, movement_distance);
    recalculate_weights(layout, particle_array, proximity);
    resample_particles2(layout, particle_array);
}

#endif 