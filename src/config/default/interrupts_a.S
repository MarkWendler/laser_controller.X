/*******************************************************************************
  System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include "ISR_Support.h"

    .extern  TIMER_2_Handler

    .section   .vector_9,code, keep
    .equ     __vector_dispatch_9, IntVectorTIMER_2_Handler
    .global  __vector_dispatch_9
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorTIMER_2_Handler

IntVectorTIMER_2_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_2_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorTIMER_2_Handler
    .extern  UART1_FAULT_Handler

    .section   .vector_38,code, keep
    .equ     __vector_dispatch_38, IntVectorUART1_FAULT_Handler
    .global  __vector_dispatch_38
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART1_FAULT_Handler

IntVectorUART1_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART1_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART1_FAULT_Handler
    .extern  UART1_RX_Handler

    .section   .vector_39,code, keep
    .equ     __vector_dispatch_39, IntVectorUART1_RX_Handler
    .global  __vector_dispatch_39
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART1_RX_Handler

IntVectorUART1_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART1_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART1_RX_Handler
    .extern  UART1_TX_Handler

    .section   .vector_40,code, keep
    .equ     __vector_dispatch_40, IntVectorUART1_TX_Handler
    .global  __vector_dispatch_40
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART1_TX_Handler

IntVectorUART1_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART1_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART1_TX_Handler
    .extern  UART2_FAULT_Handler

    .section   .vector_56,code, keep
    .equ     __vector_dispatch_56, IntVectorUART2_FAULT_Handler
    .global  __vector_dispatch_56
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART2_FAULT_Handler

IntVectorUART2_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART2_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART2_FAULT_Handler
    .extern  UART2_RX_Handler

    .section   .vector_57,code, keep
    .equ     __vector_dispatch_57, IntVectorUART2_RX_Handler
    .global  __vector_dispatch_57
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART2_RX_Handler

IntVectorUART2_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART2_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART2_RX_Handler
    .extern  UART2_TX_Handler

    .section   .vector_58,code, keep
    .equ     __vector_dispatch_58, IntVectorUART2_TX_Handler
    .global  __vector_dispatch_58
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART2_TX_Handler

IntVectorUART2_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART2_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART2_TX_Handler
    .extern  UART3_FAULT_Handler

    .section   .vector_62,code, keep
    .equ     __vector_dispatch_62, IntVectorUART3_FAULT_Handler
    .global  __vector_dispatch_62
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART3_FAULT_Handler

IntVectorUART3_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART3_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART3_FAULT_Handler
    .extern  UART3_RX_Handler

    .section   .vector_63,code, keep
    .equ     __vector_dispatch_63, IntVectorUART3_RX_Handler
    .global  __vector_dispatch_63
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART3_RX_Handler

IntVectorUART3_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART3_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART3_RX_Handler
    .extern  UART3_TX_Handler

    .section   .vector_64,code, keep
    .equ     __vector_dispatch_64, IntVectorUART3_TX_Handler
    .global  __vector_dispatch_64
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART3_TX_Handler

IntVectorUART3_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART3_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART3_TX_Handler
    .extern  UART4_FAULT_Handler

    .section   .vector_65,code, keep
    .equ     __vector_dispatch_65, IntVectorUART4_FAULT_Handler
    .global  __vector_dispatch_65
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART4_FAULT_Handler

IntVectorUART4_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART4_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART4_FAULT_Handler
    .extern  UART4_RX_Handler

    .section   .vector_66,code, keep
    .equ     __vector_dispatch_66, IntVectorUART4_RX_Handler
    .global  __vector_dispatch_66
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART4_RX_Handler

IntVectorUART4_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART4_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART4_RX_Handler
    .extern  UART4_TX_Handler

    .section   .vector_67,code, keep
    .equ     __vector_dispatch_67, IntVectorUART4_TX_Handler
    .global  __vector_dispatch_67
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART4_TX_Handler

IntVectorUART4_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART4_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART4_TX_Handler
    .extern  UART5_FAULT_Handler

    .section   .vector_68,code, keep
    .equ     __vector_dispatch_68, IntVectorUART5_FAULT_Handler
    .global  __vector_dispatch_68
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART5_FAULT_Handler

IntVectorUART5_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART5_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART5_FAULT_Handler
    .extern  UART5_RX_Handler

    .section   .vector_69,code, keep
    .equ     __vector_dispatch_69, IntVectorUART5_RX_Handler
    .global  __vector_dispatch_69
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART5_RX_Handler

IntVectorUART5_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART5_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART5_RX_Handler
    .extern  UART5_TX_Handler

    .section   .vector_70,code, keep
    .equ     __vector_dispatch_70, IntVectorUART5_TX_Handler
    .global  __vector_dispatch_70
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART5_TX_Handler

IntVectorUART5_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART5_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART5_TX_Handler
    .extern  UART6_FAULT_Handler

    .section   .vector_164,code, keep
    .equ     __vector_dispatch_164, IntVectorUART6_FAULT_Handler
    .global  __vector_dispatch_164
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART6_FAULT_Handler

IntVectorUART6_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART6_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART6_FAULT_Handler
    .extern  UART6_RX_Handler

    .section   .vector_165,code, keep
    .equ     __vector_dispatch_165, IntVectorUART6_RX_Handler
    .global  __vector_dispatch_165
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART6_RX_Handler

IntVectorUART6_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART6_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART6_RX_Handler
    .extern  UART6_TX_Handler

    .section   .vector_166,code, keep
    .equ     __vector_dispatch_166, IntVectorUART6_TX_Handler
    .global  __vector_dispatch_166
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART6_TX_Handler

IntVectorUART6_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART6_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART6_TX_Handler

