/* This file controls Lidar operations
Included features are:
    - Minumum and maximum values
    - Lidar operating function
    

Made by DeteX
*/

#ifndef LIDAR_H
#define LIDAR_H

// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include "RPLidar.h"


/* DEFINES */

#define ANGLE_DEBUT 200 //Debut de la couverture (en degrés)
#define ANGLE_FIN 340 //Fin de la couverture

#define DISTANCE_ROUGE 500 //en mm. 800 pour test, 1500 pour livraison
#define DISTANCE_JAUNE 1500 //4000 pour livraison

#define POINT_TAB_COMPLET 500
#define TOP_QUALITY 15

#define RPLIDAR_MOTOR 0 // The PWM pin for control the speed of RPLIDAR's motor. 
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal

/* Variables *************************/
typedef struct lidar_data {
    float distance[POINT_TAB_COMPLET];
    float angle[POINT_TAB_COMPLET];
    float distance_min;
    int nombre_obj;
    bool startbit;
}lidar_data_t;

/*
@Brief: Initialize a Lidar data struct
@Param: none
@Return: new lidar_data struct
*/
lidar_data_t lidar_data_init(void);

/*
@Brief: Operates LIDAR
@Param: RPLidar type object
@Return: none
 */
void run_lidar(RPLidar lidar, lidar_data_t* data, int* i_loop, int* i_tab);
#endif