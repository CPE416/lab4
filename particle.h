#ifndef PARTICLE_H
#define PARTICLE_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "block_layout.h"
#include "trap.h"

#define DISTANCE_THRESHHOLD (60)

#define NUM_PARTICLES (8)
#define RATIO_KEEP_PARTICLES (0.9)

#define TICKS_PER_DEGREE (2.0)
#define RAND_SEED (time(NULL))
#define RAND (((float) rand()) / (float) RAND_MAX)
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


void print_particle(particle_t p){
    printf("p @ %5.1f  weight: %5.3f\n", p.position, p.weight);
}

void print_particle_array(particle_t *p_array){
    for (int i = 0; i < NUM_PARTICLES; i++)
        {
            printf("%2d: pos: %5.1f  weight: %5.3f\n", 
                   i,
                   p_array[i].position,
                   p_array[i].weight);
        }
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


int generate_prox_value(block_layout_t layout, float location){
	float sensor_val;
	u08 has_block = is_block(layout, location);
	if (has_block ){
		float distance  = distance_to_block(layout, location);
		sensor_val = 250.0 - distance;
	}
	else{
		sensor_val = 50.0 * RAND;
	}
	// printf("Generated sensor value: %d from location: %5.1f\n", (int) sensor_val, location);
	return (int) sensor_val;
}

u08 prox_has_block(u08 distance){
   return distance > DISTANCE_THRESHHOLD;
}
 
float run_sensor_model(block_layout_t layout, float particle_location){
    trap_prob_t trap;
    u08 sensor_val = generate_prox_value(layout, particle_location); 
    if (is_block(layout, particle_location)){
        trap = block_trap();
        // printf("Using block sensor model\n");
    } else {
        trap = space_trap();
        // printf("Using free space sensor model\n");
    }
    float new_weight = calc_trap(trap, sensor_val);
    // printf("Calculated new weight: %f for sensor %d\n", new_weight, sensor_val);
    return new_weight;
}


void recalculate_weights(block_layout_t layout, particle_t *particle_array, u08 robot_has_block){
    // printf("Recalculating weights\n");
    float sum = 0;
    for (int i = 0; i < NUM_PARTICLES; i++){
        particle_array[i].weight = run_sensor_model(layout, particle_array[i].position);
        sum += particle_array[i].weight;
    }
    float factor = 1.0 / sum ;
    printf("Weight normalization factor: %f\n", factor);
    for (int i = 0; i < NUM_PARTICLES; i++){
        particle_array[i].weight = factor * particle_array[i].weight;
    }
}

int calc_clone_weight(float weight){
    int count = (int) (weight * RATIO_KEEP_PARTICLES * ((float) NUM_PARTICLES));
    // printf("Clone count: %d from %f\n", count, weight);
    return count;
}

void copy_particle_array(particle_t *old_particle_arrray, particle_t *new_particle_array){
    for (int i = 0; i < NUM_PARTICLES; i++){
        new_particle_array[i] = old_particle_arrray[i];
    }
}

void resample_particles(block_layout_t layout, particle_t *old_array){
    int new_index = 0;
    particle_t new_array[NUM_PARTICLES];
    for(int i = 0; (i < NUM_PARTICLES) && (new_index < NUM_PARTICLES); i++){
        particle_t p = old_array[i];
        int clone_times = calc_clone_weight(p.weight);
        printf("Resampled particle @ %5.1f w: %f, %d times\n", p.position, p.weight, clone_times);
        for (int i = 0; (i < clone_times) && (new_index < NUM_PARTICLES); i++){
            new_array[new_index] = p;
            new_index += 1;
        }
    }
    printf("Resampled %d particles\n", new_index);
    for (int i = new_index; i < NUM_PARTICLES; i++){
        new_array[i] = generate_particle();
    }
    copy_particle_array(new_array, old_array);
}


#endif