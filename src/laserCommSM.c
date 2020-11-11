/**
 * @File laserStateMachines
 * 
 * Implements the laserModule object logic and behavior
 * 
 * @param module
 */


#include "laserCommands.h"
#include "laserDescriptor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"

/**
 * @param module laser module instance
 */
void vLaserCommTask(void *pvParameter) {

    //Use local state?
    LaserCommunication_t *module = (LaserCommunication_t*) pvParameter;
    commState eCommState = COMM_UNINITIALISED;
    
    while (1) {

        switch (eCommState) {
            case COMM_UNINITIALISED: //laser module unitialised
                // TODO this step is not used for now
                eCommState = SEND_INIT;
                break;

            case SEND_INIT: //laser module communication OK and waiting to start measure
                module->dataSend(commandsSend.open, LENGTH_STANDARD);
                module->receiveCount = 0; //clear received data counter
                vTaskDelay(pdMS_TO_TICKS(2)); //wait for answer (After 2 ms should be arrived)
                if (module->dataAvailableCount() >= OPEN_ANSWER_LENGTH) { // read data from buffer
                    module->dataRead(module->databuffer, OPEN_ANSWER_LENGTH);

                    //TODO Compare, but for testing we just go forward.
                    eCommState = IDLE;
                }else {
                    //No answer--> No Module connected
                    vTaskDelay(pdMS_TO_TICKS(1000)); //wait 1000ms before retry
                    eCommState = COMM_UNINITIALISED;
                }
                break;

            case SEND_START_MEASURE: //Module is calibrating
                module->dataSend(commandsSend.startContinuousMeas, LENGTH_STANDARD);
                
                vTaskDelay(pdMS_TO_TICKS(1)); //wait for answer (After 1 ms should be arrived)

                if (module->dataAvailableCount() >= OPEN_ANSWER_LENGTH) { // read data from buffer
                    module->dataRead(module->databuffer, OPEN_ANSWER_LENGTH);
                    //TODO Compare, but for testing we just go forward.
                    module->state = WAIT_MEASURE_DATA;
                    // MODULE state change to initialised
                    eCommState = MEASURE;
                }
                else {
                    //No answer--> No Module connected
                    vTaskDelay(pdMS_TO_TICKS(1000)); //wait 1000ms before reset
                    eCommState = COMM_UNINITIALISED;
                    //TODO send error message
                }                
                
                break;

            case WAIT_MEASURE_DATA:
                if (module->dataAvailableCount() >= MEASURE_DATA_LENGTH) { // read data from buffer
                    module->dataRead(module->databuffer, MEASURE_DATA_LENGTH);
                    //TODO Compare, but for testing we just go forward.
                    eCommState = READ_DISTANCE;
                }
                // No message yet. Wait
                vTaskDelay(pdMS_TO_TICKS(1));
                break;

            case READ_DISTANCE: //Process measured data
                //TODO implement proper data extraction

                eCommState = WAIT_MEASURE_DATA;

                break;

            case SEND_STOP_MEASURE:
                module->dataSend(commandsSend.stopMeasure, LENGTH_STANDARD);
                
                vTaskDelay(pdMS_TO_TICKS(1)); //wait for answer (After 1 ms should be arrived)
                
                if (module->dataAvailableCount() >= OPEN_ANSWER_LENGTH) { // read data from buffer
                    module->dataRead(module->databuffer, OPEN_ANSWER_LENGTH);
                    //TODO Compare, but for testing we just go forward.
                    eCommState = IDLE;
                }
                else{
                    //No answer--> No Module connected
                    vTaskDelay(pdMS_TO_TICKS(1000)); //wait 1000ms before reset
                    eCommState = COMM_UNINITIALISED;
                    //TODO send error message                    
                }
                break;
            case IDLE: //No communication ongoing
                //TODO: Bring task to suspend
                break;


            case COMM_ERROR: //Communication error
                // TODO clear buffer
                break;




                /*      Lasr strength control does not work    
                        case SEND_LASER_STRENGTH: //Module is calibrating
                            sendLaserCommand(commandsSend.setStrength_1, module->comm.dataSend, LENGTH_STANDARD);
                            module->comm.receiveCount = 0; //clear received data counter
                            module->comm.state = WAIT_STRENGTH_FEEDBACK;            
                            break;

                        case WAIT_STRENGTH_FEEDBACK:
                            while (module->comm.dataAvailable() ) { // read data from buffer
                                module->comm.databuffer[module->comm.receiveCount] = module->comm.dataReceive();
                                module->comm.receiveCount++;
                            }
                            if (module->comm.receiveCount == LENGTH_STANDARD) {//if expected length arrived
                                //TODO Compare, but for testing we just go forward.
                                module->comm.state = SEND_LASER_STRENGTH2;
                            }
                            break;            
                        case SEND_LASER_STRENGTH2: //Module is calibrating
                            sendLaserCommand(commandsSend.setStrength_2, module->comm.dataSend, LENGTH_20);
                            module->comm.receiveCount = 0; //clear received data counter
                            module->comm.state = WAIT_STRENGTH2_FEEDBACK;            
                            break;

                        case WAIT_STRENGTH2_FEEDBACK:
                            while (module->comm.dataAvailable() ) { // read data from buffer
                                module->comm.databuffer[module->comm.receiveCount] = module->comm.dataReceive();
                                module->comm.receiveCount++;
                            }
                            if (module->comm.receiveCount == LENGTH_20) {//if expected length arrived
                                //TODO Compare, but for testing we just go forward.
                                module->comm.state = IDLE;
                            }
                            break; 
                 */
            default:
                eCommState = COMM_UNINITIALISED;
        }
    }


}
