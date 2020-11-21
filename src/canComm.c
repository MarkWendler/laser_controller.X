/**
 * @File canCOMM
 * 
 * Implements the can communication RTOS Task
 * 
 */
#include "canComm.h"
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
                //Register receive FIFO callback
                CAN1_CallbackRegister( vCAN_InterruptHandler, NULL, 0 ); 
                
                
                //TODO Register transmit FIFO callback?   
                
                eCanState = CAN_IDLE;
                break;
            case CAN_IDLE:
                //Wait for event infinitely  
                xQueueReceive(pCAN_Queues->pxQueueReceiveCAN, (void *)&receivedEvent, portMAX_DELAY);
                sendCAN_Frame.id = receivedEvent.id;
                sendCAN_Frame.data = (uint8_t) receivedEvent.eventType;
                sendCAN_Frame.length = 1;
                //TODO error check
                CAN1_MessageTransmit(sendCAN_Frame.id,sendCAN_Frame.length,&sendCAN_Frame.data,
                        1,CAN_MSG_TX_DATA_FRAME); //FIFO 1 --> transmit
                            GPIO_LED1_Toggle();
                break;
            default:
                break;
        }
    }
}


void vCAN_InterruptHandler(uintptr_t context) {
    /* Check CAN Status */
    CAN_ERROR status = CAN1_ErrorGet();
    canMessage_t xCanMessage;
    CAN_MSG_RX_ATTRIBUTE canMsgAttrib; //dummy, not used, always normal message
    
    if ((status & (CAN_ERROR_TX_RX_WARNING_STATE | CAN_ERROR_RX_WARNING_STATE |
            CAN_ERROR_TX_WARNING_STATE | CAN_ERROR_RX_BUS_PASSIVE_STATE |
            CAN_ERROR_TX_BUS_PASSIVE_STATE | CAN_ERROR_TX_BUS_OFF_STATE)) == CAN_ERROR_NONE)
    {
        //notify message distributor of incoming message
        CAN1_MessageReceive(&xCanMessage.id, &xCanMessage.length, &xCanMessage.data,0,1,&canMsgAttrib);
        //send message to queue from ISR to the distributor task
        if( xQueueSendFromISR( qCanMessageReceived , ( void * ) &xCanMessage,NULL ) != pdPASS )
        {
            /* TODO: Failed to post the message handle. */
        }
   
    }
    else
    { //CAN Error
        //TODO implement error handling
    }
}
/*
 * 
 */
void vMessageDistributor(void *pvParameter){
    
    uint8_t i; // For loops
    canMessage_t xCanMessage; // can message received struct
    CANTask_t *pCAN_Queues = (CANTask_t*)pvParameter; //queues
    QueueHandle_t qToLaserCtrl = NULL;
    LaserCtrlEvent_t xLaserCtrlEvent;
    xLaserCtrlEvent.eventType = CAN_MSG;
    while(1){
        
        //wait for message infinitely
        xQueueReceive(qCanMessageReceived, &xCanMessage, portMAX_DELAY);
        xLaserCtrlEvent.data = xCanMessage.data;
        if((uint16_t)xCanMessage.id == BROADCAST){
            // Send event to all laser instance
            for(i=0; i<=5; i++){
                if( xQueueSend( pCAN_Queues->pxQueueToLaserCtrl[i] , ( void * ) &xLaserCtrlEvent, 20 ) != pdPASS )
                {
                /* TODO: Failed to post to the message handle even after 20 ticks */
                }                
            }

        }
        else{ //Not broadcast
            
            switch((uint16_t)xCanMessage.id){
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
