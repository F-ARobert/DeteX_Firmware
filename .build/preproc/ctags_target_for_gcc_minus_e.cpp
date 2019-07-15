# 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\data_management.cpp"
# 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\data_management.cpp"
/* This file controls data received from various sensors linked to MXChip

Included features are:

    - Sorting data

    - calculate average

    - Store data



Made by DeteX

*/
# 10 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\data_management.cpp"
# 11 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\data_management.cpp" 2

/* Static variables *************************/
//static telemetry_table_t tele_tab;

/* Initialize telemetry data ***************/
telemetry_table_t telemetry_init(){
    int8_t i;
    telemetry_table_t tele_tab;

    tele_tab.count = 0;
    tele_tab.sum_humidity = 0;
    tele_tab.sum_temperature = 0;
    tele_tab.sum_magnetic = 0;
    tele_tab.sum_pressure = 0;

    memset(&tele_tab.temperature_table,0,12*sizeof(float));
    memset(&tele_tab.pressure_table,0,12*sizeof(float));
    memset(&tele_tab.humidity_table,0,12*sizeof(float));
    memset(&tele_tab.magnetic_table,0,12*sizeof(int32_t));

    return tele_tab;
}

/* Calculate average **********************/
int8_t calc_average(telemetry_data_t *ptr_data, telemetry_table_t *ptr_table){
    ptr_data->temperature = (ptr_table->sum_temperature/ptr_table->count);
    ptr_data->humidity = (ptr_table->sum_humidity/ptr_table->count);
    ptr_data->pressure = (ptr_table->sum_pressure/ptr_table->count);
    ptr_data->mag_field = (ptr_table->sum_magnetic/ptr_table->count);

    if (ptr_data->temperature == 0 || ptr_data->pressure == 0 || ptr_data->mag_field == 0 || ptr_data->humidity == 0){
        return DATA_MNGMT_ERROR;
    } else {

    telemetry_init(); /* reinitialize structure for next batch */
    return NO_ERROR;
    }

}

/* Read sensors ***************************/
void read_sensors(void){
    float pressure = read_pressure();
    float temperature = read_temperature();
    float humidity = read_humidity();
    int32_t mag_field = read_magnetic();


    tele_tab.humidity_table[tele_tab.count] = humidity;
    tele_tab.temperature_table[tele_tab.count] = temperature;
    tele_tab.pressure_table[tele_tab.count] = pressure;
    tele_tab.magnetic_table[tele_tab.count] = mag_field;

    tele_tab.sum_humidity += humidity;
    tele_tab.sum_magnetic += mag_field;
    tele_tab.sum_pressure += pressure;
    tele_tab.sum_temperature += temperature;
}
# 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
# 2 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 3 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 4 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 5 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 6 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2


# 9 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 10 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2


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
# 100 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
}