/**
 * File:  laserCommands 
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LASERCOMMANDS_H
#define	LASERCOMMANDS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "laserDescriptor.h"

#define LENGTH_SHORT 12
#define LENGTH_STANDARD 16
#define OPEN_ANSWER_LENGTH 16
#define MEASURE_DATA_LENGTH 24
#define LENGTH_20 20


struct {
    uint8_t open[16];
    uint8_t reset[16];
    uint8_t startContinuousMeas[16];
    uint8_t stopMeasure[16]; 
    uint8_t setStrength_1[16];
    uint8_t setStrength_2[20];
    uint8_t getStatus [12];
} commandsSend = {
    {0xFA, 0xFA, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0xC6, 0x7E, 0xBD, 0xBA },
    {0xFA, 0xFA, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0xE4, 0xD0, 0x66 },
    {0xFA, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x9B, 0x85, 0x04 },
    {0xFA, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x01, 0xE8, 0xD8 },
    {0xEA, 0x15, 0xFE, 0x00, 0xFC, 0xFF, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x46, 0x6e, 0xB9, 0xAB },
    {0xEA, 0x10, 0xFE, 0x00, 0xFC, 0xFF, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x57, 0x00, 0x03, 0x00, 0x25, 0xCB, 0x23, 0x97},
    {0xFA, 0x04, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xEF, 0x63, 0x29, 0x04}
};

#endif	/* XC_HEADER_TEMPLATE_H */

