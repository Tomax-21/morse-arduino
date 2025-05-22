#include <LiquidCrystal.h>



const int LED1 = 7;
const int BUTTON = 13;
const int BUZZER = 9;
const int LEDTEST =6;
const int LEDG = 10;

const int INTERRUPTEUR = 8;


int ledState = LOW;
int buttonState;
int last_state = LOW;
unsigned long last_time;
unsigned long time;
int line = 0;
int col = 0;
String current_word;
int len_cw =0;
int space = 1;
int translate = 1;
LiquidCrystal lcd(12,11,5,4,3,2);

int interrupteurState = LOW;
int lastInterrupteurState = HIGH;
String lettre;
String lettreADeviner;
char alphabet[26]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int nombre_aleatoire;
int reset = 0;

void setup() {
  
  
  Serial.begin(9600);
  pinMode(LEDG, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LEDTEST,OUTPUT);
  lcd.begin(16,2);
  buttonState = digitalRead(BUTTON);
  last_time = millis();
  lcd.setCursor(col,line);
  

}

void loop() {
  interrupteurState = digitalRead(INTERRUPTEUR);

  if (interrupteurState == HIGH) {
    mode1();
  } else {
    mode2();
  }


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

  
  //newGameMode2();
  checkButton();
  writeLetter();

  if(buttonState == HIGH and last_state ==LOW){
    translate=0;
    last_state = HIGH;
    last_time = millis();
  }

  if(last_state ==LOW and (millis()-last_time)>=1000 and translate == 0){
      //addSpace();
      Serial.println("bojnout");
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
        digitalWrite(LEDTEST, HIGH);
        tone(BUZZER, 100, 700);
        delay(1000);
        digitalWrite(LEDTEST, LOW);
      }

      
      newGameMode2();
      
  }

}

void initGame2() {
  current_word = "";  
  len_cw=0;
  space=0;
  translate=1;
}

void newGameMode2() {
  initGame2();
  randomSeed(analogRead(A0));
  nombre_aleatoire=random(0,25); // On créer un nmbre aléaoire entre 1 et 100
  lettreADeviner = alphabet[nombre_aleatoire];
  lcd.clear();

  lcd.print("###### ");
  lcd.print(lettreADeviner);
  lcd.print(" #######");

  lcd.setCursor(0,1);
  
}

void mode1() {
  digitalWrite(LEDTEST, LOW);


  checkButton();
  writeLetter();

  if(last_state ==LOW and (millis()-last_time)>=1000 and translate == 0){
      addSpace();
    
      
  }
  if(last_state ==LOW and (millis()-last_time)>=3000 and space == 0){
    if (line != 0 or col != 0) {

      space =1;
      col++;
    }
  }
  if(buttonState == HIGH and last_state ==LOW){
    translate=0;
    last_state = HIGH;
    last_time = millis();

  }
  
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
if(buttonState == LOW and last_state == HIGH){
    last_state = LOW;
    translate=0;
    time = millis() - last_time;
    last_time = millis();
    if(time > 5 and time < 200){ //si appuie entre 5 et 150ms écrit un point
      col++;
      if (col ==16 and line ==1){
        lcd.setCursor(0, 0);
        lcd.print(current_word);
        line = 0;
        col =len_cw+1;
        reset = 1;
      }
      
      lcd.print(".");
      current_word += ".";
      len_cw ++;
    }
    else if(time >=150){ // si appuie supèrieur à 150ms écrit un trait
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
  buttonState = digitalRead(BUTTON);

  if (buttonState == HIGH) {
    ledState=HIGH;
    tone(BUZZER, 440, 10);
  } else {
    ledState=LOW;
  }
  digitalWrite(LED1, ledState);
}
void addSpace() {
  translate =1;
  space = 0;
  if(len_cw>0){
          if(reset == 1){
            lcd.clear();
            lcd.setCursor(0,0);
            col = 0;
            line = 0;
          }
          else{
          delete_word(len_cw, col, line);
          }
          lettre = dechiffrage(current_word);
          if(lettre != "") {
            lcd.print(lettre);
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
  if(len <= col){
    for(int i =0; i<= len ;i++){
      lcd.setCursor(col-i,line);
      lcd.print(" ");
    }
    lcd.setCursor(col-len, line);
  }else{
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
