/**
 * @File laserStateMachines
 * 
 * Implements the laserModule object logic and behavior
 * 
 * @param module
 */


#include "laserCommands.h"
#include "laserDescriptor.h"


/**
 * @param module laser module instance
 */
void communicationTask(LaserModule *module) {

    switch (module->comm.state) {
        case COMM_UNINITIALISED: //laser module unitialised
            // TODO this step is not used for now
            module->comm.state = SEND_INIT;
            break;
            
        case SEND_INIT: //laser module communication OK and waiting to start measure
            module->comm.dataSend(commandsSend.open, LENGTH_STANDARD);
            module->comm.receiveCount = 0; //clear received data counter
            module->comm.state = WAIT_INIT_ANSWER;
            break;
            
        case WAIT_INIT_ANSWER: // Laser module
            if (module->comm.dataAvailableCount() >= OPEN_ANSWER_LENGTH) { // read data from buffer
                module->comm.dataRead(module->comm.databuffer, OPEN_ANSWER_LENGTH);

                //TODO Compare, but for testing we just go forward.
                module->comm.state = IDLE;
                // MODULE state change to initialised
                module->state = INITIALISED_OFF;
            }
            break;
            
        case SEND_START_MEASURE: //Module is calibrating
            module->comm.dataSend(commandsSend.startContinuousMeas, LENGTH_STANDARD);
            module->comm.state = WAIT_START_FEEDBACK;            
            break;

        case WAIT_START_FEEDBACK:
            if (module->comm.dataAvailableCount() >= OPEN_ANSWER_LENGTH) { // read data from buffer
                module->comm.dataRead(module->comm.databuffer, OPEN_ANSWER_LENGTH);
                //TODO Compare, but for testing we just go forward.
                module->comm.state = WAIT_MEASURE_DATA;
                // MODULE state change to initialised
                module->state = MEASURE;
            }
            break;
            
        case WAIT_MEASURE_DATA:
            if (module->comm.dataAvailableCount() >= MEASURE_DATA_LENGTH) { // read data from buffer
                module->comm.dataRead(module->comm.databuffer, MEASURE_DATA_LENGTH);            
                //TODO Compare, but for testing we just go forward.
                module->comm.state = READ_DISTANCE;
            }
            break;

        case READ_DISTANCE: //Process measured data
            //TODO implement proper data extraction
            
            module->comm.state = WAIT_MEASURE_DATA;
            
            break;
            
        case SEND_STOP_MEASURE:
            module->comm.dataSend(commandsSend.stopMeasure, LENGTH_STANDARD);
            module->comm.state = WAIT_STOP_FEEDBACK;             
            break;
            
        case WAIT_STOP_FEEDBACK:
            if (module->comm.dataAvailableCount() >= OPEN_ANSWER_LENGTH) { // read data from buffer
                module->comm.dataRead(module->comm.databuffer, OPEN_ANSWER_LENGTH);
                //TODO Compare, but for testing we just go forward.
                module->comm.state = IDLE;
                // MODULE state change to initialised
                module->state = INITIALISED_OFF;
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
            module->comm.state = COMM_UNINITIALISED;
    }


}
