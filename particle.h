#ifndef PARTICLE_H
#define PARTICLE_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "block_layout.h"
#include "trap.h"

#define DISTANCE_THRESHHOLD (60)

#define NUM_PARTICLES (100)
#define RATIO_KEEP_PARTICLES (0.95)

#define TICKS_PER_DEGREE (2.0)
#define RAND_SEED (time(NULL))
#define RAND (rand() / RAND_MAX)
#define RANDOM_PARTICLE_POS (360.0 * (rand() / RAND_MAX))

typedef struct {
    float position;
    float weight;
}particle_t;

particle_t generate_particle(){
    particle_t p;
    p.position = RANDOM_PARTICLE_POS;
    p.weight = 0;
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
   
    return sqrt(-2 * log(RAND)) * cos(2 * M_PI * RAND);
}

void run_motion_model(particle_t *particle_array, int ticks){
    for (int i = 0; i < NUM_PARTICLES; i++){
        particle_array[i].position += ticks_to_degrees(ticks) + generate_gaussian_value();
        if (particle_array[i].position > 360){
            particle_array[i].position -= 360;
        }
    }
}

u08 prox_has_block(u08 distance){
   return distance > DISTANCE_THRESHHOLD;
}
 
float run_sensor_model(block_layout_t layout, float particle_location, u08 sensor_val){
    trap_prob_t trap;
    if (is_block(layout, particle_location)){
        trap = block_trap();
    } else {
        trap = space_trap();
    }
    return calc_trap(trap, sensor_val);
}

void recalculate_weights(block_layout_t layout, particle_t *particle_array, u08 robot_has_block){
    for (int i = 0; i < NUM_PARTICLES; i++){
        particle_t particle = particle_array[i]; 
        particle_array[i].weight = run_sensor_model(layout, particle.position, robot_has_block);
    }
}

int calc_clone_weight(float weight){
    return weight * RATIO_KEEP_PARTICLES * NUM_PARTICLES;
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
        clone_particle(new_particle_array, particle_array[i], clone_times, &new_array_index);
    }
    while (new_array_index < NUM_PARTICLES){
        new_particle_array[new_array_index] = generate_particle();
        new_array_index++;
    }
    copy_particle_array(new_particle_array, particle_array);

}

void print_particle(particle_t p){
    printf("p@ %5.1f\tw@ %6.3f\n", p.position, p.weight);
}

void print_particle_array(particle_t *p_array){
    for (int i = 0; i < NUM_PARTICLES; i++)
    print_particle(p_array[i]);
}

#endif