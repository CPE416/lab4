#ifndef SIM_H
#define SIM_H

#include <stdio.h>

#include "particle.h"
#include "block_layout.h"

typedef struct {
    int max_iterations;
    block_layout_t layout;
    float starting_location;
    float movement_ticks;
    float std_dev_threshhold;
}sim_input_t;

typedef struct {
    int iterations;
    float location;
    float end_guess;
    float std_dev;
}sim_output_t;

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

sim_output_t simulate(sim_input_t sim_input){
    block_layout_t layout = sim_input.layout;
    int max_iterations = sim_input.max_iterations;
    float movement_ticks = sim_input.movement_ticks;
    float std_dev_threshhold = sim_input.std_dev_threshhold;

    sim_output_t sim_out;
    sim_out.location = sim_input.starting_location;
    sim_out.iterations = 0;
    sim_out.end_guess = -1.0;
    sim_out.std_dev = -1.0;

    particle_t particle_array[NUM_PARTICLES];
    init_particle_array(particle_array);

    for (; sim_out.iterations < max_iterations; sim_out.iterations++){
        u08 prox_reading = generate_prox_value(layout, sim_out.location);
        // printf("Read physical sensor value %d from location %4.1f\n", prox_reading, location);

        run_motion_model(particle_array, movement_ticks);
        recalculate_weights(layout, particle_array, prox_reading);
        resample_particles(layout, particle_array);

        //////
        printf("\n");
        print_particle_array(particle_array);
        printf("\n");
        printf("Location: %4.1f\n", sim_out.location);
        printf("\n");
        //////

        sim_out.std_dev = compute_std_deviation(particle_array, &sim_out.end_guess);
        if (sim_out.std_dev < std_dev_threshhold){
            //  Localitization complete
            // printf("\tLocalized at %4.1f for actual %4.1f after %d iterations with standard deviation of %3.1f\n", 
            //        sim_out.end_guess, sim_out.location, sim_out.iterations, sim_out.std_dev);
            break;
        }
        if (sim_out.iterations >= max_iterations - 1){
            // Reached max iterations
            // print_particle_array(particle_array);
            printf("FAILED to localize after %d iterations with standard deviation of %3.1f for avg at %4.1f \n",
                   sim_out.iterations, sim_out.std_dev, sim_out.end_guess);
        }else{
            // Continue simulation
            printf("Iteration: %d, continuing. Standard deviation of %3.1f, guess: %4.1f, actual: %4.1f\n", 
                   sim_out.iterations, sim_out.std_dev, sim_out.end_guess, sim_out.location);
            sim_out.location = increment_location(sim_out.location, movement_ticks);
            printf("New Location: %f\n", sim_out.location);
        }
    }
    return sim_out;
}
u08 is_success(sim_output_t sim_output, float theta){
    return abs(sim_output.end_guess - sim_output.location) < theta;
}

#endif
