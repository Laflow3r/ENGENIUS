#ifndef TESTS_H
#define TESTS_H

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>
#include "Base.h"
#include "Obstacle.h"


// ...
void Test_Servo(){
  delay(2000);
  SERVO_SetAngle(0,45);
  delay(2000);
  SERVO_SetAngle(0,90);
  delay(2000);
}
// ...
void test_ligne(){
 Serial.println(analogRead(0));
}

// ...


//3925 = 90 deg
//7850 = 180 deg
void test_rien(){
  
  Serial.print(distance_cm(analogRead(8))); //gauche
  Serial.print("\t");
  Serial.print(distance_cm(analogRead(9))); //avant
  Serial.print("\t");
  Serial.println(distance_cm(analogRead(11))); //droite
  delay(1000);
  
}

// ...
void Test_IR(){
  Serial.print(distance_cm(analogRead(8)));
  
  Serial.print("\t");
  Serial.print(distance_cm(analogRead(9)));
  Serial.print("\t");
  Serial.println(distance_cm(analogRead(11)));
  
}

void test_90(){
  
  int nbr = 2010;
  
  for(int i = 0; i < 8; i++)
  {
    //tour(0,nbr);
    TournerSurLui(nbr,0);
    delay(500);
  }
  
  
}

#endif