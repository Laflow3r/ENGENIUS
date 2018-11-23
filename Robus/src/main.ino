
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
  
  BoardInit();
  nunchuk.init();
  Serial.begin(BAUDRATE);
  pinMode(46, OUTPUT);
  digitalWrite(46,HIGH);
  

  
  
}

void loop() {
  
  
  SuivreMur();
  delay(3000);
  
  delay(50);


}



