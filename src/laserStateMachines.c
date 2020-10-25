/**
 * @File laserStateMachines
 * 
 * Implements the laserModule object logic and behavior
 * 
 * @param module
 */


#include "laserCommands.h"
#include "laserDescriptor.h"


void communicationTask(LaserModule *module);
void moduleControlTask(LaserModule *module);
// Send 16 byte command to laser module

/**
 * 
 * @param length Size of the array that must be sent
 * 
 */
void sendLaserCommand(const uint8_t *command, void (*dataSend)(uint8_t), uint8_t length) {
    printf("Command send:");
    for (uint8_t i = 0; i < length; i++) {
        dataSend(*command);
        command++;
    }
    //Debug
    for (uint8_t i = 0; i < length; i++) {
        printf("%X ", *command);
        command++;
    }
    printf("\n");
}

void taskModule(LaserModule *module) {

    communicationTask(module);
    moduleControlTask(module);

}

void moduleControlTask(LaserModule *module) {
    switch (module->state) {
        case UNINITIALISED:
            //TODO for now we just wait the communication task start the initialisation
            break;
        case INITIALISED_OFF:
            //TODO for now wait for external command   
            break;
        default:
            break;
    }
}

void communicationTask(LaserModule *module) {

    switch (module->comm.state) {
        case COMM_UNINITIALISED: //laser module unitialised
            // TODO this step is not used for now
            module->comm.state = SEND_INIT;
            break;
            
        case SEND_INIT: //laser module communication OK and waiting to start measure
            sendLaserCommand(commandsSend.open, module->comm.dataSend, LENGTH_STANDARD);
            module->comm.receiveCount = 0; //clear received data counter
            module->comm.state = WAIT_INIT_ANSWER;
            break;
            
        case WAIT_INIT_ANSWER: // Laser module
            while (module->comm.dataAvailable() ) { // read data from buffer
                module->comm.databuffer[module->comm.receiveCount] = module->comm.dataReceive();
                module->comm.receiveCount++;
            }
            if (module->comm.receiveCount == OPEN_ANSWER_LENGTH) {//if expected length arrived
                //TODO Compare, but for testing we just go forward.
                module->comm.state = IDLE;
                // MODULE state change to initialised
                module->state = INITIALISED_OFF;
            }
            break;
            
        case SEND_START_MEASURE: //Module is calibrating
            sendLaserCommand(commandsSend.startContinuousMeas, module->comm.dataSend, LENGTH_STANDARD);
            module->comm.receiveCount = 0; //clear received data counter
            module->comm.state = WAIT_START_FEEDBACK;            
            break;

        case WAIT_START_FEEDBACK:
            while (module->comm.dataAvailable() ) { // read data from buffer
                module->comm.databuffer[module->comm.receiveCount] = module->comm.dataReceive();
                module->comm.receiveCount++;
            }
            if (module->comm.receiveCount == OPEN_ANSWER_LENGTH) {//if expected length arrived
                //TODO Compare, but for testing we just go forward.
                module->comm.state = WAIT_MEASURE_DATA;
                // MODULE state change to initialised
                module->state = MEASURE;
                module->comm.receiveCount = 0; //clear received data counter
            }
            break;
            
        case WAIT_MEASURE_DATA:
            while (module->comm.dataAvailable() ) { // read data from buffer
                module->comm.databuffer[module->comm.receiveCount] = module->comm.dataReceive();
                module->comm.receiveCount++;
            }
            if (module->comm.receiveCount == MEASURE_DATA_LENGTH) {//if expected length arrived
                //TODO Compare, but for testing we just go forward.
                module->comm.state = READ_DISTANCE;
            }
            break;

        case READ_DISTANCE: //Process measured data
            //TODO implement proper data extraction
            
            module->comm.receiveCount = 0; //clear received data counter
            module->comm.state = WAIT_MEASURE_DATA;
            
            break;
        case SEND_STOP_MEASURE:
            sendLaserCommand(commandsSend.stopMeasure, module->comm.dataSend, LENGTH_STANDARD);
            module->comm.receiveCount = 0; //clear received data counter
            module->comm.state = WAIT_STOP_FEEDBACK;             
            break;
        case WAIT_STOP_FEEDBACK:
            while (module->comm.dataAvailable() ) { // read data from buffer
                module->comm.databuffer[module->comm.receiveCount] = module->comm.dataReceive();
                module->comm.receiveCount++;
            }
            if (module->comm.receiveCount == OPEN_ANSWER_LENGTH) {//if expected length arrived
                //TODO Compare, but for testing we just go forward.
                module->comm.state = IDLE;
                // MODULE state change to initialised
                module->state = INITIALISED_OFF;
            }            
            break;            
        case IDLE: //No communication ongoing

            break;
        case COMM_ERROR: //Communication error

            break;
            
            
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
            
        default:
            module->comm.state = COMM_UNINITIALISED;
    }


}
