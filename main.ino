#include <LiquidCrystal.h>



const int LED1 = 7;
const int BUTTON = 13;
const int BUZZER = 9;
const int LEDTEST =6;

int ledState = LOW;
int buttonState;
int last_state = LOW;
int last_time;
int time;
int line = 0;
int col = 0;
String current_word;
int len_cw =0;
int space = 1;
int translate = 0;
LiquidCrystal lcd(12,11,5,4,3,2);



void setup() {
  
  
  Serial.begin(9600);
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
  buttonState = digitalRead(BUTTON);

  if (buttonState == HIGH) {
    ledState=HIGH;
    tone(BUZZER, 440, 10);
  } else {
    ledState=LOW;
  }

  digitalWrite(LED1, ledState);
  
  if(buttonState == LOW and last_state == HIGH){
    last_state = LOW;
    translate=0;
    time = millis() - last_time;
    last_time = millis();
    if(time > 5 and time < 150){ //si appuie entre 5 et 150ms écrit un point
      lcd.print(".");
      current_word += ".";
      len_cw ++;
      col++;
    }
    else if(time >=150){ // si appuie supèrieur à 150ms écrit un trait
      lcd.print("-");
      current_word += "-";
      len_cw ++;
      col++;
    }
  }
  if(last_state ==LOW and (millis()-last_time)>=1000 and translate == 0){
    translate =1;
    space = 0;
    if(len_cw>0){
        delete_word(len_cw, col, line);
        String lettre = dechiffrage(current_word);
        if(lettre != "") {
          lcd.print(lettre);
          col++;
        }
        
        current_word ="";
        col-=len_cw;
        len_cw = 0;
        
      }
  }
  if(last_state ==LOW and (millis()-last_time)>=3000 and space == 0){
    pinMode(LEDTEST,HIGH);
    space =1;
    col++;
    pinMode(LEDTEST,LOW);
  
  }
  if(buttonState == HIGH and last_state ==LOW){
    translate=0;
    last_state = HIGH;
    last_time = millis();

  }
  if(col>=16){
    col = 0;
    line++;
  }
  if(line>1){
    line=0;
    lcd.clear();
  }
  
  lcd.setCursor(col, line);

}

void delete_word(int len, int col, int line){
  if(len <= col){
    digitalWrite(LEDTEST,HIGH);
    lcd.setCursor(col-len, line);
    lcd.print("                ");
    lcd.setCursor(col-len, line);
    digitalWrite(LEDTEST,LOW);
  }else{
    lcd.setCursor(0,line);
    lcd.print("                 ");
    lcd.setCursor(16-len-col,line-1);
    lcd.print("                 ");
    lcd.setCursor(16-len-col,line-1);
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
