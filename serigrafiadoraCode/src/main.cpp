#include <Arduino.h>
#include "pins.h"
#include "U8g2lib.h"
#include <Wire.h>
#include <PID_v1.h>
#include "hw039.h"
#include "myTimer.h"

//******************CTE PID ENTINTADOR**********************//
#define KP_ENTINTADOR 100
#define KI_ENTINTADOR 2.0
#define KD_ENTINTADOR 6.0

//****************************************//

void updateScreen(void);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ MPIN_SCL_, /* data=*/ MPIN_SDA); 

double setPointEntintador, inputEntintador, outputEntintador;//variables asociades al control de posicion del entintador
PID pidEntintador(&inputEntintador, &outputEntintador, &setPointEntintador,KP_ENTINTADOR,KI_ENTINTADOR,KD_ENTINTADOR, DIRECT);//pid del entintador
hw039   * motorEntintado;

myTimer timerLedBlink(300);
myTimer timerUpdateScreen(400);
myTimer pidEntintadorLoop(200);

void setup() {
  pinMode(MPIN_BUZZER,OUTPUT);
  pinMode(MPIN_LED,OUTPUT);
  pinMode(MPIN_RELE1,OUTPUT);
  pinMode(MPIN_RELE2,OUTPUT);
  digitalWrite(MPIN_BUZZER,LOW);
  digitalWrite(MPIN_RELE1,LOW);
  digitalWrite(MPIN_RELE2,LOW);
  motorEntintado=new hw039(MPIN_PWM_HBRIDGE_LV1,MPIN_PWM_HBRIDGE_LV2,MPIN_PWM_HBRIDGE_LV3);
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  //motorEntintado->setSpeed(-100);
  
 // analogWrite(MPIN_PWM_HBRIDGE_LV1,100);
  setPointEntintador=20.0;
  pidEntintador.SetOutputLimits(-255.0,255.0);
  pidEntintador.SetMode(AUTOMATIC);
  
  
 
}

void loop() {
  if(timerLedBlink.timeOver()){
    digitalWrite(MPIN_LED,!digitalRead(MPIN_LED));
  }
  if(timerUpdateScreen.timeOver()){
    updateScreen();
 
  }
 if(pidEntintadorLoop.timeOver()){
    
    pidEntintador.Compute();
    
 }
  
 
}

void updateScreen(void){
    u8g2.clearBuffer();	
  //u8g2.drawStr(64,32,String(counterO).c_str());
  
  u8g2.drawStr(64,16,String(analogRead(MPIN_MED_CORRIENTE)).c_str());
  u8g2.drawStr(30,16,"Cor");

  
  u8g2.drawStr(64,32,String(setPointEntintador).c_str());
  u8g2.drawStr(30,32,"Sp");

  
  u8g2.drawStr(64,48,String(outputEntintador).c_str());
  u8g2.drawStr(30,48,"Out");

  
  u8g2.drawStr(64,62,String(inputEntintador).c_str());
  u8g2.drawStr(30,62,"In");
  
  
  u8g2.sendBuffer();
}


/*

 u8g2.clearBuffer();	
  u8g2.drawStr(64,32,String(counterO).c_str());
  u8g2.drawStr(64,62,String(analogRead(MPIN_MED_CORRIENTE)).c_str());
  //u8g2.print(String(counterO).c_str());
  u8g2.sendBuffer();
  // put your main code here, to run repeatedly:

*/