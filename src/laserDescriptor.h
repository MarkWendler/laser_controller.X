/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LASERDESCRIPTOR_H
#define	LASERDESCRIPTOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    UNINITIALISED = 0, //laser module unitialised
    INITIALISED_OFF, //laser module communication OK and waiting to start measure
    MEASURE,    // Laser module
    CALIBRATE,  //Module is calibrating
    ALARM,
    MODULE_ERROR      
} moduleState;

typedef enum {
    COMM_UNINITIALISED = 0, //laser module unitialised
    SEND_INIT, //laser module communication OK and waiting to start measure
    SEND_START_MEASURE,  //Module is calibrating
    WAIT_MEASURE_DATA,
    READ_DISTANCE, //process measured data
    SEND_STOP_MEASURE,
    SEND_LASER_STRENGTH,
    WAIT_STRENGTH_FEEDBACK,
    SEND_LASER_STRENGTH2,
    WAIT_STRENGTH2_FEEDBACK,            
    IDLE,                 //No communication ongoing
    COMM_ERROR        
} communicationState;

typedef struct {
    uint8_t ID;
    moduleState state;
    uint16_t distance;

    struct {
        communicationState state;
        uint8_t receiveCount;
        size_t (*dataSend)(const uint8_t*, const size_t); //UART send function
        size_t (*dataRead)(uint8_t*, const size_t); //UART read function
        size_t (*dataAvailableCount) (void); //UART RX ready
        uint8_t databuffer[32];
    } comm;

} LaserModule;

void laserTask(LaserModule);

#endif	/* LASERDESCRIPTOR_H */

