#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"


#include "detex_sensor.h"
#include "data_management.h"


static bool hasWifi = false;
static bool hasIoTHub = false;


char line1[20];
char line2[20];
char line3[20];

void setup() {
  
  /* Sensor intialization */
  init_onboard_sensors();
  telemetry_table_t tele_tab = telemetry_init();

  /* Initialize interrupt timers */
  Timer sensor_timer;
  Timer lidar_timer;

  /* interrupt flags */
  bool pool_sensors = false;

  /* Attach */
  sensor_timer.initialize(5000);
  sensor_timer.attachInterrupt(read_sensors());

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
  sensor_timer.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  Screen.clean();

  if (pool_sensors == true){
    Screen.print(1,"Fetching new data", true);
    read_sensors();
  }

  if (t_data)
  

  t_data.temperature = read_temperature();
  t_data.humidity = read_humidity();
  t_data.pressure = read_pressure();
  t_data.mag_field = read_magnetic();

  delay(2000);

  sprintf(line1, "%.2f Celsius", t_data.temperature);
  sprintf(line2,"%.2f %%",t_data.humidity);

  Screen.print(1,line1,false);
  Screen.print(2,line2,false);
  
  delay(2000);

  sprintf(line1, "%.2f Pa", t_data.pressure);
  sprintf(line2,"%.2d ",t_data.mag_field);

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