/* This contains all header fiels necessary for sending messages to use protobuf

Made by DeteX
*/

#ifndef PROTOBUF_COMMUNICATION_H
#define PROTOBUF_COMMUNICATION_H

/* Header files necessary to use protobuf */
#include "EventBase.pb.h"
#include "LightControl.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

/* Headers necessary to gather data */
#include "lidar.h"
#include "data_management.h"

/* Header necessary to send data to Azure cloud */
#include "DevKitMQTTClient.h"

/* DEFINES */
#define DEVICE_ID_STRING "ele400-equipe4"



/* FUNCTIONS */
/*
@Brief: Send telemetry data to Azur IOT hub
@Param: data structure to send
@Return: none
*/
void send_telemetry(telemetry_data_t data);

/*
@Brief: Send ldiar data to Azur IOT hub
@Param: data structure to send
@Return: none
*/
void send_lidar(lidar_data_t data);

#endif