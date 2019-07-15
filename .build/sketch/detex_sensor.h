
/* This file allows use of onboard and external sensors with MXChip.
Included features are:
    - Initialisation of sensors
    - Communication with sensors
    - Read sensors

Made by DeteX
*/

#ifndef DETEX_SENSOR_H
#define DETEX_SENSOR_H

#include "Sensor.h"
#include "ST_INTERFACES\DevI2C.h"
#include "parson.h"


/*******************************************************/ 
/* Onboard sensors *************/
/*
@Brief: Initializes onboard sensors for MXChip
@Param: None
@Return: None
*/
void init_onboard_sensors(void);

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

/*
@Brief: Read onboard pressure sensor for MXChip
@Param: None
@Return: None
*/
float read_pressure(void);

/*
@Brief: Read onboard magnetic sensor for MXChip
@Param: None
@Return: None
*/
int read_magnetic(void);

#endif /* DETEX_SENSOR_H */