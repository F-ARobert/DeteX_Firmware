#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"


#include "timing_mngmt.h"


static bool hasWifi = false;
static bool hasIoTHub = false;


/* Global variable ****************************/
telemetry_table_t tele_tab;
telemetry_data_t t_data;

/* Initialize tickers */
Ticker lidar_send;
Ticker sensors_read;

Timer lidar_timer;

int lidar_time;

/* Global functions */
void lidar_time_read(void);
void read_all_sensors(void);

/* Assign function pointers */
void (*read_sensors_ptr)(void) = &read_all_sensors;
void (*lidar_time_read_ptr)(void) = &lidar_time_read;


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

  /* Set up iunterupts and timers */
  lidar_timer.start();
  lidar_send.attach(lidar_time_read_ptr,0.1);
  sensors_read.attach(read_sensors_ptr,5.0);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (tele_tab.count == 12){
    calc_average(&t_data, &tele_tab);

    /* For testing purposes */
    Serial.printf("---------- New Output ----------------\n");
    Serial.printf("Lidar timer read : %d\n\n", lidar_time);

    Serial.printf("%.2f Celsius\n", t_data.temperature);
    Serial.printf("%.2f %% humidity\n",t_data.humidity);
    Serial.printf("%.2f Pa\n", t_data.pressure);
    Serial.printf("Magnetic field: x %d, y %d, z %d\n\n", t_data.mag_field.x, t_data.mag_field.y,t_data.mag_field.z);

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

/********* TO PUT UN SEPERATE HEADER FILE ********************/
void lidar_time_read(void){
  lidar_time = lidar_timer.read_ms();
  lidar_timer.reset();
}

void read_all_sensors(void){
  Serial.printf("Lidar timer read : %d\n", lidar_time);
  read_sensors(&tele_tab);
}