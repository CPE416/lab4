#ifndef PARTICLE_H
#define PARTICLE_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "block_layout.h"
#include "trap.h"

#define DISTANCE_THRESHHOLD (60)

#define NUM_PARTICLES (50)
#define RATIO_KEEP_PARTICLES (0.95)

#define TICKS_PER_DEGREE (2.0)
#define RAND_SEED (time(NULL))
#define RAND ((1.0 * rand()) / RAND_MAX)
#define RANDOM_PARTICLE_POS ((360.0 * rand()) / RAND_MAX)

#include "sim.h"

typedef struct {
    float position;
    float weight;
}particle_t;

particle_t generate_particle(){
    particle_t p;
    p.position = RANDOM_PARTICLE_POS;
    p.weight = 0;

    // printf("Generated particle with position: %f\n", p.position);
    return p;
}

void init_particle_array(particle_t *particle_array){
    for (int i = 0; i < NUM_PARTICLES; i++){
        particle_array[i] = generate_particle();
    }
}

float ticks_to_degrees(int ticks){
    return ticks / TICKS_PER_DEGREE;
}

float generate_gaussian_value(){
    float u1 = RAND;
    float u2 = RAND;

    float root = sqrt(-2.0 * log(u1));
    float cosine = cos(2.0 * M_PI * u2);
    // printf("Boxie: u1: %f, u2: %f, root: %f, cosine: %f\n", u1, u2, root, cosine);
    return  root * cosine;
}

void run_motion_model(particle_t *particle_array, int ticks){
    for (int i = 0; i < NUM_PARTICLES; i++){
        float degrees = ticks_to_degrees(ticks);
        float noise = generate_gaussian_value();
        // printf("Moving particle from %5.1f by %f plus %f\n", particle_array[i].position, degrees, noise);
        particle_array[i].position += degrees + noise;
        if (particle_array[i].position > 360.0){
            particle_array[i].position -= 360.0;
        }
    }
    // printf("Ran motion model\n");
    // print_particle_array(particle_array);
}

u08 prox_has_block(u08 distance){
   return distance > DISTANCE_THRESHHOLD;
}
 
float run_sensor_model(block_layout_t layout, float particle_location, u08 sensor_val){
    trap_prob_t trap;
    if (is_block(layout, particle_location)){
        trap = block_trap();
        // printf("Using block sensor model\n");
    } else {
        trap = space_trap();
        // printf("Using free space sensor model\n");
    }
    float new_weight = calc_trap(trap, sensor_val, NUM_PARTICLES);
    // printf("Calculated new weight: %f\n", new_weight);
    return new_weight;
}

void print_particle(particle_t p);

void recalculate_weights(block_layout_t layout, particle_t *particle_array, u08 robot_has_block){
    // printf("Recalculating weights\n");
    for (int i = 0; i < NUM_PARTICLES; i++){
        particle_array[i].weight = run_sensor_model(layout, particle_array[i].position, robot_has_block);
        // print_particle(particle_array[i]);
    }
}

int calc_clone_weight(float weight){
    int count = (weight * RATIO_KEEP_PARTICLES * (float) NUM_PARTICLES);
    printf("Clone count: %d from %f\n", count, weight);
    return count;
}

void clone_particle(particle_t *particle_array, particle_t particle, int clone_times, int *array_index_p){
    int array_index = *array_index_p;
    for (int i = 0; i < clone_times; i++){
        particle_array[array_index] = particle;
        array_index++;
    }
    *array_index_p = array_index;
}

void copy_particle_array(particle_t *old_particle_arrray, particle_t *new_particle_array){
    for (int i = 0; i < NUM_PARTICLES; i++){
        new_particle_array[i] = old_particle_arrray[i];
    }
}

void resample_particles(block_layout_t layout, particle_t *particle_array){
    int new_array_index = 0;
    particle_t new_particle_array[NUM_PARTICLES];
    for(int i = 0; i < NUM_PARTICLES; i++){
        int clone_times = calc_clone_weight(particle_array[i].weight);
        printf("Clone particle @ %5.1f  x %d\n", particle_array[i].position, clone_times);
        clone_particle(new_particle_array, particle_array[i], clone_times, &new_array_index);
    }
    while (new_array_index < NUM_PARTICLES){
        new_particle_array[new_array_index] = generate_particle();
        new_array_index++;
    }
    copy_particle_array(new_particle_array, particle_array);
}

void print_particle(particle_t p){
    printf("p @ %5.1f   w @ %5.3f\n", p.position, p.weight);
}

void print_particle_array(particle_t *p_array){
    for (int i = 0; i < NUM_PARTICLES; i++)
    print_particle(p_array[i]);
}

#endif