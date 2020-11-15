/**
 * @File canCOMM header
 * 
 * Implements the laserModule object logic and behavior
 * 
 */


#ifndef CANCOMM_H
#define	CANCOMM_H
#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
    QueueHandle_t pxQueueToLaserCtrl[6]; // Queue to the Laser ctrl task
    QueueHandle_t pxQueueReceiveCAN; // Queue to get events from other tasks
} CANTask_t;


void vCanCommTask(void *pvParameter);

#endif