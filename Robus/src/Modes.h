#ifndef MODES_H
#define MODES_H

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>

#include "Base.h"

#define BLANC <100
#define NOIR >600
#define PASNOIR <500
#define BAUDRATE 9600

ArduinoNunchuk nunchuk = ArduinoNunchuk();
int direction;
int stawp =1;
int vibrationPin = 8;

int mode = 0;


void SuivreLigne() {
  float vitesse = 0.2;
  int a0 = analogRead(0); //Senseur Droite
  int a1 = analogRead(1);
  int a2 = analogRead(2);
  int a3 = analogRead(3); //Senseur milieu droite
  int a4 = analogRead(4); //Senseur milieu gauche
  int a5 = analogRead(5);
  int a6 = analogRead(6);
  int a7 = analogRead(7); //Senseur gauche

  //vitesse lorsque le robot avance sur une ligne
  
  //indique le cote que le robot a quitter la ligne
  int sortie;
  MOTOR_SetSpeed(0,vitesse);
  MOTOR_SetSpeed(1,vitesse);

      Serial.print(" a3: ");  Serial.print(a3); Serial.println();
      Serial.print(" a4: ");  Serial.print(a4); Serial.println();
      Serial.print(" a5: ");  Serial.print(a5); Serial.println();
      Serial.print(" a6: ");  Serial.print(a6); Serial.println();
     
      Serial.println();
  /*
  if(a1 NOIR && a2 NOIR && a3 NOIR){
  MOTOR_SetSpeed(0,0.2);
  MOTOR_SetSpeed(1,0);
  delay(1000);
  }

  if(a4 NOIR && a5 NOIR && a6 NOIR){
  MOTOR_SetSpeed(1,0.2);
  MOTOR_SetSpeed(0,0);
  delay(1000);
  } 
  */

  //angle droit gauche  
  if((a3 NOIR && a4 NOIR && a5 NOIR && a6 NOIR && a7 NOIR && a0 PASNOIR)){
    MOTOR_SetSpeed(0,0.1);
    MOTOR_SetSpeed(1,0.1);
    delay(150);
    MOTOR_SetSpeed(0,-0.1);
    delay(1500);
  }

  //angle droit droite
    if((a0 NOIR && a1 NOIR && a2 NOIR && a3 NOIR && a4 NOIR && a7 PASNOIR)){
        MOTOR_SetSpeed(0,0.1);
          MOTOR_SetSpeed(1,0.1);
        delay(150);
          MOTOR_SetSpeed(1,-0.1);
            delay(1500);
    }

    //Le robot est sur la ligne (senseurs milieu voient du noir)
    if((a3 NOIR && a4 NOIR)){

        MOTOR_SetSpeed(0,vitesse);
        MOTOR_SetSpeed(1,vitesse);

    if(a6 NOIR){sortie = 1;} if(a7 NOIR){sortie = 1;}
    if(a1 NOIR){sortie = 2;} if(a0 NOIR){sortie = 2;}
    }

    //milieu gauche ne detecte pas la ligne mais le droit oui (sort a gauche)
    if(a3 NOIR && a4 PASNOIR){



        MOTOR_SetSpeed(0,0.2);
        MOTOR_SetSpeed(1,0);
    if(a6 NOIR){sortie = 1;} if(a7 NOIR){sortie = 1;}
    if(a1 NOIR){sortie = 2;} if(a0 NOIR){sortie = 2;}
    }

    //milieu droite ne detecte pas la ligne mais le gauche oui
    if(a3 PASNOIR && a4 NOIR){
        MOTOR_SetSpeed(1,0.2);
        MOTOR_SetSpeed(0,0);
    if(a6 NOIR){sortie = 1;} if(a7 NOIR){sortie = 1;}
    if(a1 NOIR){sortie = 2;} if(a0 NOIR){sortie = 2;}

    }
    //les senseurs milieu ne captent rien
    if(a3 PASNOIR && a4 PASNOIR){

    if(a5 NOIR){sortie = 1;}
    if(a6 NOIR){sortie = 1;}
    //if(a7 NOIR){sortie = 1;}

    if(a2 NOIR){sortie = 2;}
    if(a1 NOIR){sortie = 2;}
    //if(a0 NOIR){sortie = 2;}
    //MOTOR_SetSpeed(0,-0.1); MOTOR_SetSpeed(1,0);
  Serial.print(" Sortie: ");  Serial.print(sortie); Serial.println();
      //le robot est sortie par la gauche
      if(sortie == 2){ MOTOR_SetSpeed(0,0.1); MOTOR_SetSpeed(1,-0.1); }

      //le robot est sortie par la droite
      if(sortie == 1){ MOTOR_SetSpeed(1,0.1); MOTOR_SetSpeed(0,-0.1); }
  }
  delay(50);

}


