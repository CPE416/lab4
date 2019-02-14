#ifndef MONTE_H
#define MONTE_H

#include "kill.h"

void localize(){

}

void localize_then_kill(){
    localize();
    kill();
}

#endif 