/* ************************************************************************** */
/** laser controller

  @Summary
    Brief description of the file.
 */
/* ************************************************************************** */

#include "laserDescriptor.h"

/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */


/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
 */
int global_data;

/* ************************************************************************** */
// Functions                                                   */
/* ************************************************************************** */

/**
 * @param length Size of the array that must be sent
 */
void moduleControlTask(LaserModule *module) {
    switch (module->state) {
        case UNINITIALISED:
            //TODO for now we just wait the communication task start the initialisation
            break;
        case INITIALISED_OFF:
            //TODO for now wait for external command   
            break;
        default:
            break;
    }
}

/* *****************************************************************************
 End of File
 */
