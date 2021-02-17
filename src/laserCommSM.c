/**
 * @File laserStateMachines
 * 
 * Implements the laserModule object logic and behavior
 * 
 */

#include "peripheral/gpio/plib_gpio.h"
#include "laserCommands.h"
#include "laserDescriptor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"

typedef enum {
    COMM_UNINITIALISED = 0, //laser module unitialised
    SEND_INIT, //laser module communication OK and waiting to start measure
    SEND_START_MEASURE,  //Module is calibrating
    READ_DISTANCE, //process measured data
    WAIT_MEASURE_DATA,
    SEND_STOP_MEASURE,      
    IDLE,                 //No communication ongoing
    COMM_ERROR        
} commState;




/************************************************************/
/*                  For receive packet processing           */
typedef struct {
    size_t(*dataRead)(uint8_t*, const size_t); //UART read function
    size_t(*dataAvailableCount) (void); //UART RX ready
    uint8_t databuffer[32];
    QueueHandle_t pxQueueToCommTask;
} ReceivePacketTask_t;

typedef enum { //Enum for the received message types from the laser module
    EMPTY_FB = 0,
    STATUS_FB = 1,
    TURN_ON_OFF_FB,
    DISTANCE_FB,
    INIT_FB,
}RecevivedPacketType_t;

typedef struct {
    RecevivedPacketType_t packetType;
    uint32_t data;
}ReceivedPacket_t;

/**
 * Task to process the received packets
 * @param pvParameter
 */
void vReceivePacketProcessing(void *pvParameter);
/************************************************************/


/**
 * @param module laser module instance
 */
void vLaserCommTask(void *pvParameter) {

    //Use local state?
    LaserCommunication_t *module = (LaserCommunication_t*) pvParameter;
    commState eCommState = COMM_UNINITIALISED;
    LaserCommEvent_t receivedCommand; //Receive command from queue
    QueueHandle_t pxQueueToDistanceTask = module->pxQueueToDistance;
    DistanceEventType_t distanceEvent;
    QueueHandle_t queueReceivePacket = xQueueCreate(5, sizeof(ReceivedPacket_t));
    
    ReceivedPacket_t receivedPacket;
    ReceivePacketTask_t packetTask;
    packetTask.dataAvailableCount = module->dataAvailableCount;
    packetTask.dataRead = module->dataRead;
    packetTask.pxQueueToCommTask = queueReceivePacket;
    // Create task for packet processing
    xTaskCreate((TaskFunction_t) vReceivePacketProcessing,
        "receivePacket",
        128,
        &packetTask,
        1,
        NULL); 
    
    vTaskDelay(pdMS_TO_TICKS(200)); //Wait for powerup
    while (1) {

        switch (eCommState) {
            case COMM_UNINITIALISED: //laser module unitialised
                // TODO this step is not used for now
                eCommState = SEND_INIT;
                xQueueReset(module->pxQueueReceiveLaserComm);
                receivedCommand = NULL;
                break;

            case SEND_INIT: //laser module communication OK and waiting to start measure
                module->dataSend(commandsSend.open, sizeof(commandsSend.open));
                
                if ( xQueueReceive(queueReceivePacket, (void *)&receivedPacket, 10) != pdPASS){ // No feedback
                    module->dataSend(commandsSend.reset, LENGTH_STANDARD);
                    vTaskDelay(pdMS_TO_TICKS(1000)); //wait 1000ms before retry
                    eCommState = COMM_UNINITIALISED;                    
                }
                else{
                    eCommState = IDLE;
                }
                break;

            case SEND_START_MEASURE: //Module is calibrating
                
                module->dataSend(commandsSend.startContinuousMeas, LENGTH_STANDARD);
                
                if ( xQueueReceive(queueReceivePacket, (void *)&receivedPacket, 10) != pdPASS){ // No feedback
                    eCommState = COMM_UNINITIALISED;                    
                }
                if(receivedPacket.packetType == TURN_ON_OFF_FB) eCommState = WAIT_MEASURE_DATA;
                else eCommState = COMM_UNINITIALISED;               
                
                break;

            case WAIT_MEASURE_DATA:

                if ( xQueueReceive(queueReceivePacket, (void *)&receivedPacket, 25) != pdPASS){ // every 10ms new packet should come
                    eCommState = COMM_UNINITIALISED;             //No data anymore       
                }
                if(receivedPacket.packetType == DISTANCE_FB){
                    distanceEvent.eventType = DISTANCE_DATA;
                    distanceEvent.data = receivedPacket.data;
                    if (xQueueSend(pxQueueToDistanceTask, (void *) &distanceEvent, 5) != pdPASS) {
                        //TODO handle Q full 
                    }     
                }
                else eCommState = COMM_UNINITIALISED;               
                

                xQueueReceive(module->pxQueueReceiveLaserComm, (void *)&receivedCommand, 0);
                if(receivedCommand == TURN_OFF) eCommState = SEND_STOP_MEASURE; 
                if(receivedCommand == RESET_MODULE) eCommState = COMM_UNINITIALISED;                
                receivedCommand = NULL;

                break;

            case SEND_STOP_MEASURE:
                module->dataSend(commandsSend.stopMeasure, LENGTH_STANDARD);
                
                if ( xQueueReceive(queueReceivePacket, (void *)&receivedPacket, 10) != pdPASS){ // No feedback
                    eCommState = COMM_UNINITIALISED;                    
                }
                if(receivedPacket.packetType == TURN_ON_OFF_FB) eCommState = IDLE;
                else eCommState = COMM_UNINITIALISED;    
                break;
            case IDLE: //No communication ongoing
                // Wait until external command "ON"
                
                receivedCommand = NULL;
                xQueueReceive(module->pxQueueReceiveLaserComm, (void *)&receivedCommand, portMAX_DELAY);
                if(receivedCommand == TURN_ON) eCommState = SEND_START_MEASURE;
                if(receivedCommand == RESET_MODULE) eCommState = COMM_UNINITIALISED;
                receivedCommand = NULL;
                break;
            case COMM_ERROR: //Communication error
                // TODO clear buffer
                break;
            default:
                eCommState = COMM_UNINITIALISED;
        }
    }
}


