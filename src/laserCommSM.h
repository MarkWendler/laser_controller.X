/* ************************************************************************** */
/** Laser UART communication task

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef LASER_COMM_H    /* Guard against multiple inclusion */
#define LASER_COMM_H

#include "FreeRTOS.h"
#include "queue.h"

void vLaserCommTask(void* pvParameter); //LaserModule
//--------------------------------

typedef enum {
    TURN_ON,
    TURN_OFF,
    RESET_MODULE, //Placeholder
}LaserCommEvent_t;

typedef struct {
    uint8_t receiveCount;
    size_t(*dataSend)(uint8_t*, const size_t); //UART send function
    size_t(*dataRead)(uint8_t*, const size_t); //UART read function
    size_t(*dataAvailableCount) (void); //UART RX ready
    uint8_t databuffer[32];
    QueueHandle_t pxQueueToDistance; // Queue to the Laser ctrl task
    QueueHandle_t pxQueueReceiveLaserComm; // Queue to get events from other tasks
} LaserCommunication_t;

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
