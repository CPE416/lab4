#ifndef MONTE_H
#define MONTE_H

#include "defs.h"
#include "particle.h"
#include "monte.h"
#include "kill.h"

void localize(){
    particle_t particle_array[NUM_PARTICLES];
    init_particle_array(particle_array, NUM_PARTICLES);

    run_motion_model(particle_array, ticks);

    u08 distnace = read_prox_sensor();
    u08 robot_has_block = prox_has_block(distance);

    recalculate_weights(particle_array, robot_has_block);

    
}

#endif 