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

#define ANGLE_DEBUT 180 //Debut de la couverture (en degrés)
#define ANGLE_FIN 360 //Fin de la couverture

#define DISTANCE_ROUGE 800 //en mm. 800 pour test, 1500 pour livraison
#define DISTANCE_JAUNE 1200 //4000 pour livraison

#define POINT_TAB_COMPLET 200

#define RPLIDAR_MOTOR 9 // The PWM pin for control the speed of RPLIDAR's motor. 
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal
                  
/*
@Brief: Operates LIDAR
@Param: RPLidar type object
@Return: none
 */
void run_lidar(RPLidar lidar);
#endif