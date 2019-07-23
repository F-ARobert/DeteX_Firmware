/*
 * USAGE:
 * ---------------------------------
 * 1. Download this sketch code to your Arduino board
 * 2. Connect the RPLIDAR's serial port (RX/TX/GND) to your Arduino board (Pin 0 and Pin1)
 * 3. Connect the RPLIDAR's motor ctrl pin to the Arduino board pin 3
 * 4. Connect an RGB LED to your Arduino board, with the Red led to pin 9, Blue led to pin 10, Green led to pin 11
 * 5. Connect the required power supplies.
 * 6. RPLIDAR will start rotating when the skecth code has successfully detected it.
 * 7. Remove objects within the 0.5 meters' radius circule range of the RPLIDAR
 * 8. Place some object inside the 0.5 meters' range, check what will happen to the RGB led :)
 */

 // This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>

// You need to create an driver instance
RPLidar lidar;

// Change the pin mapping based on your needs.
/////////////////////////////////////////////////////////////////////////////

//On peut se séparer du led_enable et avoir uniquement les led/lumières qui irons
//au ground

#define LED_ENABLE 12 // The GPIO pin for the RGB led's common lead.
      // Assumes a common positive type LED is used

#define LED_R 9       // The PWM pin for drive the Red LED
#define LED_J 10      // The PWM pin for drive the jaune LED
#define LED_V 11      // The PWM pin for drive the vert LED

#define ANGLE_DEBUT 0 //Debut de la couverture (en degrés)
#define ANGLE_FIN 180 //Fin de la couverture

#define DISTANCE_ROUGE 800 //en mm. 800 pour test, 1500 pour livraison
#define DISTANCE_JAUNE 1200 //4000 pour livraison

#define POINT_TAB_COMPLET 200

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor. \
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal
//////////////////////////////////////////////////////////////////////////////

void displayColor_distance(float distance)
{//Test pour voir ou se trouve les régions

  //Mise a 0
  int _r = 0;
  int _j = 0;
  int _v = 0;
  int case_distance = 0;

  if (distance < DISTANCE_ROUGE)
    case_distance = 1;
  else if (distance < DISTANCE_JAUNE)
    case_distance = 2;
  else
    case_distance = 3;

  //Switch selon les distances
  switch (case_distance)
  {
  case 1:
    _r = 255;
    break;

  case 2:
    _j = 255;
    break;

  case 3:
    _v = 255;
    break;
  }

  //Si on enleve LED_ENABLE il faut enlever les "255-"
  analogWrite(LED_R, 255 - _r);
  analogWrite(LED_J, 255 - _j);
  analogWrite(LED_V, 255 - _v);
}

void displayColor_angle(float angle)
{ //Test pour voir ou se trouve l'intervale des angles

  //Mise a 0
  int _r = 0;
  int _j = 0;
  int _v = 0;
  int case_angle = 0;

  //Semi-Redondant
  if (angle >= ANGLE_DEBUT && angle < ANGLE_FIN)
    case_angle = 1;
  else
    case_angle = 2;

  switch (case_angle)
  {
  case 1:
    _r = 255;
    _j = 255;
    _v = 255;
    break;

  case 2:
    _r = 0;
    _j = 0;
    _v = 0;
    break;

  case 0:
    _r = 255;
    _j = 0;
    _v = 0;
    break;
  }

  //Si on enleve LED_ENABLE il faut enlever les "255-"
  analogWrite(LED_R, 255 - _r);
  analogWrite(LED_J, 255 - _j);
  analogWrite(LED_V, 255 - _v);
}

void displayColor_nb(int nb_objet)
{
  //Affiche grace au 3 del un nombre de 1 a 7

  int _r = 255;
  int _j = 0;
  int _v = 255;

  switch (nb_objet)
  {
  case 0:
    _r = 0;
    _j = 0;
    _v = 0;
    break;

  case 1:
    _r = 255;
    _j = 0;
    _v = 0;
    break;

  case 2:
    _r = 0;
    _j = 255;
    _v = 0;
    break;

  case 3:
    _r = 255;
    _j = 255;
    _v = 0;
    break;

  case 4:
    _r = 0;
    _j = 0;
    _v = 255;
    break;

  case 5:
    _r = 255;
    _j = 0;
    _v = 255;
    break;

  case 6:
    _r = 0;
    _j = 255;
    _v = 255;
    break;

  case 7:
    _r = 255;
    _j = 255;
    _v = 255;
    break;
  }

  //Si on enleve LED_ENABLE il faut enlever les "255 - "
  analogWrite(LED_R, 255 - _r);
  analogWrite(LED_J, 255 - _j);
  analogWrite(LED_V, 255 - _v);
}

int nombre_objet(float tableau_complet[POINT_TAB_COMPLET])
{
  //Détecte le nombre d'objet selon un shéma de deux états
  //Voir shema.jpg
  int nb_objet = 0;

  int etat = 0; //0->attend un objet, 1->attend le "vide"

  for (int i = 0; i <= 1000; i++)
  {
    if (etat == 0 && tableau_complet[i] < DISTANCE_JAUNE)
    {
      nb_objet++;
      etat = 1;
    }

    if (etat == 1 && tableau_complet[i] > DISTANCE_JAUNE)
    {
      etat = 0;
    }
  }

  return nb_objet;
}

void setup()
{
  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(Serial);

  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);

  pinMode(LED_ENABLE, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_J, OUTPUT);
  pinMode(LED_V, OUTPUT);

  digitalWrite(LED_ENABLE, HIGH);

  analogWrite(LED_R, 255);
  analogWrite(LED_J, 255);
  analogWrite(LED_V, 255);
}

//Set-up le premier tour
float minDistance = 100000;
float angleAtMinDist = 0;

float tableau_complet_loop[POINT_TAB_COMPLET];
int i_loop = 0;         //Devrait finir a 2000
int nb_objet_loop = 0;
int i_tab = 0;          //Doit s'arreter a POINT_TAB_COMPLET

void loop()
{

  if (IS_OK(lidar.waitPoint()))
  {

    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    i_loop++;

    //Si nous commençons un nouveau tour
    if (lidar.getCurrentPoint().startBit)
    {
      nb_objet_loop = nombre_objet(tableau_complet_loop);
      //C'est ici que nous envoirons le proto
      //Puis que nous afficherons la lumiere
      //Proto -> nb_objet_loop, minDistance
      //Retour-> lumières

      //FONCTIONS DE TEST: Un ou l'autre pas plusieurs
      //displayColor_distance(minDistance);
      //displayColor_angle(angleAtMinDist); //Un ou l'autre pas les 2
      //displayColor_nb(nombre_objet(tableau_complet_loop));


      //et set up le prochain tour
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
