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
static RGB_LED rgbLed;
static float humidity;
static float temperature;



#line 21 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
void init_onboard_temp_sensor(void);
#line 31 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
float read_temperature(void);
#line 40 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
float read_humidity(void);
#line 26 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
void setup();
#line 47 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
void loop();
#line 21 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
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
    float mag_field;
}telemetry_data;

telemetry_data t_data;

char line1[20];

void setup() {
  // put your setup code here, to run once:
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

  Screen.print(1,"Fetching temp", true);

  delay(2000);

  t_data.temperature = read_temperature();

  Screen.print(1, "Temprature :",false);
  sprintf(line1, "%f Celsius", t_data.temperature);
  Screen.print(2,line1,false);

  delay(5000);

  /*if (hasIoTHub && hasWifi)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
    snprintf(buff, 128, "{\"topic\":\"iot\"}");
    
    if (DevKitMQTTClient_SendEvent(buff))
  }*/
}
