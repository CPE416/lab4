#ifndef PARTICLE_H
#define PARTICLE_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM (rand() / RAND_MAX)

typedef struct {
    float position;
}particle_t;

particle_t generate_particle(){
    particle_t p;
    p.position = RANDOM;
    return p;
}


void init_particle_array(particle_t *particle_array, const int num_particles){
    for (int i = 0; i < num_particles; i++){
        particle_array[i] = generate_particle();
    }
}


#endif