
/* This file controls Lidar operations
Included features are:
    - Minumum and maximum values
    - Lidar operating function
    

Made by DeteX
*/

#include "lidar.h"

/* STATIC FUNCTIONALITIES */
uint8_t nombre_objet(float *tableau_complet, int size);

/* Lidar_data initialize ***************/
lidar_data_t lidar_data_init(void){
  lidar_data_t new_data;
    
  memset(new_data.angle, -1, POINT_TAB_COMPLET);
  memset(new_data.distance, 0, POINT_TAB_COMPLET);
  new_data.distance_min = 10000;
  new_data.nombre_obj=0;

  return new_data;
}


/* Run lidar *****************/
void run_lidar(RPLidar lidar, lidar_data_t data, int* i_loop, int* i_tab){
    if (IS_OK(lidar.waitPoint()))
    {
      char *test;
      float distance = lidar.getCurrentPoint().distance;
      float angle = lidar.getCurrentPoint().angle;
      *i_loop += 1;
      //Screen.print(1,"lidar OK");
      sprintf(test,"Obtained %.2f %.2f", distance, angle);
      //Screen.print(1,test,true);
      //delay(3000);
      //Si nous commençons un nouveau tour
      if (lidar.getCurrentPoint().startBit)
      {
        data.startbit = true;
        data.nombre_obj = nombre_objet(data.distance, POINT_TAB_COMPLET);
        
        //et set up le prochain tour
        *i_tab = 0; 
        *i_loop = 0;
      }
      else // If startBit == 0 (not new loop)
      {
        data.startbit = false;
        //Si nous sommes dans notre plage angulaire
        if ((angle >= ANGLE_DEBUT) && (angle < ANGLE_FIN)) {
            
          //Mettre un point sur 5 dans le tableau
          if (0 == (*i_loop % 5)) {
            data.distance[*i_tab] = distance;
            data.angle[*i_tab] = angle;

            //Pour trouvé la plus petite distance
            if ((distance > 0) && (distance < data.distance_min))
            {
              data.distance_min = distance;
            }

            *i_tab++;
              
            if (*i_tab == POINT_TAB_COMPLET) {
              *i_tab = 0;
            }
          }
        }
      }
    }
    else
    {
      //Screen.print(1,"No connect",true);
      //Fourni
      analogWrite(PB_3, 0); //stop the rplidar motor

      // try to detect RPLIDAR...
      rplidar_response_device_info_t info;
      if (IS_OK(lidar.getDeviceInfo(info, 100)))
      {
        //Screen.print(1,"trying",true);
        //detected...
        lidar.startScan();
        analogWrite(PB_3, 255);
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
