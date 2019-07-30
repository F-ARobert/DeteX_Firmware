/* This contains all header fiels necessary for sending messages to use protobuf

Made by DeteX
*/

#include "protobuf_communication.h"


/* VARIABLES */
char* id_string;
char device_id_string[] = DEVICE_ID_STRING;
char* payload_string;
char* correlation_id_string;

/* send telemetry ********************************/
void send_telemetry(telemetry_data_t data){
    uint8_t buff[1280];
    size_t msg_length;
    bool status;

    EventBase msg_telemetry = EventBase_init_zero;
      
    pb_callback_t id;
    pb_callback_t device_id;
    pb_callback_t payload;
    pb_callback_t correlation_id;

    strcpy(id_string,"5d2b572f3dd05300015cad67");
    strcpy(correlation_id_string, "anythingoes");
    strcpy(payload_string,"5");

    id.arg = id_string;
    device_id.arg = device_id_string;
    payload.arg = payload_string;
    correlation_id.arg = correlation_id_string;

    msg_telemetry.id = id;
    msg_telemetry.deviceId = device_id;
    msg_telemetry.deviceTime = millis();
    msg_telemetry.version = 1;
    msg_telemetry.correlationId = correlation_id;
    msg_telemetry.commandId;
    msg_telemetry.payload = payload;

    pb_ostream_t stream = pb_ostream_from_buffer(buff,sizeof(buff));

    status = pb_encode(&stream, EventBase_fields, &msg_telemetry);
    msg_length = stream.bytes_written;

    // replace the following line with your data sent to Azure IoTHub
    snprintf((char*)buff, msg_length, "{\"topic\":\"iot\"}");
        
    if (DevKitMQTTClient_SendEvent((char*)buff))
    {
      Screen.print(1, "Sending...");
    }
    else
    {
      Screen.print(1, "Failure...");
    }
}


/* send lidar ***********************/
void send_lidar(lidar_data_t data){

}