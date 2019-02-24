#ifndef SIM_H
#define SIM_H

#include <stdio.h>

#include "block_layout.h"

float increment_location(float location, float degrees){
    float new_location = location + degrees;
    if (new_location > 360.0){
        new_location -= 360.0;
    }
    return new_location;
}


#endif
