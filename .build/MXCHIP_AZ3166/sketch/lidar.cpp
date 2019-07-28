
/* This file controls Lidar operations
Included features are:
    - Minumum and maximum values
    - Lidar operating function
    

Made by DeteX
*/

#include "lidar.h"

/* STATIC FUNCTIONALITIES */
uint8_t nombre_objet(float *tableau_complet, int size);

//Set-up le premier tour
float minDistance = 100000;
float angleAtMinDist = 0;

float tableau_complet_loop[POINT_TAB_COMPLET];
int i_loop = 0;         //Devrait finir a 2000
int nb_objet_loop = 0;
int i_tab = 0;          //Doit s'arreter a POINT_TAB_COMPLET


/* Run lidar *****************/
void run_lidar(RPLidar lidar){
    if (IS_OK(lidar.waitPoint()))
    {
      float distance = lidar.getCurrentPoint().distance;
      float angle = lidar.getCurrentPoint().angle;
      i_loop++;

      //Si nous commençons un nouveau tour
      if (lidar.getCurrentPoint().startBit)
      {
        nb_objet_loop = nombre_objet(tableau_complet_loop, (sizeof*tableau_complet_loop)*POINT_TAB_COMPLET);
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
            
            if (POINT_TAB_COMPLET == i_tab) {
                i_tab = 0;
            }
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
}

/* STATIC FUNCTIONS ****************************/
/* LIDAR FUNCTIONALITIES */
uint8_t nombre_objet(float *tableau_complet, int size)
{
  //Détecte le nombre d'objet selon un shéma de deux états
  //Voir shema.jpg
  int nb_objet = 0;

  int etat = 0; //0->attend un objet, 1->attend le "vide"

  for (int i = 0; i <= size; i++)
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
