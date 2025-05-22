#include <LiquidCrystal.h>



const int LED1 = 7;
const int BUTTON = 13;
const int BUZZER = 9;
const int LEDTEST =6;

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
int lastInterrupteurState = LOW;
String lettre;

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
    String current_word;  
    len_cw=0;
    space=0;
    translate=1;
  }
  lastInterrupteurState = interrupteurState;


}
void mode2() {
  digitalWrite(LEDTEST, HIGH);

}

void mode1() {
  digitalWrite(LEDTEST, LOW);


  buttonState = digitalRead(BUTTON);

  if (buttonState == HIGH) {
    Serial.println("hello");
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
        lettre = dechiffrage(current_word);
        if(lettre != "") {
          lcd.print(lettre);
          col++;
        }
        lettre = "";
        current_word ="";
        col-=len_cw;
        len_cw = 0;
        
      }
      delay(500);
      
  }
  if(last_state ==LOW and (millis()-last_time)>=3000 and space == 0){
    space =1;
    col++;
  
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
