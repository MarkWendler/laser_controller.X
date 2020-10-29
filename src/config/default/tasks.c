/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "../../laserDescriptor.h"
#include "../../laserCommSM.h"

// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
/* Handle for the APP_Tasks. */
TaskHandle_t xLaser_1_Comm_Task, xLaser_2_Comm_Task, xLaser_3_Comm_Task, 
        xLaser_4_Comm_Task, xLaser_5_Comm_Task, xLaser_6_Comm_Task;

extern LaserModule module_1, module_2, module_3, module_4,  module_5, module_6;

// Local functions
void createCommTasks(void);
/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain the application's state machine. */
        /* Create OS Thread for APP_Tasks. */

    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */

}

/*
 * Create all laser UART communication tasks
 */
void createCommTasks(void){
        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_1_Comm_Task",
                1024,
                (void*)&module_1,
                1,
                &xLaser_2_Comm_Task);    
    
        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_2_Comm_Task",
                1024,
                (void*)&module_2,
                1,
                &xLaser_2_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_3_Comm_Task",
                1024,
                (void*)&module_3,
                1,
                &xLaser_3_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_4_Comm_Task",
                1024,
                (void*)&module_4,
                1,
                &xLaser_4_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_5_Comm_Task",
                1024,
                (void*)&module_5,
                1,
                &xLaser_5_Comm_Task);

        xTaskCreate((TaskFunction_t) vLaserCommTask,
                "Laser_6_Comm_Task",
                1024,
                (void*)&module_6,
                1,
                &xLaser_6_Comm_Task);    
}

/*******************************************************************************
 End of File
 */

