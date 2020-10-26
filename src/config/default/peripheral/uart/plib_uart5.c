/*******************************************************************************
  UART5 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart5.c

  Summary:
    UART5 PLIB Implementation File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "device.h"
#include "plib_uart5.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART5 Implementation
// *****************************************************************************
// *****************************************************************************

UART_RING_BUFFER_OBJECT uart5Obj;

#define UART5_READ_BUFFER_SIZE      128
#define UART5_RX_INT_DISABLE()      IEC2CLR = _IEC2_U5RXIE_MASK;
#define UART5_RX_INT_ENABLE()       IEC2SET = _IEC2_U5RXIE_MASK;

static uint8_t UART5_ReadBuffer[UART5_READ_BUFFER_SIZE];

#define UART5_WRITE_BUFFER_SIZE     128
#define UART5_TX_INT_DISABLE()      IEC2CLR = _IEC2_U5TXIE_MASK;
#define UART5_TX_INT_ENABLE()       IEC2SET = _IEC2_U5TXIE_MASK;

static uint8_t UART5_WriteBuffer[UART5_WRITE_BUFFER_SIZE];

void static UART5_ErrorClear( void )
{
    /* rxBufferLen = (FIFO level + RX register) */
    uint8_t rxBufferLen = UART_RXFIFO_DEPTH;
    uint8_t dummyData = 0u;

    /* If it's a overrun error then clear it to flush FIFO */
    if(U5STA & _U5STA_OERR_MASK)
    {
        U5STACLR = _U5STA_OERR_MASK;
    }

    /* Read existing error bytes from FIFO to clear parity and framing error flags */
    while(U5STA & (_U5STA_FERR_MASK | _U5STA_PERR_MASK))
    {
        dummyData = (uint8_t )(U5RXREG );
        rxBufferLen--;

        /* Try to flush error bytes for one full FIFO and exit instead of
         * blocking here if more error bytes are received */
        if(rxBufferLen == 0u)
        {
            break;
        }
    }

    // Ignore the warning
    (void)dummyData;

    /* Clear error interrupt flag */
    IFS2CLR = _IFS2_U5EIF_MASK;

    /* Clear up the receive interrupt flag so that RX interrupt is not
     * triggered for error bytes */
    IFS2CLR = _IFS2_U5RXIF_MASK;

    return;
}

void UART5_Initialize( void )
{
    /* Set up UxMODE bits */
    /* STSEL  = 0*/
    /* PDSEL = 0 */
    /* BRGH = 0 */
    /* RXINV = 0 */
    /* ABAUD = 0 */
    /* LPBACK = 0 */
    /* WAKE = 0 */
    /* SIDL = 0 */
    /* RUNOVF = 0 */
    /* CLKSEL = 0 */
    /* SLPEN = 0 */
    U5MODE = 0x0;

    /* Enable UART5 Receiver, Transmitter and TX Interrupt selection */
    U5STASET = (_U5STA_UTXEN_MASK | _U5STA_URXEN_MASK | _U5STA_UTXISEL0_MASK);

    /* BAUD Rate register Setup */
    U5BRG = 32;

    IEC2CLR = _IEC2_U5TXIE_MASK;

    /* Initialize instance object */
    uart5Obj.rdCallback = NULL;
    uart5Obj.rdInIndex = 0;
    uart5Obj.rdOutIndex = 0;
    uart5Obj.isRdNotificationEnabled = false;
    uart5Obj.isRdNotifyPersistently = false;
    uart5Obj.rdThreshold = 0;

    uart5Obj.wrCallback = NULL;
    uart5Obj.wrInIndex = 0;
    uart5Obj.wrOutIndex = 0;
    uart5Obj.isWrNotificationEnabled = false;
    uart5Obj.isWrNotifyPersistently = false;
    uart5Obj.wrThreshold = 0;

    /* Turn ON UART5 */
    U5MODESET = _U5MODE_ON_MASK;

    /* Enable UART5_FAULT Interrupt */
    IEC2SET = _IEC2_U5EIE_MASK;

    /* Enable UART5_RX Interrupt */
    IEC2SET = _IEC2_U5RXIE_MASK;
}

