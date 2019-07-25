/* This file controls data received from various sensors linked to MXChip
Included features are:
    - Sorting data
    - calculate average
    - Store data

Made by DeteX
*/

#ifndef DATA_MANAGEMENT_H
#define DATA_MANAGEMENT_H

#include "detex_sensor.h"

#define NUMBER_OF_OBSERVATIONS 12

/* ENUM ************************************************/
enum error {NO_ERROR,DATA_MNGMT_ERROR};

/* Strucutres *****************************************/
typedef struct telemetry_data {
    float temperature;
    float humidity;
    float pressure;
    mag_field_t mag_field;
}telemetry_data_t;

typedef struct telemetry_table {
    int8_t count;
    //float temperature_table[NUMBER_OF_OBSERVATIONS];
    float sum_temperature;
    //float pressure_table[NUMBER_OF_OBSERVATIONS];
    float sum_pressure;
    //float humidity_table[NUMBER_OF_OBSERVATIONS];
    float sum_humidity;
    //mag_field_t magnetic_table[NUMBER_OF_OBSERVATIONS];
    mag_field_t sum_magnetic;
}telemetry_table_t;

/* External variables ***************************/
//extern telemetry_data_t t_data;

/* Calculate average ***********************/
/* Initialize telemtry tables **************/
/*
@Brief: Creates empty telemetry_table struct
@Param: None
@Return: Error message
 */
telemetry_table_t telemetry_init(void);

/*
@Brief: Calculates the averages of tables in a telemetry table struct
@Param: A telemetry data structure pointer, telemetry table structure pointer
@Return: Error message
 */
int8_t calc_average(telemetry_data_t *ptr_data, telemetry_table_t *ptr_table);

/*
@Brief: Reads data from all sensors
@Param: A telemetry table structure
@Return: Error message
 */
void read_sensors(telemetry_table_t *ptr);

#endif