
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

int IsForward(){
  nunchuk.update();
  if(nunchuk.analogY >= 190){return 1;} else {return 0;}
}

int checkZ(){
    nunchuk.update();
  if(nunchuk.zButton == 1){return 1;} else {return 0;};
}

void UpdateNun(){
  nunchuk.update();
}

void setup(){


  //check_test();
  
  BoardInit();
  nunchuk.init();
  Serial.begin(BAUDRATE);
  //changerDistanceMur(1,5);
  pinMode(46, OUTPUT);
  digitalWrite(46,HIGH);
  //test_90();
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



