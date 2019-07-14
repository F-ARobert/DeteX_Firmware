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

  /* Sensor intialization */
  init_onboard_temp_sensor();

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