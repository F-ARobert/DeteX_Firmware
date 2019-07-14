/* This file allows use of onboard and external sensors with MXChip.
Included features are:
    - Initialisation of sensors
    - Communication with sensors
    - Read commands

Made by DeteX
*/


#include"detex_sensor.h"

/* VARIABLES ***********************************/
DevI2C *i2c;
HTS221Sensor *HT_sensor;
static RGB_LED rgbLed;
static float humidity;
static float temperature;



void init_onboard_temp_sensor(void)
{
    i2c = new DevI2C(D14, D15);
    HT_sensor = new HTS221Sensor(*i2c);
    HT_sensor->init(NULL);

    humidity = -1;
    temperature = -1000;
}

float read_temperature(void){
    HT_sensor->reset();

    float temperature = 0;
    HT_sensor->getTemperature(&temperature);

    return temperature;
}

float read_humidity(void){
    HT_sensor->reset();

    float humidity = 0;
    HT_sensor->getHumidity(&humidity);

    return humidity;
}

