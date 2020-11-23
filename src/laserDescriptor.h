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
#include "laserCommSM.h"
/*************************************************
 *     Definitions for Laser class and tasks
 */
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

//---------------------------------

typedef uint32_t distance_t;

//---------------------------------

#endif	/* LASERDESCRIPTOR_H */

