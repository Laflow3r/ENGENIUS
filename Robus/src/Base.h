#ifndef BASE_H
#define BASE_H

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>

float nbr0 = 0;
float nbr1 = 0;
float diff_live;

//NE PAS UTILISER ; c'est utilisé par la fonction avance() pour aller tout droit (aka PID)
void checkSpeed(float diff, float vit_actuelle){
    


  nbr0 =  ENCODER_Read(0);
  nbr1 =  ENCODER_Read(1);
  diff_live = nbr0 - nbr1;
  float changement;
    
  if (diff_live < 5) {
    changement = 0.05;
  }else if (diff_live < 10) 
  {
    changement = 0.1;
  }else if (diff_live < 15) 
  {
    changement = 0.15;
  }else if (diff_live < 20) 
  {
    changement = 0.2;
  }
  else
  {
    changement = 0.3;
  }
  
  
    
  if ((nbr0 > nbr1)) {
    MOTOR_SetSpeed(0, vit_actuelle * (1-changement)); 
    MOTOR_SetSpeed(1, vit_actuelle * (1+changement)); 
  } else if ((nbr0 < nbr1)) {
    MOTOR_SetSpeed(0, vit_actuelle * (1+changement)); 
    MOTOR_SetSpeed(1, vit_actuelle * (1-changement)); 
  } else {
    MOTOR_SetSpeed(0,vit_actuelle);
    MOTOR_SetSpeed(1,vit_actuelle);
  }
  
}
//Pour tourner avec les 2 moteurs -1 = gauche  1 = droite
void TournerSurLui(float degre, float side){

  float speed = 0.4;
  int roueDroite=0, roueGauche=0;
  float bias = 1.00638;

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  if (side == 0){
     do{

    // MOTOR_SetSpeed(1,speed);
    // if (roueDroite > degre - 800){
    //   if ( speed > 0.1) speed -= 0.05 ;
    // }else if (speed < .4) speed += 0.005;
    MOTOR_SetSpeed(1,speed);
    MOTOR_SetSpeed(0,-1*speed);

    roueGauche = ENCODER_Read(0) * bias;
    roueDroite = ENCODER_Read(1) ; 
    // if (roueDroite > degre - 800){
    //   if ( speed > 0.1) speed -= 0.05 ;
    //   else speed = 0.1;
    // }else if (speed < 0.4) speed += 0.005;

    if (roueDroite > degre) MOTOR_SetSpeed(0,0);
    if ((roueGauche) > degre) MOTOR_SetSpeed(1,0);
     //Serial.println(roueDroite);
     delay(10);
   }
   while(roueDroite < degre || -1 * roueGauche < degre);
  }
  if (side == 1){
    do{

    MOTOR_SetSpeed(0,speed);
    if (roueGauche > degre - 800){
      if ( speed > 0.1) speed -= 0.05 ;
    }else if (speed < .4) speed += 0.005;
    MOTOR_SetSpeed(0,speed);
    MOTOR_SetSpeed(1,-1*speed);

    roueGauche = ENCODER_Read(0);
    roueDroite = ENCODER_Read(1) * bias;
    if (roueGauche > degre - 800){
      if ( speed > 0.1) speed -= 0.05 ;
      else speed = 0.1;
    }else if (speed < 0.4) speed += 0.005;

    if (roueGauche > degre) MOTOR_SetSpeed(0,0);
    if (-1*roueDroite > degre) MOTOR_SetSpeed(1,0);
     //Serial.println(roueDroite);
     delay(10);
   }
   while(side*roueGauche < degre || side*-1 * roueDroite < degre);
  }
  
  
   MOTOR_SetSpeed(0,0);
   MOTOR_SetSpeed(1,0);
   ENCODER_Reset(0);
   ENCODER_Reset(1);
   delay(100);
}
//Pour tourner avec un moteur
void tour(int side, int nbr){
  float vitesse = 0.00;
  int dist_v = 1430;
  float pour_v = 0.035;
  if (side == 0) {
    while(ENCODER_Read(1) < nbr) {

      if (ENCODER_Read(1) <dist_v) {
        vitesse = 0.10 + (ENCODER_Read(1) / 100) *pour_v ; 
      }
      else if (ENCODER_Read(1) > nbr -dist_v) {
        vitesse = 0.10 + ((nbr - ENCODER_Read(1)) / 100) *pour_v ;
      }else{
        vitesse  = 0.60;
      }
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,vitesse);
    }
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
  }
  else if(side == 1){
    while(ENCODER_Read(0) < nbr) {

      if (ENCODER_Read(0) <dist_v) {
        vitesse = 0.10 + (ENCODER_Read(0) / 100) *pour_v ; 
      }else if (ENCODER_Read(0) > nbr -dist_v) {
        vitesse = 0.10 + ((nbr - ENCODER_Read(0)) / 100) *pour_v ;
      }else{
        vitesse  = 0.4;
      }

      MOTOR_SetSpeed(1,0);
      MOTOR_SetSpeed(0,vitesse); 
    }
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    }
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}
//Pour avancer
void avance(int distance){
  float vitesse = 0.00;
  int dist_v = 1430;
  float pour_v = 0.035;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while(ENCODER_Read(0) < distance){
    
    if (ENCODER_Read(0) < dist_v) {
      vitesse = 0.10 + (ENCODER_Read(0) / 100) *pour_v ; 
      
    }
    else if (ENCODER_Read(0) > distance - dist_v) {
      vitesse = 0.10 + ((distance - ENCODER_Read(0)) / 100) *pour_v ;
    }
    else
    {
      vitesse  = 0.6;
    }
    checkSpeed(0,vitesse);

  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  delay(100);
}
//Pour arrêter sec
void Stop(){
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

float CmToTic(float cm){
  return ((cm/(7.0*PI))*3600.0);
}

#endif