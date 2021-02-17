/**
 * @File canCOMM
 * 
 * Implements the can communication RTOS Task
 * 
 */
#include "canComm.h"
#include "semphr.h"

#include "projdefs.h"
#include "peripheral/can/plib_can1.h"
#include "laserControlSM.h"
#include "laserAddresses.h"
#include "laserCommSM.h"
#include "config/default/peripheral/gpio/plib_gpio.h"

//local
typedef enum {
    CAN_UNINITIALISED = 0, //laser module unitialised
    CAN_IDLE, //laser module communication OK and waiting to start measure
    TRY_SEND_AGAIN, //CAN TX is busy, so we have to try again
} CAN_STATE_t;

typedef struct {
    uint32_t id; 
    uint8_t length; 
    uint8_t data;
} canMessage_t;

QueueHandle_t qCanMessageReceived;

//local functions
void vCAN_InterruptHandler(uintptr_t context); //Handles the incoming messages
void vMessageDistributor(void *pvParameter); //Distributes the message to the right laser module
/**
 * @param canAttrib CAN queue info
 */
void vCanCommTask(void *pvParameter) {

    ToCANEvent_t receivedEvent;
    canMessage_t sendCAN_Frame;
            
    CANTask_t *pCAN_Queues = (CANTask_t*) pvParameter;
    CAN_STATE_t eCanState = CAN_UNINITIALISED;
    //create queue for receive interrupt and distributor communication
    qCanMessageReceived = xQueueCreate(4, sizeof(canMessage_t));
    // Create task for message distributor
    xTaskCreate((TaskFunction_t) vMessageDistributor,
        "messageDistributor",
        128,
        (void*)pCAN_Queues,
        1,
        NULL);     
    
    
    
    while (1) {

        switch (eCanState) {
            case CAN_UNINITIALISED:
                eCanState = CAN_IDLE;

                break;
            case CAN_IDLE:
                //Wait for event infinitely  
                xQueueReceive(pCAN_Queues->pxQueueReceiveEvent, (void *)&receivedEvent, portMAX_DELAY);
                sendCAN_Frame.id = receivedEvent.id;
                sendCAN_Frame.data = (uint8_t) receivedEvent.eventType;
                sendCAN_Frame.length = 1;
                
                if( true == CAN1_MessageTransmit(sendCAN_Frame.id,sendCAN_Frame.length,&sendCAN_Frame.data,
                        1,CAN_MSG_TX_DATA_FRAME) ) //FIFO 1 --> transmit
                {
                    //send succesful
                    //GPIO_LED1_Toggle();
                }
                else {//SEND failed, so try again
                    eCanState = TRY_SEND_AGAIN;
                }
                break;
            case TRY_SEND_AGAIN:
                vTaskDelay(pdMS_TO_TICKS(2)); //Wait some time to send the previous message
                if( true == CAN1_MessageTransmit(sendCAN_Frame.id,sendCAN_Frame.length,&sendCAN_Frame.data,
                        1,CAN_MSG_TX_DATA_FRAME) ) //FIFO 1 --> transmit
                {
                    eCanState = CAN_IDLE;
                    //GPIO_LED1_Toggle();
                }
                else {//SEND failed, so try again
                    eCanState = TRY_SEND_AGAIN;
                }
      
            default:
                break;
        }
    }
}
//----------------------
// Can message receive related implementations
volatile SemaphoreHandle_t xCANRecSemaphore;

void vCAN_InterruptHandler(uintptr_t context) {
    /* Check CAN Status */
    CAN_ERROR status = CAN1_ErrorGet();
    if ((status & (CAN_ERROR_TX_RX_WARNING_STATE | CAN_ERROR_RX_WARNING_STATE |
            CAN_ERROR_TX_WARNING_STATE | CAN_ERROR_RX_BUS_PASSIVE_STATE |
            CAN_ERROR_TX_BUS_PASSIVE_STATE | CAN_ERROR_TX_BUS_OFF_STATE)) == CAN_ERROR_NONE)
    {
        //notify message distributor of incoming message
        xSemaphoreGiveFromISR( xCANRecSemaphore, NULL);
 
    }
    else
    { //CAN Error
        //TODO implement error handling
    }
}

 // Buffer to receive   
    static uint8_t rx_message[8];
    static uint32_t rx_messageID = 0;
    static uint8_t rx_messageLength = 0;
    static uint16_t timestamp = 0;    
    static CAN_MSG_RX_ATTRIBUTE msgAttr = CAN_MSG_RX_DATA_FRAME;
/**
 * Only one instance is available!
 */
void vMessageDistributor(void *pvParameter){


    uint8_t i; // For loops
    CANTask_t *pCAN_Queues = (CANTask_t*)pvParameter; //queues
    QueueHandle_t qToLaserCtrl = NULL;
    LaserCtrlEvent_t xLaserCtrlEvent;
    
    xLaserCtrlEvent.eventType = CAN_MSG;
    
    xCANRecSemaphore = xSemaphoreCreateBinary(); //TODO handle memory run out
    
    //Register receive FIFO callback
    CAN1_CallbackRegister( vCAN_InterruptHandler, NULL, 0 ); 

    while(1){
        // Register for msg receiving
        memset(rx_message, 0x00, sizeof(rx_message));
        if (CAN1_MessageReceive(&rx_messageID, &rx_messageLength, rx_message, &timestamp, 0, &msgAttr) == false) {
            // Failed.
            _nop();
        }        
        //wait for message infinitely
        xSemaphoreTake( xCANRecSemaphore, portMAX_DELAY);
        
        xLaserCtrlEvent.data = rx_message[0];
        if((uint16_t)rx_messageID == BROADCAST){
            // Send event to all laser instance
            for(i=0; i<=5; i++){
                if( xQueueSend( pCAN_Queues->pxQueueToLaserCtrl[i] , ( void * ) &xLaserCtrlEvent, 20 ) != pdPASS )
                {
                /* TODO: Failed to post to the message handle even after 20 ticks */
                }                
            }

        }
        else{ //Not broadcast
            qToLaserCtrl = NULL;
            switch((uint16_t)rx_messageID){
                case LASER_ADDRESS_1:
                    qToLaserCtrl = pCAN_Queues->pxQueueToLaserCtrl[0];
                    break;
                case LASER_ADDRESS_2:
                    qToLaserCtrl = pCAN_Queues->pxQueueToLaserCtrl[1];
                    break;            
                case LASER_ADDRESS_3:
                    qToLaserCtrl = pCAN_Queues->pxQueueToLaserCtrl[2];
                    break;
                case LASER_ADDRESS_4:
                    qToLaserCtrl = pCAN_Queues->pxQueueToLaserCtrl[3];
                    break;
                case LASER_ADDRESS_5:
                    qToLaserCtrl = pCAN_Queues->pxQueueToLaserCtrl[4];
                    break;
                case LASER_ADDRESS_6:
                    qToLaserCtrl = pCAN_Queues->pxQueueToLaserCtrl[5];
                    break;                   
            }
            if(qToLaserCtrl != NULL){
                if( xQueueSend( qToLaserCtrl , ( void * ) &xLaserCtrlEvent, 20 ) != pdPASS )
                {
                    /* TODO: Failed to post to the message handle even after 20 ticks */
                }  
            }
            else{
                //Error queue pointer uninitialized
            }
        }
    }
}
