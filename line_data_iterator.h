#ifndef LINE_DATA_ITER_H
#define LINE_DATA_ITER_H

#include "defs.h"

#define MIN_LINE_VAL (160)
#define MAX_LINE_VAL (255)
// #define INCREMENT (1)

int left_val;
int right_val;
int delta;

void init_line_data_iter(u08 delta_val){
    left_val= 0;
    right_val = 0;
    delta = delta_val;
}

u08 continue_epoch(){
    // printf("Continue epoch?: %d < %d\n", left_val, MAX_LINE_VAL);
    if(left_val < MAX_LINE_VAL){
        return 1;
    } else{
        left_val = MIN_LINE_VAL;
        return 0;
    }
}

void increment_iter(){
    right_val += delta;
    if (right_val > MAX_LINE_VAL){
        right_val = MIN_LINE_VAL;
        left_val += delta;
    }
}

line_data_t get_line_iter(){
    line_data_t line_data;
    line_data.left =  left_val;
    line_data.right = right_val;

    increment_iter();

    // line_data.left =  200;
    // line_data.right = 0;

    return line_data;
}


#endif