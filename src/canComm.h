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

typedef enum {
    ERROR_TO_CAN = 0x11,    // Message from CAN
    ALARM_TO_CAN = 0x01,    // Alarm event from distance measure task
    DEBUG_MSG_TO_CAN = 0xF1,
} EnumToCANEventType_t;

typedef struct {
    EnumToCANEventType_t eventType;
    uint8_t id;   
} ToCANEvent_t;

void vCanCommTask(void *pvParameter);

#endif