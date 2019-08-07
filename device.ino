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


#include "protobuf_communication.h" // non fonctionnel



/* DEFINES */
#define LIDAR_ON 1
#define DISPLAY_TELEMETRY 1
#define LIGHT_DEMONSTRATION 1
#define INTERNAL_LIGHT_LOGIC 1

#define PIN_GREEN PC_13
#define PIN_YELLOW PB_0
#define PIN_RED PB_2

//typedef enum {GREEN, YELLOW, RED};

/* Global variable ****************************/
telemetry_table_t tele_tab;
telemetry_data_t t_data;
lidar_data_t lidar_data;

/* LIDAR SET UP */
// You need to create an driver instance
RPLidar lidar;
bool lidar_on = false;
char line1[20];
char line2[20];
char line3[20];
char line4[20];
//Set-up le premier tour
int i_loop = 0;         //Devrait finir a 2000
int i_tab = 0;          //Doit s'arreter a POINT_TAB_COMPLET
/* END LIDAR SET UP */
uint8_t state = GREEN;

static bool hasWifi = false;
static bool hasIoTHub = false;

/* Initialize tickers */
Ticker lidar_send;
Ticker sensors_read;
Ticker lidar_show;

Timer lidar_timer;
int sensors_read_count;
int lidar_time;

/* Global functions */
void lidar_time_read(void);
void read_all_sensors(void);
void show_lidar_data(void);

/* Assign function pointers */
void (*read_sensors_ptr)(void) = &read_all_sensors;
void (*lidar_time_read_ptr)(void) = &lidar_time_read;
void (*lidar_show_ptr)(void) = &show_lidar_data;

bool turn = false;

void setup() {


  #if LIDAR_ON
  Serial.begin(115200);
  pinMode(PB_3, OUTPUT);
  analogWrite(PB_3,255);
  lidar_on = lidar.begin(Serial);
  lidar_data = lidar_data_init();
  #endif

  /* Light pinout */
  pinMode(PIN_GREEN,OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_RED,OUTPUT);

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

  /* Set up iunterupts and timers */
  lidar_timer.start();
  lidar_send.attach(lidar_time_read_ptr,0.5);
  sensors_read.attach(read_sensors_ptr,5.0);
  lidar_show.attach(lidar_show_ptr,0.5);
  Screen.clean();
}

void loop() {
  // put your main code here, to run repeatedly:
  #if LIDAR_ON
  if (lidar_on){
    run_lidar(lidar, &lidar_data,&i_loop, &i_tab);
  } else {
    Screen.print(1,"NO LIDAR",false);
  }
  #endif
  
  #if LIGHT_DEMONSTRATION

    // Logique des lumieres.
    if((lidar_data.state == RED) && (DEFAULT_MIN_DIST != lidar_data.distance_min)){
      digitalWrite(PIN_RED,HIGH);
      digitalWrite(PIN_YELLOW,LOW);
      digitalWrite(PIN_GREEN,LOW);
    }
    else if((lidar_data.state == YELLOW) && (DEFAULT_MIN_DIST != lidar_data.distance_min)){
      digitalWrite(PIN_YELLOW,HIGH);
      digitalWrite(PIN_GREEN,LOW);
      digitalWrite(PIN_RED,LOW);
    }
    else if(lidar_data.state == GREEN){
      digitalWrite(PIN_GREEN,HIGH);
      digitalWrite(PIN_YELLOW,LOW);
      digitalWrite(PIN_RED,LOW);
    }
    
  #endif

  
/*  if (hasIoTHub && hasWifi){
    send_telemetry(t_data);
    send_lidar(lidar_data);
   }*/

  
  if (tele_tab.count == 12){
    calc_average(&t_data, &tele_tab);
    #if DISPLAY_TELEMETRY
    Screen.clean();
    sprintf(line1,"%.2f C",t_data.temperature);
    sprintf(line2,"%.2f %% H",t_data.humidity);
    sprintf(line3,"%.2f Pa", t_data.pressure);
    sprintf(line4,"x %3d, y %3d, z %3d", t_data.mag_field.x, t_data.mag_field.y,t_data.mag_field.z);

    if (true == turn){
      Screen.print(1,line1,false);
      Screen.print(2,line2,false);
      turn = !turn;
    } else{
      Screen.print(1,line3,false);
      Screen.print(2,line4,true);
      turn=!turn;
    }
  #endif
    tele_tab = telemetry_init();
  }

}

/********* operating functions: should be in seperate header file ********************/
void lidar_time_read(void){
  lidar_time = lidar_timer.read_ms();
  /* if (turn){
    digitalWrite(PIN_RED,HIGH);
    digitalWrite(PIN_YELLOW,LOW);
    turn = !turn;
  }else{
    digitalWrite(PIN_RED,LOW);
    digitalWrite(PIN_YELLOW,HIGH);
    turn = !turn;
  }*/
  send_lidar(lidar_data);

  //Serial.printf("Lidar timer read %d",lidar_time);
  lidar_timer.reset();
}

void read_all_sensors(void){
  read_sensors(&tele_tab);
}

void show_lidar_data(void){
 #if LIDAR_ON
  
  sprintf(line1, "%3.0f mm", lidar_data.distance[i_tab]);
  sprintf(line2, "%3.0f dist min", lidar_data.distance_min);
  //sprintf(line3, "%2.0u obj. detect.", lidar_data.nombre_obj);

  Screen.print(1, line1, true);
  Screen.print(2, line2, true);
  //Screen.print(3, line3, true);
  
  // Logique interne des lumieres.
  if((lidar_data.state == RED) && (DEFAULT_MIN_DIST != lidar_data.distance_min)){
    Screen.print(0,"RED",false);// turn on red light.
  }
  else if((lidar_data.state == YELLOW) && (DEFAULT_MIN_DIST != lidar_data.distance_min)){
    Screen.print(0,"YELLOW",false); // turn on yellow light.
  }
  else{
    Screen.print(0,"GREEN",false); // Turn on green light
  }
  #endif
}