bool UART5_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t baud = setup->baudRate;
    uint32_t brgValHigh = 0;
    uint32_t brgValLow = 0;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if (setup != NULL)
    {
        /* Turn OFF UART5 */
        U5MODECLR = _U5MODE_ON_MASK;

        if(srcClkFreq == 0)
        {
            srcClkFreq = UART5_FrequencyGet();
        }

        /* Calculate BRG value */
        brgValLow = ((srcClkFreq / baud) >> 4) - 1;
        brgValHigh = ((srcClkFreq / baud) >> 2) - 1;

        /* Check if the baud value can be set with low baud settings */
        if((brgValHigh >= 0) && (brgValHigh <= UINT16_MAX))
        {
            brgVal =  (((srcClkFreq >> 2) + (baud >> 1)) / baud ) - 1;
            U5MODESET = _U5MODE_BRGH_MASK;
        }
        else if ((brgValLow >= 0) && (brgValLow <= UINT16_MAX))
        {
            brgVal = ( ((srcClkFreq >> 4) + (baud >> 1)) / baud ) - 1;
            U5MODECLR = _U5MODE_BRGH_MASK;
        }
        else
        {
            return status;
        }

        if(setup->dataWidth == UART_DATA_9_BIT)
        {
            if(setup->parity != UART_PARITY_NONE)
            {
               return status;
            }
            else
            {
               /* Configure UART5 mode */
               uartMode = U5MODE;
               uartMode &= ~_U5MODE_PDSEL_MASK;
               U5MODE = uartMode | setup->dataWidth;
            }
        }
        else
        {
            /* Configure UART5 mode */
            uartMode = U5MODE;
            uartMode &= ~_U5MODE_PDSEL_MASK;
            U5MODE = uartMode | setup->parity ;
        }

        /* Configure UART5 mode */
        uartMode = U5MODE;
        uartMode &= ~_U5MODE_STSEL_MASK;
        U5MODE = uartMode | setup->stopBits ;

        /* Configure UART5 Baud Rate */
        U5BRG = brgVal;

        U5MODESET = _U5MODE_ON_MASK;

        status = true;
    }

    return status;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static inline bool UART5_RxPushByte(uint8_t rdByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;

    tempInIndex = uart5Obj.rdInIndex + 1;

    if (tempInIndex >= UART5_READ_BUFFER_SIZE)
    {
        tempInIndex = 0;
    }

    if (tempInIndex == uart5Obj.rdOutIndex)
    {
        /* Queue is full - Report it to the application. Application gets a chance to free up space by reading data out from the RX ring buffer */
        if(uart5Obj.rdCallback != NULL)
        {
            uart5Obj.rdCallback(UART_EVENT_READ_BUFFER_FULL, uart5Obj.rdContext);

            /* Read the indices again in case application has freed up space in RX ring buffer */
            tempInIndex = uart5Obj.rdInIndex + 1;

            if (tempInIndex >= UART5_READ_BUFFER_SIZE)
            {
                tempInIndex = 0;
            }
        }
    }

    if (tempInIndex != uart5Obj.rdOutIndex)
    {
        UART5_ReadBuffer[uart5Obj.rdInIndex] = rdByte;
        uart5Obj.rdInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Data will be lost. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void UART5_ReadNotificationSend(void)
{
    uint32_t nUnreadBytesAvailable;

    if (uart5Obj.isRdNotificationEnabled == true)
    {
        nUnreadBytesAvailable = UART5_ReadCountGet();

        if(uart5Obj.rdCallback != NULL)
        {
            if (uart5Obj.isRdNotifyPersistently == true)
            {
                if (nUnreadBytesAvailable >= uart5Obj.rdThreshold)
                {
                    uart5Obj.rdCallback(UART_EVENT_READ_THRESHOLD_REACHED, uart5Obj.rdContext);
                }
            }
            else
            {
                if (nUnreadBytesAvailable == uart5Obj.rdThreshold)
                {
                    uart5Obj.rdCallback(UART_EVENT_READ_THRESHOLD_REACHED, uart5Obj.rdContext);
                }
            }
        }
    }
}

size_t UART5_Read(uint8_t* pRdBuffer, const size_t size)
{
    size_t nBytesRead = 0;
    uint32_t rdOutIndex;
    uint32_t rdInIndex;

    while (nBytesRead < size)
    {
        UART5_RX_INT_DISABLE();

        rdOutIndex = uart5Obj.rdOutIndex;
        rdInIndex = uart5Obj.rdInIndex;

        if (rdOutIndex != rdInIndex)
        {
            pRdBuffer[nBytesRead++] = UART5_ReadBuffer[uart5Obj.rdOutIndex++];

            if (uart5Obj.rdOutIndex >= UART5_READ_BUFFER_SIZE)
            {
                uart5Obj.rdOutIndex = 0;
            }
            UART5_RX_INT_ENABLE();
        }
        else
        {
            UART5_RX_INT_ENABLE();
            break;
        }
    }

    return nBytesRead;
}

size_t UART5_ReadCountGet(void)
{
    size_t nUnreadBytesAvailable;
    uint32_t rdInIndex;
    uint32_t rdOutIndex;

    /* Take a snapshot of indices to avoid creation of critical section */
    rdInIndex = uart5Obj.rdInIndex;
    rdOutIndex = uart5Obj.rdOutIndex;

    if ( rdInIndex >=  rdOutIndex)
    {
        nUnreadBytesAvailable =  rdInIndex -  rdOutIndex;
    }
    else
    {
        nUnreadBytesAvailable =  (UART5_READ_BUFFER_SIZE -  rdOutIndex) + rdInIndex;
    }

    return nUnreadBytesAvailable;
}

size_t UART5_ReadFreeBufferCountGet(void)
{
    return (UART5_READ_BUFFER_SIZE - 1) - UART5_ReadCountGet();
}

size_t UART5_ReadBufferSizeGet(void)
{
    return (UART5_READ_BUFFER_SIZE - 1);
}

bool UART5_ReadNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = uart5Obj.isRdNotificationEnabled;

    uart5Obj.isRdNotificationEnabled = isEnabled;

    uart5Obj.isRdNotifyPersistently = isPersistent;

    return previousStatus;
}

void UART5_ReadThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0)
    {
        uart5Obj.rdThreshold = nBytesThreshold;
    }
}

