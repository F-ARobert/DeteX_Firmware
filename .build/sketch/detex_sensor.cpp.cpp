#include <Arduino.h>
#line 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
#line 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
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
#line 25 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
void init_onboard_sensors(void);
#line 48 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
float read_temperature(void);
#line 57 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
float read_humidity(void);
#line 67 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
float read_pressure(void);
#line 75 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
int read_magnetic(void);
#line 28 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
void setup();
#line 53 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
void loop();
#line 25 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
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
#line 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"


#include"detex_sensor.h"


static bool hasWifi = false;
static bool hasIoTHub = false;

/* Variables *****************************************/
typedef struct telemetry_data {
    float temperature;
    float humidity;
    float pressure;
    int16_t mag_field;
}telemetry_data;

telemetry_data t_data;

char line1[20];
char line2[20];
char line3[20];

void setup() {
  // put your setup code here, to run once:

  /* Sensor intialization */
  init_onboard_sensors();

  if (WiFi.begin() == WL_CONNECTED)
  {
    hasWifi = true;
    Screen.print(1, "Running...");

    if (!DevKitMQTTClient_Init())
    {
      hasIoTHub = false;
      return;
    }
    hasIoTHub = true;
  }
  else
  {
    hasWifi = false;
    Screen.print(1, "No Wi-Fi");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Screen.clean();
  Screen.print(1,"Fetching data", true);

  t_data.temperature = read_temperature();
  t_data.humidity = read_humidity();
  t_data.pressure = read_pressure();
  t_data.mag_field = read_magnetic();

  delay(2000);

  sprintf(line1, "%.2f Celsius", t_data.temperature);
  sprintf(line2,"%.2f %",t_data.humidity);

  Screen.print(1,line1,false);
  Screen.print(2,line2,false);
  
  delay(2000);

  sprintf(line1, "%.2f Pa", t_data.pressure);
  sprintf(line2,"%.2d %",t_data.mag_field);

  Screen.print(1,line1,false);
  Screen.print(2,line2,false);

  delay(2000);
  /*if (hasIoTHub && hasWifi)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
    snprintf(buff, 128, "{\"topic\":\"iot\"}");
    
    if (DevKitMQTTClient_SendEvent(buff))
  }*/
}
