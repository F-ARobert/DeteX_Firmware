#include <Arduino.h>
#line 1 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
#line 1 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
/* This file controls data received from various sensors linked to MXChip
Included features are:
    - Sorting data
    - calculate average
    - Store data

Made by DeteX
*/

#include "data_management.h"

/* Static variables *************************/
//static telemetry_table_t tele_tab;

/* Initialize telemetry data ***************/
#line 16 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
telemetry_table_t telemetry_init();
#line 37 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
int8_t calc_average(telemetry_data_t *ptr_data, telemetry_table_t *ptr_table);
#line 56 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
void read_sensors(telemetry_table_t *ptr);
#line 76 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
void setup();
#line 114 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
void loop();
#line 16 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
telemetry_table_t telemetry_init(){
    int8_t i;
    telemetry_table_t tele_tab;
    
    tele_tab.count = 0;
    tele_tab.sum_humidity = 0;
    tele_tab.sum_temperature = 0;
    tele_tab.sum_magnetic.x = 0;
    tele_tab.sum_magnetic.y = 0;
    tele_tab.sum_magnetic.z = 0;
    tele_tab.sum_pressure = 0;

    /*memset(&tele_tab.temperature_table,0,NUMBER_OF_OBSERVATIONS*sizeof(float));
    memset(&tele_tab.pressure_table,0,NUMBER_OF_OBSERVATIONS*sizeof(float));
    memset(&tele_tab.humidity_table,0,NUMBER_OF_OBSERVATIONS*sizeof(float));
    memset(&tele_tab.magnetic_table,0,NUMBER_OF_OBSERVATIONS*sizeof(int));*/

    return tele_tab;
}

/* Calculate average **********************/
int8_t calc_average(telemetry_data_t *ptr_data, telemetry_table_t *ptr_table){
    ptr_data->temperature = (ptr_table->sum_temperature/ptr_table->count);
    ptr_data->humidity = (ptr_table->sum_humidity/ptr_table->count);
    ptr_data->pressure = (ptr_table->sum_pressure/ptr_table->count);
    ptr_data->mag_field.x = (ptr_table->sum_magnetic.x/ptr_table->count);
    ptr_data->mag_field.y = (ptr_table->sum_magnetic.y/ptr_table->count);
    ptr_data->mag_field.z = (ptr_table->sum_magnetic.z/ptr_table->count);
    
    if (ptr_data->temperature == 0 || ptr_data->pressure == 0 || ptr_data->humidity == 0){
        return DATA_MNGMT_ERROR;
    } else {

    telemetry_init(); /* reinitialize structure for next batch */
    return NO_ERROR;
    }
    
}

/* Read sensors ***************************/
void read_sensors(telemetry_table_t *ptr){
    float pressure = read_pressure();
    float temperature = read_temperature();
    float humidity = read_humidity();
    mag_field_t mag_field = read_magnetic();

    /*ptr->humidity_table[ptr->count] = humidity;
    ptr->temperature_table[ptr->count] = temperature;
    ptr->pressure_table[ptr->count] = pressure;
    ptr->magnetic_table[ptr->count] = mag_field;*/

    ptr->sum_humidity += humidity;
    ptr->sum_magnetic.x += mag_field.x;
    ptr->sum_magnetic.y += mag_field.y;
    ptr->sum_magnetic.z += mag_field.z;
    ptr->sum_pressure += pressure;
    ptr->sum_temperature += temperature;

    ptr->count++;
}






#line 1 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"
#include "Serial.h"


#include "timing_mngmt.h"

// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include "RPLidarDriver/RPLidar.h"

/* DEFINES */
#define LIDAR_ON 0

#define ANGLE_DEBUT 0 //Debut de la couverture (en degrés)
#define ANGLE_FIN 180 //Fin de la couverture

#define DISTANCE_ROUGE 800 //en mm. 800 pour test, 1500 pour livraison
#define DISTANCE_JAUNE 1200 //4000 pour livraison

