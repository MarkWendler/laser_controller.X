/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "laserDescriptor.h"
#include "laserCommSM.h"

#define QUEUE_LENGTH 4

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

/* Handle for the APP_Tasks. */
TaskHandle_t xLaser_1_Comm_Task, xLaser_2_Comm_Task, xLaser_3_Comm_Task, 
        xLaser_4_Comm_Task, xLaser_5_Comm_Task, xLaser_6_Comm_Task;

TaskHandle_t xDebugTask;

extern LaserModule_t module_1, module_2, module_3, module_4,  module_5, module_6;

// Local functions
void createCommTasks(void);
uint8_t createAndConnectQueues(LaserModule_t *);
void debugTask(void);


int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
       

    createAndConnectQueues(&module_1);
    createAndConnectQueues(&module_2);
    createAndConnectQueues(&module_3);
    createAndConnectQueues(&module_4);
    createAndConnectQueues(&module_5);
    createAndConnectQueues(&module_6);
    
    createCommTasks();

 
    xTaskCreate((TaskFunction_t) debugTask,
        "DebugTask",
        128,
        NULL,
        1,
        &xDebugTask);   
    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */    

    return ( EXIT_FAILURE );
}

void debugTask(void){
    
    while(1u){
        GPIO_LED1_Toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
             
        
    }
}


uint8_t createAndConnectQueues(LaserModule_t *module){
    // Create queue for Laser Control Task
    module->ctrl.pxQueueReceiveLaserCtrl = xQueueCreate(QUEUE_LENGTH, sizeof(LaserCtrlEvent_t));
    // Connect inputs to the CTRL queue
    module->canAttributes.pxQueueToLaserCtrl = module->ctrl.pxQueueReceiveLaserCtrl;
    module->distAttributes.pxQueueToLaserCtrl = module->ctrl.pxQueueReceiveLaserCtrl;
    
    // Create queue for Laser Communication Task
    module->comm.pxQueueReceiveLaserComm = xQueueCreate(QUEUE_LENGTH, sizeof(LaserCommEvent_t));
    //Connect inputs to the comm queue
    module->ctrl.pxQueueToComm = module->comm.pxQueueReceiveLaserComm;
    
    // Create queue for distance measure task
    module->distAttributes.pxQueueReceiveDistance = xQueueCreate(QUEUE_LENGTH, sizeof(distance_t));
    // Connect inputs to distance measure task
    module->comm.pxQueueToDistance = module->distAttributes.pxQueueReceiveDistance;
    
    //Create queue for CAN task
    module->canAttributes.pxQueueReceiveCAN = xQueueCreate(QUEUE_LENGTH, sizeof(ToCANEvent_t));
    // Connect inputs to CAN Task
    module->ctrl.pxQueueToCAN = module->canAttributes.pxQueueReceiveCAN;
    
    
    //TODO implement error handle if not succesfully created
    return 0;
}

/*
 * Create all laser UART communication tasks
 */
void createCommTasks(void){
        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_1_Comm_Task",
                128,
                (void*)&module_1.comm,
                1,
                &xLaser_2_Comm_Task);    
    
        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_2_Comm_Task",
                128,
                (void*)&module_2.comm,
                1,
                &xLaser_2_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_3_Comm_Task",
                128,
                (void*)&module_3.comm,
                1,
                &xLaser_3_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_4_Comm_Task",
                128,
                (void*)&module_4.comm,
                1,
                &xLaser_4_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_5_Comm_Task",
                128,
                (void*)&module_5.comm,
                1,
                &xLaser_5_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_6_Comm_Task",
                128,
                (void*)&module_6.comm,
                1,
                &xLaser_6_Comm_Task);    
}

/*******************************************************************************
 End of File
*/