void UART5_ReadCallbackRegister( UART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    uart5Obj.rdCallback = callback;

    uart5Obj.rdContext = context;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static bool UART5_TxPullByte(uint8_t* pWrByte)
{
    bool isSuccess = false;
    uint32_t wrOutIndex = uart5Obj.wrOutIndex;
    uint32_t wrInIndex = uart5Obj.wrInIndex;

    if (wrOutIndex != wrInIndex)
    {
        *pWrByte = UART5_WriteBuffer[uart5Obj.wrOutIndex++];

        if (uart5Obj.wrOutIndex >= UART5_WRITE_BUFFER_SIZE)
        {
            uart5Obj.wrOutIndex = 0;
        }
        isSuccess = true;
    }

    return isSuccess;
}

static inline bool UART5_TxPushByte(uint8_t wrByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;

    tempInIndex = uart5Obj.wrInIndex + 1;

    if (tempInIndex >= UART5_WRITE_BUFFER_SIZE)
    {
        tempInIndex = 0;
    }
    if (tempInIndex != uart5Obj.wrOutIndex)
    {
        UART5_WriteBuffer[uart5Obj.wrInIndex] = wrByte;
        uart5Obj.wrInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Report Error. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void UART5_WriteNotificationSend(void)
{
    uint32_t nFreeWrBufferCount;

    if (uart5Obj.isWrNotificationEnabled == true)
    {
        nFreeWrBufferCount = UART5_WriteFreeBufferCountGet();

        if(uart5Obj.wrCallback != NULL)
        {
            if (uart5Obj.isWrNotifyPersistently == true)
            {
                if (nFreeWrBufferCount >= uart5Obj.wrThreshold)
                {
                    uart5Obj.wrCallback(UART_EVENT_WRITE_THRESHOLD_REACHED, uart5Obj.wrContext);
                }
            }
            else
            {
                if (nFreeWrBufferCount == uart5Obj.wrThreshold)
                {
                    uart5Obj.wrCallback(UART_EVENT_WRITE_THRESHOLD_REACHED, uart5Obj.wrContext);
                }
            }
        }
    }
}

static size_t UART5_WritePendingBytesGet(void)
{
    size_t nPendingTxBytes;

    /* Take a snapshot of indices to avoid creation of critical section */
    uint32_t wrOutIndex = uart5Obj.wrOutIndex;
    uint32_t wrInIndex = uart5Obj.wrInIndex;

    if ( wrInIndex >=  wrOutIndex)
    {
        nPendingTxBytes =  wrInIndex -  wrOutIndex;
    }
    else
    {
        nPendingTxBytes =  (UART5_WRITE_BUFFER_SIZE -  wrOutIndex) + wrInIndex;
    }

    return nPendingTxBytes;
}

size_t UART5_WriteCountGet(void)
{
    size_t nPendingTxBytes;

    nPendingTxBytes = UART5_WritePendingBytesGet();

    return nPendingTxBytes;
}

size_t UART5_Write(uint8_t* pWrBuffer, const size_t size )
{
    size_t nBytesWritten  = 0;

    UART5_TX_INT_DISABLE();

    while (nBytesWritten < size)
    {
        if (UART5_TxPushByte(pWrBuffer[nBytesWritten]) == true)
        {
            nBytesWritten++;
        }
        else
        {
            /* Queue is full, exit the loop */
            break;
        }
    }

    /* Check if any data is pending for transmission */
    if (UART5_WritePendingBytesGet() > 0)
    {
        /* Enable TX interrupt as data is pending for transmission */
        UART5_TX_INT_ENABLE();
    }

    return nBytesWritten;
}

size_t UART5_WriteFreeBufferCountGet(void)
{
    return (UART5_WRITE_BUFFER_SIZE - 1) - UART5_WriteCountGet();
}

size_t UART5_WriteBufferSizeGet(void)
{
    return (UART5_WRITE_BUFFER_SIZE - 1);
}

bool UART5_WriteNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = uart5Obj.isWrNotificationEnabled;

    uart5Obj.isWrNotificationEnabled = isEnabled;

    uart5Obj.isWrNotifyPersistently = isPersistent;

    return previousStatus;
}

void UART5_WriteThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0)
    {
        uart5Obj.wrThreshold = nBytesThreshold;
    }
}

