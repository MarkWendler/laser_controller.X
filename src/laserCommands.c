/*
 * File:   laserCommands.c
 * Author: M18034
 *
 * Created on April 12, 2020, 1:14 PM
 */


#include <xc.h>
#include <stdio.h>
#include "laserDescriptor.h"

void activateLaser (LaserModule *module){
    if(module->state == INITIALISED_OFF){
        module->comm.state = SEND_START_MEASURE;
    }
    //TODO error handling
    //TODO implement a command processor
}

void setLaserStrength (LaserModule *module){
    if(module->state == INITIALISED_OFF){
        module->comm.state = SEND_LASER_STRENGTH;
    }
    //TODO error handling
    //TODO implement a command processor
}


void deactivateLaser (LaserModule *module){
    if(module->state == MEASURE){
        module->comm.state = SEND_STOP_MEASURE;
    }
}