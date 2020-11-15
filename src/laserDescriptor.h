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

#include "FreeRTOS.h"
#include "queue.h"
#include "canComm.h"
#include "laserControlSM.h"
/*************************************************
 *     Definitions for Laser class and tasks
 */
typedef enum {
    UNINITIALISED = 0, //laser module unitialised
    INITIALISED_OFF, //laser module communication OK and waiting to start measure
    MEASURE,    // Laser module
    CALIBRATE,  //Module is calibrating
    ALARM_STATE,
    MODULE_ERROR      
} moduleState;

typedef enum {
    COMM_UNINITIALISED = 0, //laser module unitialised
    SEND_INIT, //laser module communication OK and waiting to start measure
    SEND_START_MEASURE,  //Module is calibrating
    READ_DISTANCE, //process measured data
    WAIT_MEASURE_DATA,
    SEND_STOP_MEASURE,
    SEND_LASER_STRENGTH,
    SEND_LASER_STRENGTH2,         
    IDLE,                 //No communication ongoing
    COMM_ERROR        
} commState;

typedef struct {
    commState state;
    uint8_t receiveCount;
    size_t(*dataSend)(uint8_t*, const size_t); //UART send function
    size_t(*dataRead)(uint8_t*, const size_t); //UART read function
    size_t(*dataAvailableCount) (void); //UART RX ready
    uint8_t databuffer[32];
    QueueHandle_t pxQueueToDistance; // Queue to the Laser ctrl task
    QueueHandle_t pxQueueReceiveLaserComm; // Queue to get events from other tasks
} LaserCommunication_t;

typedef struct {
    uint8_t ID;
    moduleState state;
    QueueHandle_t pxQueueToCAN; // Queue to the CAN communication task
    QueueHandle_t pxQueueToComm; // Queue to the Laser COmmunication task
    QueueHandle_t pxQueueToDistance; // Queue to the Distance measure task
    QueueHandle_t pxQueueReceiveLaserCtrl; // Queue to get events from other tasks
} LaserCtrl_t;

typedef struct {
    QueueHandle_t pxQueueToLaserCtrl; // Queue to the Laser ctrl task
    QueueHandle_t pxQueueReceiveDistance; // Queue to get events from other tasks
} DistanceTask_t; // Handles the distance measurement and filtering

typedef struct {

    LaserCtrl_t ctrl;
    LaserCommunication_t comm;
    DistanceTask_t distAttributes;

} LaserModule_t;

/******************************************************************************
 * queue type defs
 ******************************************************************************/


//--------------------------------

typedef enum {
    TURN_ON,
    TURN_OFF,
    RESET_MODULE, //Placeholder
}LaserCommEvent_t;

//---------------------------------

typedef uint32_t distance_t;

//---------------------------------

#endif	/* LASERDESCRIPTOR_H */

