
/*
Projet: WalkUS
Equipe: Engenius P24
Auteurs: Engenius
Description: Robot guide d'aveugle
Date: 08/11/18
*/


#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>

#include "Obstacle.h"
#include "Base.h"
#include "Tests.h"
#include "Modes.h"

void setup(){


  //check_test();
  
  BoardInit();
  nunchuk.init();
  Serial.begin(BAUDRATE);
  //changerDistanceMur(1,5);
}

void loop() {
  
  //allo();
  //Serial.println(distance_cm(analogRead(8)));
  //perpendiculaire();
  //vibration(3,1000,500);
  walkus();
  //delay(2000);
  //SuivreLigne();
  delay(50);
}