//Fonction pour vibrer, delayOn = temps que le moteur vibre, le delayOff = temps entre les vibrations
void vibration(int fois,int delayOn, int delayOff){
  int i;
  for (i=0; i<fois; i++){
      digitalWrite(vibrationPin, HIGH); //HIGH=motor on
      delay(delayOn);
      digitalWrite(vibrationPin, LOW);//LOW=motor off
      delay(delayOff);
    
  } 
}

void bouger(float gauche, float droite){
  MOTOR_SetSpeed(0,gauche);
  MOTOR_SetSpeed(1,droite);
}



void arret(float vitesse){
  if(direction == 1){
  while(vitesse > 0){vitesse = vitesse - 0.1; bouger(vitesse,vitesse); delay(100);} }
 if(direction==-1){while(vitesse < 0){vitesse = vitesse + 0.1; bouger(vitesse,vitesse); delay(100);} }
  bouger(0,0);
  stawp = 1;
}

void nunchuck(){

  float vitesse = 0.5;

  if(nunchuk.zButton == 1 ) {
  Serial.print("\tstawp: "); Serial.print(stawp); Serial.print("\t ");
  Serial.print("direction: "); Serial.print(direction); Serial.print("\t ");

  //arreter
  if(nunchuk.analogX < 190 && nunchuk.analogX > 60 && nunchuk.analogY < 190 && nunchuk.analogY > 60) {Serial.println("STAWP"); if(stawp == 0){arret(vitesse*direction); stawp = 1;} if(stawp==2){bouger(0,0);} stawp = 1;}
  //aller tout droit
  if(nunchuk.analogY >= 190 ) {checkSpeed(0,vitesse); direction = 1; stawp=0; Serial.println("J'avance CR*SS"); }
  //Reculer
  if(nunchuk.analogY <= 60 ) {Serial.println("Beeeeep Beeeeep Beeeeep"); direction = -1; stawp =0; bouger(-vitesse,-vitesse);}
  //Aller a droite
  if(nunchuk.analogX >= 190 ) {Serial.println("Tribord toute #droite"); stawp =2; bouger(direction*0.3,0);}
  //Aller a gauche
  if(nunchuk.analogX <= 60 ) {Serial.println("Babord toute #Gauche"); stawp =2; bouger(0,direction*0.3);}
  }
  else{bouger(0,0);}

}

void PrintNunchuk(){
   nunchuk.update();
  //print nunchuk output
  Serial.print(nunchuk.analogX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.analogY, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelY, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelZ, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.zButton, DEC);
  Serial.print(' ');
  Serial.println(nunchuk.cButton, DEC);
}

void start(int mode){


  //mode guide
  if(mode==1){
  Serial.println("mode 1 selectionne"); vibration(3,500,500);
  //SuivreLigne();
    }
  else
  //mode libre
  if(mode==2){
   Serial.println("mode 2 selectionne"); vibration(3,1500,200);  
  //nunchuck();
  }


}

void walkus(){
    int temp = 0;
  //Choisir le mode d'operation avec le bouton C. Change lorsque le bouton c est relache apres avoir ete appuye seul.
   nunchuk.update();
  
  if(nunchuk.cButton == 1 && nunchuk.zButton == 0){
  nunchuk.update();                                                     
  while(nunchuk.cButton==1){delay(10); nunchuk.update();                Serial.println("C enfonce");}
   nunchuk.update();
                                                                        Serial.println("C relache");
   if(mode==0) {mode=1; vibration(1,500,0);}
  else
   if(mode==1){mode=2; vibration(2,500,150);}
  else
   if(mode==2){mode=1; vibration(1,500,0);}

 Serial.print("\tMode "); Serial.print(mode); Serial.println("");
  }
                                                        
   nunchuk.update();

  //Confirmer la selection du mode en gardant enfonce C et Z pendant 1 seconde
  if(mode!=0){
    while(nunchuk.cButton == 0 && nunchuk.zButton == 1){
       nunchuk.update();

  temp = temp + 1;
  delay(100);
  if(temp==10){vibration(3,100,100); start(mode);}
  }
  }
}

#endif