#include "config/default/peripheral/uart/plib_uart1.h"
#include "config/default/peripheral/uart/plib_uart2.h"
#include "config/default/peripheral/uart/plib_uart3.h"
#include "laserDescriptor.h"
//************************************//
// Laser object instances //
volatile LaserModule_t module_1 = {
    .ctrl =
    {
        .ID = 1,
        .state = UNINITIALISED
    },
    .comm =
    {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};

volatile LaserModule_t module_2 = {
    .ctrl =
    {
        .ID = 2,
        .state = UNINITIALISED
    },
    .comm =
    {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};

volatile LaserModule_t module_3 = {
    .ctrl =
    {
        .ID = 3,
        .state = UNINITIALISED
    },
    .comm =
    {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};

volatile LaserModule_t module_4 = {
    .ctrl =
    {
        .ID = 4,
        .state = UNINITIALISED
    },
    .comm =
    {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};

volatile LaserModule_t module_5 = {
    .ctrl =
    {
        .ID = 5,
        .state = UNINITIALISED
    },
    .comm =
    {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};

volatile LaserModule_t module_6 = {
    .ctrl =
    {
        .ID = 6,
        .state = UNINITIALISED
    },
    .comm =
    {
        .state = COMM_UNINITIALISED,
        .receiveCount = 0,
        .dataSend = UART1_Write, //hook the corresponding communication function
        .dataRead = UART1_Read,
        .dataAvailableCount = UART1_ReadCountGet
    }
};