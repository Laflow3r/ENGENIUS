
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

int Zpressed(){
  nunchuk.update();
  return nunchuk.zButton;
}
int Cpressed(){
  nunchuk.update();
  return nunchuk.cButton;
}

int IsForward(){
  nunchuk.update();
  if(nunchuk.analogY >= 190){return 1;} else {return 0;}
}

int checkZ(){
    nunchuk.update();
    while(nunchuk.zButton =! 1){delay(50); nunchuk.update();}
return 0;
}

void UpdateNun(){
  nunchuk.update();
}

void setup(){
  
  //initialisation
  BoardInit();
  nunchuk.init();

  //Essentiel pour Serial.print les valeurs du nunchuk
  Serial.begin(BAUDRATE);
  pinMode(46, OUTPUT);
  digitalWrite(46,HIGH);
  

  
  
}

void loop() {
  
  //****** Fonction Principale ******

          walkus();        delay(50);
  
  // ********************************
 


//Test_IR();
//test_ligne();

}



