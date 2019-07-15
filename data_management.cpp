/* This file controls data received from various sensors linked to MXChip
Included features are:
    - Sorting data
    - calculate average
    - Store data

Made by DeteX
*/

#include "data_management.h"

/* Static variables *************************/
//static telemetry_table_t tele_tab;

/* Initialize telemetry data ***************/
telemetry_table_t telemetry_init(){
    int8_t i;
    telemetry_table_t tele_tab;
    
    tele_tab.count = 0;
    tele_tab.sum_humidity = 0;
    tele_tab.sum_temperature = 0;
    tele_tab.sum_magnetic = 0;
    tele_tab.sum_pressure = 0;

    memset(&tele_tab.temperature_table,0,NUMBER_OF_OBSERVATIONS*sizeof(float));
    memset(&tele_tab.pressure_table,0,NUMBER_OF_OBSERVATIONS*sizeof(float));
    memset(&tele_tab.humidity_table,0,NUMBER_OF_OBSERVATIONS*sizeof(float));
    memset(&tele_tab.magnetic_table,0,NUMBER_OF_OBSERVATIONS*sizeof(int32_t));

    return tele_tab;
}

/* Calculate average **********************/
int8_t calc_average(telemetry_data_t *ptr_data, telemetry_table_t *ptr_table){
    ptr_data->temperature = (ptr_table->sum_temperature/ptr_table->count);
    ptr_data->humidity = (ptr_table->sum_humidity/ptr_table->count);
    ptr_data->pressure = (ptr_table->sum_pressure/ptr_table->count);
    ptr_data->mag_field = (ptr_table->sum_magnetic/ptr_table->count);
    
    if (ptr_data->temperature == 0 || ptr_data->pressure == 0 || ptr_data->mag_field == 0 || ptr_data->humidity == 0){
        return DATA_MNGMT_ERROR;
    } else {

    telemetry_init(); /* reinitialize structure for next batch */
    return NO_ERROR;
    }
    
}

/* Read sensors ***************************/
void read_sensors(void){
    float pressure = read_pressure();
    float temperature = read_temperature();
    float humidity = read_humidity();
    int32_t mag_field = read_magnetic();


    tele_tab.humidity_table[tele_tab.count] = humidity;
    tele_tab.temperature_table[tele_tab.count] = temperature;
    tele_tab.pressure_table[tele_tab.count] = pressure;
    tele_tab.magnetic_table[tele_tab.count] = mag_field;

    tele_tab.sum_humidity += humidity;
    tele_tab.sum_magnetic += mag_field;
    tele_tab.sum_pressure += pressure;
    tele_tab.sum_temperature += temperature;
}





