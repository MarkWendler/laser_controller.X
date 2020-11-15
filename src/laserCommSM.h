/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "laserDescriptor.h"

/**
 * @parameter pvParameter pointer to LaserModule instance
 */

void vLaserCommTask(void* pvParameter); //LaserModule

typedef enum {
    ERROR_TO_CAN = 0x11,    // Message from CAN
    ALARM_TO_CAN = 0x01,    // Alarm event from distance measure task
} EnumToCANEventType_t;

typedef struct {
    EnumToCANEventType_t eventType;
    uint8_t id;   
} ToCANEvent_t;

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
