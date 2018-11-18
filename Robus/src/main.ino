
/*
Projet: WalkUS
Equipe: Engenius P24
Auteurs: Engenius
Description: Robot guide d'aveugle
Date: 08/11/18
*/


#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Arduino.h>
#include <Nunchuck.h>
#include <Wire.h>

#define BLANC <100
#define NOIR >600
#define CHECKPOINTS 180

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
int vibrationPin = 3;

int loop_cnt=0;
int nunx,nuny,zbut,cbut;
int ledPin = 13;


void Test_Nunchuck(){
  // dd
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
//Pour suivre la ligne
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
//Évite les obstacles à gauche, en face ou à droite
void Obstacle(){

  int distance = 15;
  int dist_gauche = distance_cm(analogRead(8));
  int dist_avant = distance_cm(analogRead(9));
  int dist_droite = distance_cm(analogRead(11));
  int dist_correction = 0;
  int largeur_robot = 25;
  int dist_apres_correction = 0;
  int distance_cote = 0;
  int distance_1er_essai = 0;
  int distance_2e_essai = 0;
  int distance_vol_oiseau = 0;

  //Obstacle à gauche =========================================================================
  if ( dist_gauche < distance) {
    
    Stop();

    if (dist_droite > 40){ //Si y'a rien à droite
      dist_correction = 20; //Valeur standard de correction
    }else{ //Si y'a qqch à droite
      dist_correction = (dist_droite / 2) - (largeur_robot / 2) ; //Se corrige pour aller dans le milieu
    }

    //Se tasse à droite et se remet parrallèle
    tour(1,3925);
    avance(CmToTic(dist_correction));
    tour(0,3925);
    
    //Avance jusqu'à ce qu'il n'y aille plus d'objet à gauche
    dist_apres_correction = distance_cm(analogRead(8));
    while(distance_cm(analogRead(8)) <= (dist_apres_correction + 5)){
      checkSpeed(0, 0.3);
    }
    avance(CmToTic(largeur_robot));

    //Se remet sur sa trajectoire initiale
    tour(0,3925);
    avance(CmToTic(dist_correction));
    tour(1,3925);
  }
  
  //Obstacle à droite =================================================================================================
  if ( dist_droite < distance) {
    
    Stop();

    if (dist_gauche > 40){ //Si y'a rien à gauche
      dist_correction = 20; //Valeur standard de correction
    }else{ //Si y'a qqch à gauche
      dist_correction = (dist_gauche / 2) - (largeur_robot / 2) ; //Se corrige pour aller dans le milieu
    }

    //Se tasse à gauche
    tour(0,3925);
    avance(CmToTic(dist_correction)); 
    tour(1,3925);

    //Avance jusqu'à ce qu'il n'y aille plus d'objet
    dist_apres_correction = distance_cm(analogRead(11));
    while(distance_cm(analogRead(11)) <= (dist_apres_correction + 5)){ 
      checkSpeed(0, 0.3);
    }

    //Revient sur sa trajectoire initiale
    avance(CmToTic(largeur_robot));
    tour(1,3925);
    avance(CmToTic(dist_correction));
    tour(0,3925);
  }

  //Obstacle en face ==============================================================================
  if (dist_avant < distance + (largeur_robot/2)) {

    //Tourne à droite et avance jusqu'à ce qu'il n'y aille plus d'objet OU qu'il y aille quelque chose en face
    Stop();
    tour(1,3925);
    dist_correction = distance_cm(analogRead(8));
    while(distance_cm(analogRead(8)) <= (dist_correction + 5) && distance_cm(analogRead(9)) > distance){
      checkSpeed(0, 0.3);
    }
    distance_1er_essai = ENCODER_Read(0) + (largeur_robot/2); //La distance qu'il est allé vers la droite
    
    //Si il s'est rendu au bout sans problème
    if(distance_cm(analogRead(8)) >= (dist_correction + 5)){
      avance(CmToTic(largeur_robot));
    }

    //S'il y a un objet qui l'empêche il fait un 180 et essaye de l'autre côté
    // if(distance_cm(analogRead(9)) < distance){
    //   TournerSurLui(3875,0);
    //   while(distance_cm(analogRead(8)) <= (dist_correction + 5) && distance_cm(analogRead(9)) > distance){
    //     checkSpeed(0, 0.3);
    //   }
    //   distance_2e_essai = ENCODER_Read(0); //Distance qu'il a fait lorsqu'il revient sur ses pas
    //   distance_vol_oiseau = abs(distance_2e_essai - distance_1er_essai); //Distance à vol d'oiseau parcourue !!!
    // }
    

    // Se remet parrallèle à sa trajectoire initiale et avance jusqu'à ce qu'il n'y aille plus d'objet
    tour(0,3925);
    avance(1000);
    dist_apres_correction = distance_cm(analogRead(8));
    while(distance_cm(analogRead(8)) <= (dist_apres_correction + 5)){
      checkSpeed(0, 0.3);
    }

    //Retourne sur sa trajectoire initiale
    tour(0,3925);
    avance(distance_vol_oiseau);
    tour(1,3925);
  }
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
  float nbr = 50;
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
    delay(100);

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
       do{

    MOTOR_SetSpeed(0,0.3);
    MOTOR_SetSpeed(1,0);

    if(j == 0){
      dist_temp = doublecheck_dist(8);
    }else{
      dist_temp = dist_avant;
    }
    delay(100);

    Serial.print("2");
    Serial.print("\t");
    Serial.print(distance_cm(analogRead(8)));
    Serial.print(" < ");
    Serial.println(dist_temp);

    j++;
    dist_avant = doublecheck_dist(8);
  }while(dist_avant < dist_temp && dist_avant < dist_1);
  
  

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

      if (diff != 0){
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

void test_checkSpeed_V2(){
  int dist_a = doublecheck_dist(8);
  int i = 1;
  while (i > 0){
    checkSpeed_V2(dist_a);
  }
  
}

void setup(){

  BoardInit();
  //check_test();
  //perpendiculaire();
  changerDistanceMur(1,5);
}

void loop() {
  
  //allo();
  //Serial.println(distance_cm(analogRead(8)));
  
  delay(10);
}



