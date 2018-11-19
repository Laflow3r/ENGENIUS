
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

#define BLANC <100
#define NOIR >600
#define PASNOIR <500
#define CHECKPOINTS 180
#define BAUDRATE 9600

ArduinoNunchuk nunchuk = ArduinoNunchuk();

float referenceVolt = 5.00;
float diff_global = 0;
int debut_L = 0;
int debut_H = 0;
float nbr0 = 0;
float nbr1 = 0;
float vitesse = 0.4;
float numCheck;
float diff_live;
int dist_L = 0;
int dist_H = 0;
int vibrationPin = 8;

int loop_cnt=0;
int nunx,nuny,zbut,cbut;
int ledPin = 13;

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

// ...
void Test_IR(){
  Serial.print(distance_cm(analogRead(8)));
  
  Serial.print("\t");
  Serial.print(distance_cm(analogRead(9)));
  Serial.print("\t");
  Serial.println(distance_cm(analogRead(11)));
  
}
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
//Pour suivre la ligne (Utiliser SuivreLigne)
void Ligne(){
  int a0 = analogRead(0);
  int a1 = analogRead(1);
  int a2 = analogRead(2);
  int a3 = analogRead(3);
  int a4 = analogRead(4);
  int a5 = analogRead(5);
  int a6 = analogRead(6);
  int a7 = analogRead(7);
  float pourc = 0.05;
  
  if (a0 NOIR) { //Arrive de la droite
    
    while(!(a3 NOIR && a4 NOIR)){
      //SWEET FUCKALL
    }
    TourSurLigne(2);
  }else 
    if (a7 NOIR) { //Arrive de la gauche
      while(!(a3 NOIR && a4 NOIR)){
        //SWEET FUCKALL
      }
      TourSurLigne(5);
    }else 
      if (a4 NOIR && a3 BLANC) { //Commence a tourner vers la gauche
        MOTOR_SetSpeed(0,vitesse*(1+pourc));
        MOTOR_SetSpeed(0,vitesse*(1-pourc));
      }else 
        if (a3 NOIR && a4 BLANC) { //Commence a tourner vers la droite
          MOTOR_SetSpeed(0,vitesse*(1-pourc));
          MOTOR_SetSpeed(0,vitesse*(1+pourc));
        }else 
          if ( a3 NOIR && a4 NOIR) { //Est droit sur la ligne
            checkSpeed(0,vitesse);
          }
            else { // A aucune idee ou il est
              checkSpeed(0,vitesse);
            }
 
}
//NE PAS UTILISER ; c'est utilisé par la fonction Ligne()
void TourSurLigne(int side) { // 2 = tourne a droite  5 = tourne a gauche
  float speed = vitesse;
  if (side == 2) {
    speed = vitesse;
  }else{
    speed = -vitesse;
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  delay(100);
  do
  {
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1,-speed);
  } while (analogRead(side) NOIR);
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  delay(100);
  

} 
//Transforme le chiffre retourné par l'infrarouge en distance en cm
float distance_cm(float dist){
  return (float)(85.624 * pow(2.7182,-0.006*dist));
}
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

  float speed = 0.2;
  int roueDroite=0, roueGauche=0;
  float bias = 1.00638;

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  if (side == 0){
     do{

    MOTOR_SetSpeed(1,speed);
    if (roueDroite > degre - 800){
      if ( speed > 0.1) speed -= 0.05 ;
    }else if (speed < .4) speed += 0.005;
    MOTOR_SetSpeed(1,speed);
    MOTOR_SetSpeed(0,-1*speed);

    roueGauche = ENCODER_Read(0) * bias;
    roueDroite = ENCODER_Read(1) ; 
    if (roueDroite > degre - 800){
      if ( speed > 0.1) speed -= 0.05 ;
      else speed = 0.1;
    }else if (speed < 0.4) speed += 0.005;

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
// ...
float CmToTic(float cm){
  return ((cm/(7.0*PI))*3600.0);
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

void ModeGuide(){
  //allo
  
}

//3925 = 90 deg
//7850 = 180 deg
void test_rien(){
  
  Serial.print(distance_cm(analogRead(8)));
  Serial.print("\t");
  Serial.print(distance_cm(analogRead(9)));
  Serial.print("\t");
  Serial.println(distance_cm(analogRead(11)));
  delay(1000);
  
}

float doublecheck_dist(int analog_input){
  float dist = 0;
  float nbr = 10;
  for(int i = 0; i < nbr; i++)
    {
      dist += distance_cm(analogRead(analog_input));    
      }
      return (dist / nbr) ;
}

void allo(){
  
  checkSpeed(0,0.3);
  if (doublecheck_dist(9) < 20){

      Stop();
      
      TournerSurLui(2010, 1);
      ENCODER_Reset(0);
      int distance_mur = distance_cm(analogRead(8));
      do{
       checkSpeed(0,0.3);
      }while(doublecheck_dist(8) < distance_mur + 15);
      int distance_1er_essai = ENCODER_Read(0);
      
      Stop();
      delay(1000);
      avance(2000);
      TournerSurLui(2010,0);
      avance(5000);
      delay(1000);

      distance_mur = doublecheck_dist(8);
      do{
       checkSpeed(0,0.3);
      }while(distance_cm(analogRead(8)) < distance_mur + 15);

      avance(2000);
      TournerSurLui(2010,0);
      avance(distance_1er_essai+2000);
      TournerSurLui(2010,1);
      delay(5000);
      avance(10000);

    
  } 
}

void checkSpeed_V2(int distance_mur){
  int temp = doublecheck_dist(8);
  int diff = abs(distance_mur - temp);
  float changement;
  float vitesse_f = 0.3;
  int nbr = 0;
  int distance_temp = 0;

  Serial.print(distance_mur);
   Serial.print(" - ");
  Serial.print(temp);
  Serial.print(" = ");
  Serial.print(diff);
  Serial.print("\t");
  

  if (diff < 2  ){
    changement = 0.05;
  } 
  else if (diff < 5 ){
    changement = 0.10;
  } else if (diff < 10 ) {
    changement = 0.20;
  }else{
    changement = 0.30;
  } 
  changement = 0.1;
  Serial.print(changement); 
  Serial.print("\t");

  if ( (temp - distance_mur) > 0){ //Trop loin
    
    
    
    MOTOR_SetSpeed(0, vitesse_f * (1-changement)); 
    MOTOR_SetSpeed(1, vitesse_f * (1+changement)); 
    Serial.println("loin");
    nbr = 1;
  }else if ((temp - distance_mur) < 0){ //Trop proche
    MOTOR_SetSpeed(0, vitesse_f * (1+changement)); 
    MOTOR_SetSpeed(1, vitesse_f * (1-changement));
    Serial.println("proche");
    nbr = 2;
  }else{
    MOTOR_SetSpeed(0, vitesse_f); 
    MOTOR_SetSpeed(1, vitesse_f);
    Serial.println("parfait");
  }
  
}

void perpendiculaire(){
  float dist_temp;
  float dist_avant = 0;
  float dist_1 = 0;
  float temp = 0;
  int j = 0;
  
      

      j = 0;
  do{

    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0.3);

    if(j == 0){
      dist_temp = doublecheck_dist(8);
    }else{
      dist_temp = dist_avant;
    }
    delay(75);

    Serial.print("1");
    Serial.print("\t");
    Serial.print(distance_cm(analogRead(8)));
    Serial.print(" < ");
    Serial.println(dist_temp);

    j++;
    dist_avant = doublecheck_dist(8);
  }while(dist_avant < dist_temp);
    j = 0;
    dist_1 = dist_avant;
    temp = dist_avant;
    dist_avant = 5;
    dist_temp = 10;

  while(dist_avant < dist_temp && dist_avant < dist_1){

    MOTOR_SetSpeed(0,0.3);
    MOTOR_SetSpeed(1,0);

    if(j == 0){
      dist_temp = doublecheck_dist(8);
    }else{
      dist_temp = dist_avant;
    }
    delay(75);

    Serial.print("2");
    Serial.print("\t");
    Serial.print(distance_cm(analogRead(8)));
    Serial.print(" < ");
    Serial.println(dist_temp);

    j++;
    dist_avant = doublecheck_dist(8);
  }
  
  
  //LAFLEUR
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  Serial.println("DONE");
}


//0 = proche   1 = loin
int changerDistanceMur(float side, float diff){

  float nbrTic = ((143.0/3.0)*(acos((9.0-(abs(diff)))/9.0)));

  Serial.print("=============== NBR = "); Serial.print(nbrTic); Serial.println(" ===============");

  if (side == 0){
    //À REMPLIR PLUS TARD
  }
  if (side == 1){
    Stop();
    if (diff > 0){
      
      while(ENCODER_Read(1) < nbrTic){
        MOTOR_SetSpeed(0,0);
        MOTOR_SetSpeed(1,0.3);
        Serial.print("1\t0\t"); Serial.println(ENCODER_Read(1));
      }
      Stop();
      while(ENCODER_Read(0) < nbrTic){
        MOTOR_SetSpeed(0,0.3);
        MOTOR_SetSpeed(1,0);
        Serial.print("1\t1\t"); Serial.println(ENCODER_Read(0));
      }
    }

    // if (diff < 0){
      
    //   while(ENCODER_Read(0) < nbrTic){
    //     MOTOR_SetSpeed(0,0.3);
    //     MOTOR_SetSpeed(1,0);
    //     Serial.print("0\t1\t"); Serial.println(ENCODER_Read(0));
    //   }
    //   Stop();
    //   while(ENCODER_Read(1) < nbrTic){
    //     MOTOR_SetSpeed(0,0);
    //     MOTOR_SetSpeed(1,0.3);
    //     Serial.print("0\t0\t"); Serial.println(ENCODER_Read(1));
    //   }
    // }
    Stop();
  }

    
    Stop();
    
   

}
    
void check_test(){

  
  float change = 0.1;
  float vit = 0.3;
  int dist_T = 0;
  int dist_temp = 0;
  int diff = 0;

  perpendiculaire();

  dist_T = doublecheck_dist(8); //Distance T du mur
  
  while(doublecheck_dist(8) < dist_T + 15){

    if (doublecheck_dist(8) > dist_T + 2  || doublecheck_dist(8) < dist_T - 2 ){
      
      perpendiculaire();

      dist_temp = doublecheck_dist(8);
      diff = dist_temp - dist_T;

      if (diff < -5 || diff > 5){
        changerDistanceMur(1,diff);
      }else{
        checkSpeed(0,0.3);
      }
    }else{
      checkSpeed(0,0.3);
    }
  }
  Stop();
  delay(3000);
  //HAHA MOI CEST JACOB
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

//Fonction pour vibrer, delayOn = temps que le moteur vibre, le delayOff = temps entre les vibrations


void bouger(float gauche, float droite){
MOTOR_SetSpeed(0,gauche);
MOTOR_SetSpeed(1,droite);
}

int direction;
int stawp =1;

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

void PrintNunchuk()
{
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

void setup(){

  BoardInit();
  //check_test();
  
  //changerDistanceMur(1,5);
}

void loop() {
  
  //allo();
  //Serial.println(distance_cm(analogRead(8)));
  perpendiculaire();
  delay(10);
}