void vReceivePacketProcessing(void *pvParameter){
    ReceivePacketTask_t *packetTask = (ReceivePacketTask_t*) pvParameter;
    uint8_t buffer[32];
    uint8_t counter = 0;
    uint8_t packetLength;
    ReceivedPacket_t receivedPacket; 
    
    while(1){ //Task
        while( packetTask->dataAvailableCount() ){ // until have data in buffer
            
            if(counter == 0 ){ // first byte
                packetTask->dataRead(buffer, 1); //read first byte
                if(buffer[0] == 0xFB) counter++;
                else {
                    counter = 0; //not first byte
                    packetLength = 0;
                }
            }
            else{
                packetTask->dataRead(&buffer[counter], 1);
                
                if(counter == 1){ //check message type
                    switch(buffer[1]){
                        case 0x03: //Measurement data
                           receivedPacket.packetType = DISTANCE_FB;
                           packetLength = 24;
                           break;
                        case 0x02: //ON or OFF laser
                            receivedPacket.packetType = TURN_ON_OFF_FB;
                            packetLength = 16;
                            break;
                        case 0xFB:
                            receivedPacket.packetType = INIT_FB;
                            packetLength = 16;
                            break;
                    }
                }
                counter++;

                if(counter == (packetLength) ){ //final byte
                    //TODO CRC
                    if(receivedPacket.packetType == DISTANCE_FB){ //extract distance
                        receivedPacket.data = ((uint32_t)buffer[13]<<8) + (buffer[12]);
                    }
                    else {
                        receivedPacket.data = 0;
                    }
                    if (xQueueSend(packetTask->pxQueueToCommTask, (void *) &receivedPacket, 5) != pdPASS) {
                        //TODO handle Q full 
                    }   
                    receivedPacket.packetType = EMPTY_FB;
                    counter = 0;
                    packetLength = 0;
                }
                
            }
        }
        vTaskDelay(1);
    }
    
    
}