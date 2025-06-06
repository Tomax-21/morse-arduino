#include <LiquidCrystal.h>



const int LED1 = 7; // pin de la led qui affiche l'etat du bouton
const int BUTTON = 13; // pin du bouton bip bip
const int BUZZER = 9; // pin du buzzer bzzz bzzz
const int LEDREPONSE =6; // pin de la led qui affiche la reponse dans le mode 1 (pin rouge)
const int LEDG = 10; // deuxiemem pin de la led rgb
const int INTERRUPTEUR = 8; // pin qui recupere le status de l'interrupteur


int ledState = LOW; // etat de la led
int buttonState; // etat du bouton
int last_state = LOW; // etat precedent du bouton
unsigned long last_time; // temps precedent pour faire un delta temps
unsigned long time; // temps
int line = 0; // ligne sur laquelle on affiche sur l'ecran lcd
int col = 0; // colonne sur laquelle on affiche sur l'ecran lcd
String current_word; // mot qui s'affiche 
int len_cw =0; // longueur du mot
int space = 1; // si on veut mettre un espace
int translate = 1; // si on veut traduire de morse vers alphabet
LiquidCrystal lcd(12,11,5,4,3,2); // parametre de l'ecran lcd

int interrupteurState = LOW; // etat de l'interrupteur
int lastInterrupteurState = HIGH; // etat precedent de l'interrupteur, pour detecter une variation
String lettre; // lettre qu'on rentre
String lettreADeviner; // la lettre que l'on doit deviner
char alphabet[26]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int nombre_aleatoire; // nombre aleatoire pour choisir une lettre
int reset = 0; // quand on veut reset l'ecran

void setup() {
  // initialisation des pins
  Serial.begin(9600);
  pinMode(LEDG, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LEDREPONSE,OUTPUT);
  lcd.begin(16,2);
  buttonState = digitalRead(BUTTON);
  last_time = millis(); // mise en place du temps
  lcd.setCursor(col,line); // mise en place du curseur
  

}

void loop() {
  // on lit l'interrupteur pour savoir dans quel mode on veut jouer
  interrupteurState = digitalRead(INTERRUPTEUR);

  if (interrupteurState == HIGH) {
    mode1();
  } else {
    mode2();
  }

  // on detecte une variation, dans ce cas, on reset toute les données des variables necessaires
  if (lastInterrupteurState != interrupteurState) {
    lcd.clear();
    lcd.setCursor(0,0);
    line = 0;
    col = 0;
    current_word = "";  
    len_cw=0;
    space=0;
    translate=1;

    if (interrupteurState == LOW) {
      newGameMode2();
    }
  }
  lastInterrupteurState = interrupteurState;

}

void mode2() {
  // deuxieme mode de jeu

  checkButton();
  writeLetter();

  if(buttonState == HIGH and last_state ==LOW){
    translate=0;
    last_state = HIGH;
    last_time = millis();
  }

  if(last_state ==LOW and (millis()-last_time)>=1000 and translate == 0){
      //addSpace();
      Serial.println("bonjour");
      Serial.println(current_word);
      lettre = dechiffrage(current_word);
      translate =1;
      Serial.println(lettre);

      if (lettre == lettreADeviner) {
        digitalWrite(LEDG, HIGH);
        tone(BUZZER, 1000, 700);
        delay(1000);
        digitalWrite(LEDG, LOW);
      }
      else{
        digitalWrite(LEDREPONSE, HIGH);
        tone(BUZZER, 100, 700);
        delay(1000);
        digitalWrite(LEDREPONSE, LOW);
      }

      
      newGameMode2();
      
  }

}

void initGame2() {
  // on reset les fonctions necessaire pour le bon deroulement de la partie et eviter les interferences si on a deja joué au jeu précédent
  current_word = "";  
  len_cw=0;
  space=0;
  translate=1;
}

void newGameMode2() {
  // on initialise les valeurs de depart
  initGame2();
  // mise en place de l'aléatoire
  randomSeed(analogRead(A0));

  // on choisi une lettre aléatoire
  nombre_aleatoire=random(0,25); // On créer un nmbre aléaoire entre 1 et 100
  lettreADeviner = alphabet[nombre_aleatoire];

  // mise en place de l'interface
  lcd.clear();
  lcd.print("###### ");
  lcd.print(lettreADeviner);
  lcd.print(" #######");

  lcd.setCursor(0,1);
  
}

void mode1() {
  //
  digitalWrite(LEDREPONSE, LOW);

  checkButton();
  writeLetter();

  // si le bouton n'est pas appuyé et qu'on a rien fait depuis plus de 1sec et qu'on a pas deja traduit
  if(last_state ==LOW and (millis()-last_time)>=1000 and translate == 0){
    // on traduit le mot et on incremente le curseur
      addSpace();
    
      
  }

  // si le bouton n'est pas appuyé depuis plus de 3 sec et qu'on a deja traduit, alors on fait un espace
  if(last_state ==LOW and (millis()-last_time)>=3000 and space == 0){
    if (line != 0 or col != 0) {
      space =1;
      col++;
    }
  }

  //si on appuis, on met a jour le temps et on dit que translate = 0 ce qui signifie que la prochaine fois qu'on attendra plus de 1sec sans rien faire, alors il faudra traduire
  if(buttonState == HIGH and last_state ==LOW){
    translate=0;
    last_state = HIGH;
    last_time = millis();

  }
  
  // si on est au bout de l'ecran, on supprime ce qu'il y a dessus et on redemarre au debut
  if(col>=16){
    col = 0;
    if(line ==0){
      line = 1;
    }
    else if(line == 1){
      line = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      line=0;
      col=0;
    }
  }
  
  lcd.setCursor(col, line);

}