#define POINT_TAB_COMPLET 200

#define RPLIDAR_MOTOR 9 // The PWM pin for control the speed of RPLIDAR's motor. 
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal



#if LIDAR_ON
/* LIDAR SET UP */
// You need to create an driver instance
RPLidar lidar;

//Set-up le premier tour
float minDistance = 100000;
float angleAtMinDist = 0;

float tableau_complet_loop[POINT_TAB_COMPLET];
int i_loop = 0;         //Devrait finir a 2000
int nb_objet_loop = 0;
int i_tab = 0;          //Doit s'arreter a POINT_TAB_COMPLET
/* END LIDAR SET UP */
#endif

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
int nombre_objet(float tableau_complet[POINT_TAB_COMPLET]);

/* Assign function pointers */
void (*read_sensors_ptr)(void) = &read_all_sensors;
void (*lidar_time_read_ptr)(void) = &lidar_time_read;

#if LIDAR_ON

#endif


void setup() {

#if LIDAR_ON
  Serial1.begin(115200);
  lidar.begin(Serial1);

  pinMode(RPLIDAR_MOTOR, OUTPUT);
#endif


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
  
  #if LIDAR_ON
  if (IS_OK(lidar.waitPoint()))
  {
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    i_loop++;

    //Si nous commençons un nouveau tour
    if (lidar.getCurrentPoint().startBit)
    {
      nb_objet_loop = nombre_objet(tableau_complet_loop);
      //C'est ici que nous envoirons le proto
      //Puis que nous afficherons la lumiere
      //Proto -> nb_objet_loop, minDistance
      //Retour-> lumières


      //et set up le prochain tour
      i_tab = 0; 
      i_loop = 0;
      minDistance = 100000;
      angleAtMinDist = 0;
    }
    else
    {
      //Si nous sommes dans notre plage angulaire
      if (angle >= ANGLE_DEBUT && angle < ANGLE_FIN)
      {
        //Pour trouvé la plus petite distance (fournie)
        if (distance > 0 && distance < minDistance)
        {
          minDistance = distance;
          angleAtMinDist = angle;
        }

        //Mettre un point sur 5 dans le tableau
        if (i_loop % 5 == 0)
        {
          tableau_complet_loop[i_tab] = distance;
          i_tab++;
        }
      }
    }
  }
  else
  {
    //Fourni
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100)))
    {
      //detected...
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 255);
      delay(1000);
    }
  }
  #endif


  if (tele_tab.count == 12){
    calc_average(&t_data, &tele_tab);

    /* For testing purposes */
    Serial.printf("---------- New Output ----------------\n");
    Serial.printf("Lidar timer read : %d\n", lidar_time);

    Serial.printf("%.2f Celsius\n", t_data.temperature);
    Serial.printf("%.2f %% humidity\n",t_data.humidity);
    Serial.printf("%.2f Pa\n", t_data.pressure);
    Serial.printf("Magnetic field: x %d, y %d, z %d\n", t_data.mag_field.x, t_data.mag_field.y,t_data.mag_field.z);

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

/* LIDAR FUNCTIONALITIES */
int nombre_objet(float tableau_complet[POINT_TAB_COMPLET])
{
  //Détecte le nombre d'objet selon un shéma de deux états
  //Voir shema.jpg
  int nb_objet = 0;

  int etat = 0; //0->attend un objet, 1->attend le "vide"

  for (int i = 0; i <= POINT_TAB_COMPLET; i++)
  {
    if (etat == 0 && tableau_complet[i] < DISTANCE_JAUNE)
    {
      nb_objet++;
      etat = 1;
      //Si un tableau d'objets existe, c'est ici qu'on le remplis
    }

    if (etat == 1 && tableau_complet[i] > DISTANCE_JAUNE)
    {
      etat = 0;
    }
  }

  return nb_objet;
}
