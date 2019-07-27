# 1 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
# 1 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp"
/* This file controls data received from various sensors linked to MXChip
Included features are:
    - Sorting data
    - calculate average
    - Store data

Made by DeteX
*/

# 11 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/data_management.cpp" 2

/* Static variables *************************/
//static telemetry_table_t tele_tab;

/* Initialize telemetry data ***************/
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
# 1 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
# 2 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2
# 3 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2
# 4 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2
# 5 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2
# 6 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2
# 7 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2


# 10 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2

// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
# 13 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino" 2

/* DEFINES */
# 26 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal
# 46 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
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
int nombre_objet(float tableau_complet[200]);

/* Assign function pointers */
void (*read_sensors_ptr)(void) = &read_all_sensors;
void (*lidar_time_read_ptr)(void) = &lidar_time_read;






void setup() {
# 86 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
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
# 179 "/Users/Felix/Desktop/DeteX_project/Firmware_detex/Device/device.ino"
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
int nombre_objet(float tableau_complet[200])
{
  //Détecte le nombre d'objet selon un shéma de deux états
  //Voir shema.jpg
  int nb_objet = 0;

  int etat = 0; //0->attend un objet, 1->attend le "vide"

  for (int i = 0; i <= 200; i++)
  {
    if (etat == 0 && tableau_complet[i] < 1200 /*4000 pour livraison*/)
    {
      nb_objet++;
      etat = 1;
      //Si un tableau d'objets existe, c'est ici qu'on le remplis
    }

    if (etat == 1 && tableau_complet[i] > 1200 /*4000 pour livraison*/)
    {
      etat = 0;
    }
  }

  return nb_objet;
}
