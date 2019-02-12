#ifndef PROP_H
#define PROP_H

#include <stdio.h>

// #include "delay.h"
// #include "hardware.h"
#include "defs.h"


#define THRESHOLD 200
#define BASE_MOTOR_SPEED 25

struct motor_command compute_proportional(u08 left, u08 right){
    struct motor_command output;
    output.left = 0;
    output.right = 0;

    // TODO: Implement P controller here

    return output;
}

#endif