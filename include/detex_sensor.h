
/* This file allows use of onboard and external sensors with MXChip.
Included features are:
    - Initialisation of sensors
    - Communication with sensors
    - Read commands

Made by DeteX
*/

#ifndef DETEX_SENSOR_H
#define DETEX_SENSOR_H

#include"sensor.h"
#include"DevI2C.h"
#include"parson.h"


/*******************************************************/ 
/* Onboard temperature sensor */

/*
@Brief: Initializes onboard temperature sensor for MXChip
@Param: None
@Return: None
 */
void InitOnboardTempSensor(void);





#endif /* DETEX_SENSOR_H */