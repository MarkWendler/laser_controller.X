/** Laser modules's addresses
  @Description
 Define Laser can adresses to pisical modules;
 */

#ifndef LASER_ADDRESSES_H    /* Guard against multiple inclusion */
#define LASER_ADDRESSES_H


#define BROADCAST 0xFF
#define DEBUG_ADDR 0xF0
//LASERCONTROLLER module's number.
#define LASERCONTROLLER 1

#if LASERCONTROLLER == 1
#define LASER_ADDRESS_1 1
#define LASER_ADDRESS_2 2
#define LASER_ADDRESS_3 3
#define LASER_ADDRESS_4 4
#define LASER_ADDRESS_5 5
#define LASER_ADDRESS_6 6

#elif LASERCONTROLLER == 2
#define LASER_ADDRESS_1 7
#define LASER_ADDRESS_2 8
#define LASER_ADDRESS_3 9
#define LASER_ADDRESS_4 10
#define LASER_ADDRESS_5 11
#define LASER_ADDRESS_6 12

#elif LASERCONTROLLER == 3
#define LASER_ADDRESS_1 13
#define LASER_ADDRESS_2 14
#define LASER_ADDRESS_3 15
#define LASER_ADDRESS_4 16
#define LASER_ADDRESS_5 17
#define LASER_ADDRESS_6 18

#elif LASERCONTROLLER == 4
#define LASER_ADDRESS_1 19
#define LASER_ADDRESS_2 20
#define LASER_ADDRESS_3 21
#define LASER_ADDRESS_4 22
#define LASER_ADDRESS_5 23
#define LASER_ADDRESS_6 24

#elif LASERCONTROLLER == 5
#define LASER_ADDRESS_1 25
#define LASER_ADDRESS_2 26
#define LASER_ADDRESS_3 27
#define LASER_ADDRESS_4 28
#define LASER_ADDRESS_5 29
#define LASER_ADDRESS_6 30

#elif LASERCONTROLLER == 6
#define LASER_ADDRESS_1 31
#define LASER_ADDRESS_2 32
#define LASER_ADDRESS_3 33
#define LASER_ADDRESS_4 34
#define LASER_ADDRESS_5 35
#define LASER_ADDRESS_6 36

#endif


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
