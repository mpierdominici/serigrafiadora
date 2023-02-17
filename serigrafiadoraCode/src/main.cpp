#include <Arduino.h>
#include "pins.h"
void setup() {
  pinMode(MPIN_BUZZER,OUTPUT);
  pinMode(MPIN_LED,OUTPUT);
  pinMode(MPIN_RELE1,OUTPUT);
  pinMode(MPIN_RELE2,OUTPUT);
  digitalWrite(MPIN_BUZZER,LOW);
  // put your setup code here, to run once:
}
uint8_t counterO=0;
void loop() {
  digitalWrite(MPIN_LED,HIGH);
  delay(300); 
   digitalWrite(MPIN_LED,LOW);
 delay(300);
 //  digitalWrite(MPIN_RELE1,HIGH);
 // delay(300); 
//   digitalWrite(MPIN_RELE1,LOW);
 // delay(300);
 //  digitalWrite(MPIN_RELE2,HIGH);
 // delay(300); 
 //  digitalWrite(MPIN_RELE2,LOW);
  //delay(300);
  //counterO++;
  if(counterO>20){
    counterO=0;
    digitalWrite(MPIN_BUZZER,HIGH);
  delay(100); 
   digitalWrite(MPIN_BUZZER,LOW);
  delay(100);
  }
  // put your main code here, to run repeatedly:
}