void UART5_WriteCallbackRegister( UART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    uart5Obj.wrCallback = callback;

    uart5Obj.wrContext = context;
}

UART_ERROR UART5_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = U5STA;

    errors = (UART_ERROR)(status & (_U5STA_OERR_MASK | _U5STA_FERR_MASK | _U5STA_PERR_MASK));

    if(errors != UART_ERROR_NONE)
    {
        UART5_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART5_AutoBaudQuery( void )
{
    if(U5MODE & _U5MODE_ABAUD_MASK)
        return true;
    else
        return false;
}

void UART5_AutoBaudSet( bool enable )
{
    if( enable == true )
    {
        U5MODESET = _U5MODE_ABAUD_MASK;
    }

    /* Turning off ABAUD if it was on can lead to unpredictable behavior, so that
       direction of control is not allowed in this function.                      */
}

void UART5_FAULT_InterruptHandler (void)
{
    /* Disable the fault interrupt */
    IEC2CLR = _IEC2_U5EIE_MASK;
    /* Disable the receive interrupt */
    IEC2CLR = _IEC2_U5RXIE_MASK;

    /* Client must call UARTx_ErrorGet() function to clear the errors */
    if( uart5Obj.rdCallback != NULL )
    {
        uart5Obj.rdCallback(UART_EVENT_READ_ERROR, uart5Obj.rdContext);
    }
}

void UART5_RX_InterruptHandler (void)
{
    /* Clear UART5 RX Interrupt flag */
    IFS2CLR = _IFS2_U5RXIF_MASK;

    /* Keep reading until there is a character availabe in the RX FIFO */
    while((U5STA & _U5STA_URXDA_MASK) == _U5STA_URXDA_MASK)
    {
        if (UART5_RxPushByte( (uint8_t )(U5RXREG) ) == true)
        {
            UART5_ReadNotificationSend();
        }
        else
        {
            /* UART RX buffer is full */
        }
    }
}

void UART5_TX_InterruptHandler (void)
{
    uint8_t wrByte;

    /* Check if any data is pending for transmission */
    if (UART5_WritePendingBytesGet() > 0)
    {
        /* Clear UART5TX Interrupt flag */
        IFS2CLR = _IFS2_U5TXIF_MASK;

        /* Keep writing to the TX FIFO as long as there is space */
        while(!(U5STA & _U5STA_UTXBF_MASK))
        {
            if (UART5_TxPullByte(&wrByte) == true)
            {
                U5TXREG = wrByte;

                /* Send notification */
                UART5_WriteNotificationSend();
            }
            else
            {
                /* Nothing to transmit. Disable the data register empty interrupt. */
                UART5_TX_INT_DISABLE();
                break;
            }
        }
	}
    else
    {
        /* Nothing to transmit. Disable the data register empty interrupt. */
        UART5_TX_INT_DISABLE();
    }
}

