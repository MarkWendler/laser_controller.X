#include "mcc_generated_files/uart1.h"
#include "laserDescriptor.h"
//************************************//
// Laser object instances //
volatile LaserModule module_1 = { 
    .ID = 1,
    .state = UNINITIALISED,
    .distance = 0,
    
    .comm = {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataReceive = UART1_Read,
        .dataAvailable = UART1_is_rx_ready
    }
};
// Laser module 2
volatile LaserModule module_2 = { 
    .ID = 1,
    .state = UNINITIALISED,
    .distance = 0,
    
    .comm = {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataReceive = UART1_Read,
        .dataAvailable = UART1_is_rx_ready
    }
};
// Laser module 3
volatile LaserModule module_3 = { 
    .ID = 1,
    .state = UNINITIALISED,
    .distance = 0,
    
    .comm = {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataReceive = UART1_Read,
        .dataAvailable = UART1_is_rx_ready
    }
};
// Laser module 4
volatile LaserModule module_4 = { 
    .ID = 1,
    .state = UNINITIALISED,
    .distance = 0,
    
    .comm = {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataReceive = UART1_Read,
        .dataAvailable = UART1_is_rx_ready
    }
};
// Laser module 5
volatile LaserModule module_5 = { 
    .ID = 1,
    .state = UNINITIALISED,
    .distance = 0,
    
    .comm = {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataReceive = UART1_Read,
        .dataAvailable = UART1_is_rx_ready
    }
};
