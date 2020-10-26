#include "config/default/peripheral/uart/plib_uart1.h"
#include "config/default/peripheral/uart/plib_uart2.h"
#include "config/default/peripheral/uart/plib_uart3.h"
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
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
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
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
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
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
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
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
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
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};
