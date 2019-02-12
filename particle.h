#ifndef PARTICLE_H
#define PARTICLE_H

// #include <math.h>
#include <stdlib.h>
#include <time.h>

#define RAND_SEED (time(NULL))

#define random (rand() / RAND_MAX)

typedef struct {
    float weight;
}particle_t;



#endif