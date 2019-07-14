
/* This file allows use of onboard and external sensors with MXChip.
Included features are:
    - Initialisation of sensors
    - Communication with sensors
    - Read commands

Made by DeteX
*/

#ifndef DETEX_SENSOR_H
#define DETEX_SENSOR_H

#include "Sensor.h"
#include "ST_INTERFACES\DevI2C.h"
#include "parson.h"


/*******************************************************/ 
/* Onboard temperature sensor */

/*
@Brief: Initializes onboard temperature sensor for MXChip
@Param: None
@Return: None
 */
void init_onboard_temp_sensor(void);


/*
@Brief: Read temperature data
@Param: None
@Return: float temperature
 */
float read_temperature(void);

/*
@Brief: Read humidity data
@Param: None
@Return: float temperature
 */
float read_humidity(void);


#endif /* DETEX_SENSOR_H */