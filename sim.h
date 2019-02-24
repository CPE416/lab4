#ifndef SIM_H
#define SIM_H

#include <stdio.h>

#include "block_layout.h"
#include "particle.h"

float increment_location(float location, float degrees){
    float new_location = location + degrees;
    if (new_location > 360.0){
        new_location -= 360.0;
    }
    return new_location;
}

float compute_std_deviation(particle_t *particle_array, float *avg){
    float sum = 0;
    float std_dev = 0;
    for (int i = 0; i < NUM_PARTICLES; i++){
        sum += particle_array[i].position;
    }
    float mean = sum / NUM_PARTICLES;
    for(int j = 0; j < NUM_PARTICLES; j++){ 
        std_dev += pow(particle_array[j].position - mean, 2);
    }
    *avg = mean;
    return sqrt(std_dev/NUM_PARTICLES);
}

#endif
