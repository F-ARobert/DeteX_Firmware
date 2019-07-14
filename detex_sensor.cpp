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
LPS22HBSensor *pressure_sensor;
LIS2MDLSensor *mag_sensor;
static RGB_LED rgbLed;
static float humidity;
static float temperature;
static float pressure;
static int mag_field;


/* Temeprature and humidity *****************/
void init_onboard_sensors(void)
{
    i2c = new DevI2C(D14, D15);

    /* Temperature and humidity */
    HT_sensor = new HTS221Sensor(*i2c);
    HT_sensor->init(NULL);

    /* Pressure */
    pressure_sensor = new LPS22HBSensor(*i2c);
    pressure_sensor->init(NULL);

    /* Magnetometer */
    mag_sensor = new LIS2MDLSensor(*i2c);
    mag_sensor->init(NULL);

    humidity = -1;
    temperature = -1000;
    pressure = 0;
    mag_field = 0;
}

/* Temeprature and humidity *****************/
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

/* Pressure *******************************/
float read_pressure(void){
    float pressure = 0;
    pressure_sensor->getPressure(&pressure);

    return pressure;
}

/* Magnetometer *************************/
int read_magnetic(void){
    int mag_field = 0;
    mag_sensor->getMAxes(&mag_field);

    return mag_field;
}