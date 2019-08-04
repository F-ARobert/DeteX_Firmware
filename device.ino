#include <string.h>
#include <time.h>

#include "Arduino.h"
#include "pins_arduino.h"

#include "AZ3166WiFi.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"
#include "BufferedSerial.h"
#include "PinNames.h"
#include "Ticker.h"


#include "protobuf_communication.h"



/* DEFINES */
#define LIDAR_ON 0

/* Global variable ****************************/
telemetry_table_t tele_tab;
telemetry_data_t t_data;
lidar_data_t lidar_data;

/* LIDAR SET UP */
// You need to create an driver instance
RPLidar lidar;
bool lidar_on = false;
char* line1;
//Set-up le premier tour
int i_loop = 0;         //Devrait finir a 2000
int i_tab = 0;          //Doit s'arreter a POINT_TAB_COMPLET
/* END LIDAR SET UP */

static bool hasWifi = false;
static bool hasIoTHub = false;

/* Initialize tickers */
Ticker lidar_send;
Ticker sensors_read;

Timer lidar_timer;
int sensors_read_count;
int lidar_time;

/* Global functions */
void lidar_time_read(void);
void read_all_sensors(void);

/* Assign function pointers */
void (*read_sensors_ptr)(void) = &read_all_sensors;
void (*lidar_time_read_ptr)(void) = &lidar_time_read;


void setup() {
  #if LIDAR_ON
  Serial.begin(115200);
  pinMode(PB_3, OUTPUT);
  analogWrite(PB_3,255);
  lidar_on = lidar.begin(Serial);
  lidar_data = lidar_data_init();
  #endif

  /* Sensor intialization */
  init_onboard_sensors();
  tele_tab = telemetry_init();
  Screen.print(1,"init ok",true);
  delay(1500);
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

  /* Set up iunterupts and timers */
  //lidar_timer.start();
  //lidar_send.attach(lidar_time_read_ptr,0.1);
  sensors_read.attach(read_sensors_ptr,5.0);
  Screen.clean();
}

void loop() {
  // put your main code here, to run repeatedly:
  #if LIDAR_ON
  if (lidar_on){
    run_lidar(lidar, lidar_data,&i_loop, &i_tab);
  } else {
    Screen.print(1,"NO LIDAR");
  }
  #endif

  sprintf(line1, "sensors %d",sensors_read_count);
  Screen.print(1,line1,true);
  
  #if LIDAR_ON
  if (lidar_data.startbit){
    
  } else {
    Screen.print(1,"NO DATA");
  }
  #endif
  
/*  if (hasIoTHub && hasWifi){
    send_telemetry(t_data);
    send_lidar(lidar_data);
   }*/

  if (tele_tab.count == 12){
    calc_average(&t_data, &tele_tab);
    tele_tab = telemetry_init();
  }

}

/********* TO PUT UN SEPERATE HEADER FILE ********************/
void lidar_time_read(void){
  lidar_time = lidar_timer.read_ms();
  //Serial.printf("Lidar timer read %d",lidar_time);
  lidar_timer.reset();
}

void read_all_sensors(void){
  sensors_read_count++;
  read_sensors(&tele_tab);
}