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
#include "laserDistanceMeasure.h"
/*************************************************
 *     Definitions for Laser class and tasks
 */

typedef struct {

    LaserCtrl_t ctrl;
    LaserCommunication_t comm;
    LaserDistanceTask_t distanceQueues;

} LaserModule_t;

//---------------------------------

#endif	/* LASERDESCRIPTOR_H */

