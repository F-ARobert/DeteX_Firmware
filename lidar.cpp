
/* This file controls Lidar operations
Included features are:
    - Minumum and maximum values
    - Lidar operating function
    

Made by DeteX
*/

#include "lidar.h"

/* STATIC FUNCTIONALITIES */
uint8_t nombre_objet(float *tableau_complet, int size);
static float dist_min;
static int red_counter = 0;
static int yellow_counter = 0;
static int green_counter = 0;


/* Lidar_data initialize ***************/
lidar_data_t lidar_data_init(void){
  lidar_data_t new_data;
    
  memset(new_data.angle, -1, POINT_TAB_COMPLET);
  memset(new_data.distance, 0, POINT_TAB_COMPLET);
  new_data.distance_min = DEFAULT_MIN_DIST;
  new_data.nombre_obj=0;
  new_data.state = NONE;
  new_data.startbit = false;

  return new_data;
}


/* Run lidar *****************/
void run_lidar(RPLidar lidar, lidar_data_t* data, int* i_loop, int* i_tab){
    if (IS_OK(lidar.waitPoint()))
    {
      float distance = lidar.getCurrentPoint().distance;
      float angle = lidar.getCurrentPoint().angle;
      uint8_t quality = lidar.getCurrentPoint().quality;
      *i_loop += 1;

      //Si nous commençons un nouveau tour
      if (lidar.getCurrentPoint().startBit)
      {
        data->distance_min = dist_min;
        data->startbit = true;
        data->nombre_obj = nombre_objet(data->distance, POINT_TAB_COMPLET);
        
        if (data->distance_min <= DISTANCE_ROUGE) {
          ++red_counter;
          yellow_counter = 0;
          green_counter = 0;

          if (3 == red_counter){
            data->state = RED;
            red_counter = 0;
          }

        } else if (data->distance_min < DISTANCE_JAUNE){
          ++yellow_counter;
          red_counter = 0;
          green_counter = 0;

          data->state = YELLOW;
          yellow_counter = 0;

        } else {
          ++green_counter;
          red_counter = 0;
          yellow_counter = 0;

          if (20 == green_counter){
            data->state = GREEN;
            green_counter = 0;
          }
        }
        
        //et set up le prochain tour
        *i_tab = 0; 
        *i_loop = 0;
        dist_min = DEFAULT_MIN_DIST;
      }
      else // If startBit == 0 (not new loop)
      {
        data->startbit = false;
        //Si nous sommes dans notre plage angulaire
        if ((angle >= ANGLE_DEBUT) && (angle < ANGLE_FIN) && (MINIMUM_QUALITY < quality)) {
          //Pour trouvé la plus petite distance
          if ((distance > 0) && (distance < data->distance_min))
          {
            dist_min = distance;
          }
          //Mettre un point sur 5 dans le tableau
          data->distance[*i_tab] = distance;
          data->angle[*i_tab] = angle;
          ++*i_tab;

          if(POINT_TAB_COMPLET == *i_tab){
            *i_tab = 0;
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
    if ((etat == 0) && (tableau_complet[i] < LIMITE_ZONE))
    {
      nb_objet++;
      etat = 1;
    } 
    else if((etat == 1) && (tableau_complet[i] > LIMITE_ZONE))
    {
      etat = 0;
    }
  }
  return nb_objet;
}
