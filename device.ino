#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"


#include "detex_sensor.h"
#include "data_management.h"


static bool hasWifi = false;
static bool hasIoTHub = false;


/* Global variable ****************************/
telemetry_table_t tele_tab;
telemetry_data_t t_data;

/* Initialize timers */
Timer sensor_timer;
Timer lidar_timer;
Timer timing;

/* Test variables */
char line1[20];
char line2[20];
char line3[20];
char line4[20];
bool sensor_flag = true; 


void setup() {
  
  /* Sensor intialization */
  init_onboard_sensors();
  tele_tab = telemetry_init();
  
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
  lidar_timer.start();
  timing.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (1 <= lidar_timer.read_ms()/100) {
    lidar_timer.reset();
  }

  if ( 1 <= sensor_timer.read()/5) {
    read_sensors(&tele_tab);
    sensor_timer.reset();
  }

  

  if (tele_tab.count == 12){
    t_data.temperature = tele_tab.sum_temperature/tele_tab.count;
    t_data.humidity = tele_tab.sum_humidity/tele_tab.count;
    t_data.pressure = tele_tab.sum_humidity/tele_tab.count;
    t_data.mag_field = tele_tab.sum_magnetic/tele_tab.count;
    tele_tab = telemetry_init();
  }
  
  if (tele_tab.count == 11) {
    Screen.print(1,"Prepare for new data",true);
  }
  

  sprintf(line1, "%.2f Celsius", t_data.temperature);
  sprintf(line2,"%.2f %%",t_data.humidity);
  sprintf(line3, "%.2f Pa", t_data.pressure);
  sprintf(line4,"%.2d",t_data.mag_field);

  Screen.print(0,line1,false);
  Screen.print(1,line2,false);
  Screen.print(2,line3,false);
  Screen.print(3,line4,false);

 
  /*if (hasIoTHub && hasWifi)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
    snprintf(buff, 128, "{\"topic\":\"iot\"}");
    
    if (DevKitMQTTClient_SendEvent(buff))
  }*/
}