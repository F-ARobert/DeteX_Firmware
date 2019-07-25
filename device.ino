#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"
#include "mbed.h" 

#include "data_management.h"


static bool hasWifi = false;
static bool hasIoTHub = false;


/* Global variable ****************************/
telemetry_table_t tele_tab;
telemetry_data_t t_data;
bool first_pass = true;
    float pressure = 0;
    float temperature = 0;
    float humidity = 0;
    int mag_field = 0;

/* Initialize timers */
Timer sensor_timer;
Timer lidar_timer;
Timer timing;
int lidar_time;
int sensor_time;
int timing_time;

/* Test variables */
char line1[20];
char line2[20];
char line3[20];
char line4[20];
bool sensor_flag = true; 


void setup() {
  pinMode(LED_USER, OUTPUT);
  /* Sensor intialization */
  init_onboard_sensors();
  tele_tab = telemetry_init();
  
  /* if (WiFi.begin() == WL_CONNECTED)
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
  } */


}

void loop() {
  // put your main code here, to run repeatedly:
  if (first_pass) {
    sensor_timer.start();
    lidar_timer.start();
    timing.start();
    first_pass = false;
  }

  

  
  if (1 <= (lidar_timer.read_ms()/100)) {
    lidar_time = lidar_timer.read_ms();
    lidar_timer.reset();
  }



  if (1 <= (sensor_timer.read()/5)) {
    read_sensors(&tele_tab);
    sensor_timer.reset();
  }

  if (tele_tab.count == 12){
    calc_average(&t_data, &tele_tab);
    /*t_data.temperature = tele_tab.sum_temperature/tele_tab.count;
    t_data.humidity = tele_tab.sum_humidity/tele_tab.count;
    t_data.pressure = tele_tab.sum_pressure/tele_tab.count;
    t_data.mag_field.x = tele_tab.sum_magnetic.x/tele_tab.count;
    t_data.mag_field.y = tele_tab.sum_magnetic.y/tele_tab.count;
    t_data.mag_field.z = tele_tab.sum_magnetic.z/tele_tab.count;*/

    /* For testing purposes */

    Serial.printf("---------- New Output ----------------\n");
    Serial.printf("Lidar timer read : %d\n\n", lidar_time);

    Serial.printf("%.2f Celsius\n", t_data.temperature);
    Serial.printf("%.2f %% humidity\n",t_data.humidity);
    Serial.printf("%.2f Pa\n", t_data.pressure);
    Serial.printf("Magnetic field: x %d, y %d, z %d\n\n", t_data.mag_field.x, t_data.mag_field.y,t_data.mag_field.z);
    /*sprintf(line1, "%.2f Celsius", t_data.temperature);
    sprintf(line2,"%.2f %%",t_data.humidity);
    sprintf(line3, "%.2f Pa", t_data.pressure);
    sprintf(line4,"x %d, y %d, z %d", t_data.mag_field.x, t_data.mag_field.y,t_data.mag_field.z);

    Screen.print(0,line1,false);
    Screen.print(1,line2,false);
    Screen.print(2,line3,false);
    Screen.print(3,line4,true);*/
    /* End testing */

    tele_tab = telemetry_init();
  }
 
  /*if (hasIoTHub && hasWifi)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
    snprintf(buff, 128, "{\"topic\":\"iot\"}");
    
    if (DevKitMQTTClient_SendEvent(buff))
  }*/

}