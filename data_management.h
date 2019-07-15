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

/* Static variables *************************/
const enum error {NO_ERROR,ERROR};

static typedef struct telemetry_table {
    int8_t count;
    float temperature_table[12];
    float pressure_table[12];
    float humidity_table[12];
    int32_t magnetic_table[12];
}telemetry_table_t;


/* Calculate average ***********************/
/* Initialize telemtry tables **************/
/*
@Brief: Creates empty telemetry_table struct
@Param: None
@Return: Error message
 */
int8_t telemetry_init(void);


/*
@Brief: Calculates the averages of tables in a telemetry table struct
@Param: A telemetry structure
@Return: Error message
 */
int8_t calc_average(telemetry_table_t *structure);

/* Reads all telemetry sensors ************/
int8_t read_sensors(void);







#endif