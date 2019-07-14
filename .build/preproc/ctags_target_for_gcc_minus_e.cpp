# 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
# 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
/* This file allows use of onboard and external sensors with MXChip.

Included features are:

    - Initialisation of sensors

    - Communication with sensors

    - Read commands



Made by DeteX

*/
# 10 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
# 11 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp" 2

/* VARIABLES ***********************************/
DevI2C *i2c;
HTS221Sensor *HT_sensor;
static RGB_LED rgbLed;
static float humidity;
static float temperature;



void init_onboard_temp_sensor(void)
{
    i2c = new DevI2C(D14, D15);
    HT_sensor = new HTS221Sensor(*i2c);
    HT_sensor->init(
# 25 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp" 3 4
                   __null
# 25 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\detex_sensor.cpp"
                       );

    humidity = -1;
    temperature = -1000;
}

float read_temperature(void){
    HT_sensor->reset();

    float temperature = 0;
    HT_sensor->getTemperature(&temperature);

    return temperature;
}

float read_humidity(void){
    HT_sensor->reset();

    float humidity = 0;
    HT_sensor->getHumidity(&humidity);

    return humidity;
}
# 1 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
# 2 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 3 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 4 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 5 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2
# 6 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2


# 9 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino" 2


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
# 75 "c:\\Users\\carta\\Documents\\IoTWorkbenchProjects\\projects\\DeteX_Firmware\\Device\\device.ino"
}
