#include <Arduino.h>
#include "pins.h"
#include "U8g2lib.h"
#include <Wire.h>
#include <PID_v1.h>
#include "hw039.h"
#include "myTimer.h"

//******************CTE PID ENTINTADOR**********************//
#define KP_ENTINTADOR 30
#define KI_ENTINTADOR 2
#define KD_ENTINTADOR 6

#define SAMPLES_2_CM_B 303.259
#define SAMPLES_2_CM_M -0.3888

//****************************************//

void updateScreen(void);
double getPosEntintador(void);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ MPIN_SCL_, /* data=*/ MPIN_SDA); 

double setPointEntintador, inputEntintador, outputEntintador;//variables asociades al control de posicion del entintador
PID pidEntintador(&inputEntintador, &outputEntintador, &setPointEntintador,KP_ENTINTADOR,KI_ENTINTADOR,KD_ENTINTADOR, DIRECT);//pid del entintador
hw039   * motorEntintado;

myTimer timerLedBlink(300);
myTimer timerUpdateScreen(400);
myTimer pidEntintadorLoop(200);
myTimer timerTest(3000);

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
bool tggle=false;
void loop() {
  if(timerLedBlink.timeOver()){
    digitalWrite(MPIN_LED,!digitalRead(MPIN_LED));
  }
  if(timerUpdateScreen.timeOver()){
    updateScreen();
 
  }
 if(pidEntintadorLoop.timeOver()){
  
    inputEntintador=getPosEntintador();
    pidEntintador.Compute();
    motorEntintado->setSpeed(outputEntintador);
    
 }

 if(timerTest.timeOver()){
  if(tggle){
    setPointEntintador=10;

  }else{
    setPointEntintador=40;

  }
  tggle=!tggle;
   
 }
 // inputEntintador=analogRead(MPIN_POSICION_ENTINTADOR);
  
 
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

double getPosEntintador(void){
  static double lastMesure=0;
  double newSample;
  newSample = analogRead(MPIN_POSICION_ENTINTADOR) * SAMPLES_2_CM_M + SAMPLES_2_CM_B;
  newSample=(newSample + lastMesure) / 2;
  lastMesure=newSample;
  return newSample;
}