void writeLetter() {
  //si on relache le bouton
  if(buttonState == LOW and last_state == HIGH){
    last_state = LOW;
    translate=0;
    // on met a jour le temps
    time = millis() - last_time;
    last_time = millis();
    // si on a appuyé pendant entre 5 et 175ms, alors on ecrit un . au bon endroit
    if(time > 5 and time < 175){ //si appuie entre 5 et 150ms écrit un point
      col++;
      if (col ==16 and line ==1){
        // si on est au bout, de l'ecrant, on revient au debut
        lcd.setCursor(0, 0);
        lcd.print(current_word);
        line = 0;
        col =len_cw+1;
        reset = 1;
      }
      
      // ecriture sur l'ecran
      lcd.print(".");
      // on met a jour les variables necessaires au traitement
      current_word += ".";
      len_cw ++;
    }
    else if(time >=175){ // si appuie superieur à 175ms écrit un trait -
      col++;
      if (col ==16 and line ==1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(current_word);
        line = 0;
        col =len_cw+1;
        reset = 1;
      }
      lcd.print("-");
      current_word += "-";
      len_cw ++;
    }
  }
}
void checkButton() {
  // cet fonction met à jour la led et le son en synchronisation avec l'etat du bouton
  buttonState = digitalRead(BUTTON);

  // si le bouton est appuyé
  if (buttonState == HIGH) {
    // on allume la led et on joue un son
    ledState=HIGH;
    tone(BUZZER, 440, 10);
  } else {
    // sinon on eteint juste la led
    ledState=LOW;
  }
  digitalWrite(LED1, ledState);
}

void addSpace() {
  translate =1; //on active la traduction pour le prochain caractère
  space = 0; //on desactive l'espace pour pas en mettre plusieurs de suite
  if(len_cw>0){ //si un mot est en cours d'écriture
    if(reset == 1){ //si le reset est activé
      lcd.clear();
      lcd.setCursor(0,0);
      col = 0;
      line = 0;
    }
    else{
      delete_word(len_cw, col, line); //on supprime le mot atuel
    }

    lettre = dechiffrage(current_word);//on récupère la lettre correspondante au code morse
    if(lettre != "") { 
      lcd.print(lettre); //on affiche la lettre si elle existe
      col++;
    }
    
    lettre = "";
    current_word ="";
    if(reset == 1){
      reset = 0;
    }
    else{
      col-=len_cw;
    }
    len_cw = 0;
          
  }
}
void delete_word(int len, int col, int line){
  if(len <= col){ //si on ne doit pas gerer de changement de ligne
    for(int i =0; i<= len ;i++){
      lcd.setCursor(col-i,line); //on supprime les caractère un par un
      lcd.print(" ");
    }
    lcd.setCursor(col-len, line);
  }else{ //pareil en gérant le changement de ligne
    int l1 = len -col;
    for(int i = 0; i<=col; i++){
      lcd.setCursor(col-i,line);
      lcd.print(" ");
    }
    line = (line+1)%2;
    for(int i = 0; i<=l1;i++){
      lcd.setCursor(16-i,line);
      lcd.print(" ");
    }
    lcd.setCursor(16-l1,line);
  }  
}

String dechiffrage(String current_word){
  // on rentre un mot en morse et ça nous retourne la lettre correspondante
  String lettre;
  if (current_word== ".-"){
    lettre = "A";
  } else if (current_word== "-..."){
    lettre = "B";
  } else if (current_word== "-.-."){
    lettre = "C";
  } else if (current_word== "-.."){
    lettre = "D";
  } else if (current_word== "."){
    lettre = "E";
  } else if (current_word== "..-."){
    lettre = "F";
  } else if (current_word== "--."){
    lettre = "G";
  } else if (current_word== "...."){
    lettre = "H";
  }else if (current_word== ".."){
    lettre = "I";
  } else if (current_word== ".---"){
    lettre = "J";
  } else if (current_word== "-.-"){
    lettre = "K";
  } else if (current_word== ".-.."){
    lettre = "L";
  } else if (current_word== "--"){
    lettre = "M";
  } else if (current_word== "-."){
    lettre = "N";
  } else if (current_word== "---"){
    lettre = "O";
  } else if (current_word== ".--."){
    lettre = "P";
  } else if (current_word== "--.-"){
    lettre = "Q";
  } else if (current_word== ".-."){
    lettre = "R";
  } else if (current_word== "..."){
    lettre = "S";
  } else if (current_word== "-"){
    lettre = "T";
  } else if (current_word== "..-"){
    lettre = "U";
  } else if (current_word== "...-"){
    lettre = "V";
  } else if (current_word== ".--"){
    lettre = "W";
  } else if (current_word== "-..-"){
    lettre = "X";
  } else if (current_word== "-.--"){
    lettre = "Y";
  } else if (current_word== "--.."){
    lettre = "Z";
  } else{
    lettre = "";
  }
  return lettre;
}
