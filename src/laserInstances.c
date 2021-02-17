#include "config/default/peripheral/uart/plib_uart1.h"
#include "config/default/peripheral/uart/plib_uart2.h"
#include "config/default/peripheral/uart/plib_uart3.h"
#include "config/default/peripheral/uart/plib_uart4.h"
#include "config/default/peripheral/uart/plib_uart5.h"
#include "config/default/peripheral/uart/plib_uart6.h"
#include "laserDescriptor.h"
//************************************//
// Laser object instances //
volatile LaserModule_t module_1 = {
    .ctrl =
    {
        .ID = 1,
    },
    .comm =
    {
        .receiveCount = 0,
        .dataSend = UART6_Write, //hook the corresponding communication function
        .dataRead = UART6_Read,
        .dataAvailableCount = UART6_ReadCountGet
    }
};

volatile LaserModule_t module_2 = {
    .ctrl =
    {
        .ID = 2,
    },
    .comm =
    {
        .receiveCount = 0,
        .dataSend = UART4_Write, //hook the corresponding communication function
        .dataRead = UART4_Read,
        .dataAvailableCount = UART4_ReadCountGet
    }
};

volatile LaserModule_t module_3 = {
    .ctrl =
    {
        .ID = 3,
    },
    .comm =
    {
        .receiveCount = 0,
        .dataSend = UART3_Write, //hook the corresponding communication function
        .dataRead = UART3_Read,
        .dataAvailableCount = UART3_ReadCountGet
    }
};

volatile LaserModule_t module_4 = {
    .ctrl =
    {
        .ID = 4,
    },
    .comm =
    {
        .receiveCount = 0,
        .dataSend = UART5_Write, //hook the corresponding communication function
        .dataRead = UART5_Read,
        .dataAvailableCount = UART5_ReadCountGet
    }
};

volatile LaserModule_t module_5 = {
    .ctrl =
    {
        .ID = 5,
    },
    .comm =
    {
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
    },
    .comm =
    {
        .receiveCount = 0,
        .dataSend = UART2_Write, //hook the corresponding communication function
        .dataRead = UART2_Read,
        .dataAvailableCount = UART2_ReadCountGet
    }
};