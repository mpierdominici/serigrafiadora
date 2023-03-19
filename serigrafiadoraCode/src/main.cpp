#include <Arduino.h>
#include "pins.h"
#include "U8g2lib.h"
#include <Wire.h>
#include <PID_v1.h>
#include "hw039.h"
#include "myTimer.h"
#include <AccelStepper.h>

//******************CTE PID ENTINTADOR**********************//
#define KP_ENTINTADOR 10  //50
#define KI_ENTINTADOR 0.2 //2
#define KD_ENTINTADOR 1 //6

#define SAMPLES_2_CM_B 303.259
#define SAMPLES_2_CM_M -0.3888

#define PWM_DEAD_ZONE 70
//****************************************//

void updateScreen(void);
double getPosEntintador(void);
double deadZoneMotor(double dZspeed);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ MPIN_SCL_, /* data=*/ MPIN_SDA); 

double setPointEntintador, inputEntintador, outputEntintador;//variables asociades al control de posicion del entintador
PID pidEntintador(&inputEntintador, &outputEntintador, &setPointEntintador,KP_ENTINTADOR,KI_ENTINTADOR,KD_ENTINTADOR, DIRECT);//pid del entintador
hw039   * motorEntintado;

myTimer timerLedBlink(300);
myTimer timerUpdateScreen(400);
myTimer pidEntintadorLoop(200);
myTimer timerTest(3000);

AccelStepper stepper1(AccelStepper::FULL2WIRE,MPIN_STEPPER_STEP1,MPIN_STEPPER_DIR1);
AccelStepper stepper2(AccelStepper::FULL2WIRE,MPIN_STEPPER_STEP2,MPIN_STEPPER_DIR2);

uint8_t testSecuenciador=0; //TODO:Sacar es para probar el movimiento completo

void setup() {
  pinMode(MPIN_BUZZER,OUTPUT);
  pinMode(MPIN_LED,OUTPUT);
  pinMode(MPIN_RELE1,OUTPUT);
  pinMode(MPIN_RELE2,OUTPUT);

  pinMode(MPIN_IN_BTN1,INPUT);
  pinMode(MPIN_IN_BTN2,INPUT);
  pinMode(MPIN_IN_BTN3,INPUT);
  pinMode(MPIN_IN_BTN4,INPUT);

  pinMode(MPIN_STEPPER_ENABLE,OUTPUT);
  
  pinMode(MPIN_STEPPER_ENABLE,OUTPUT);
  digitalWrite(MPIN_STEPPER_ENABLE,HIGH);
  digitalWrite(MPIN_STEPPER_ENABLE,LOW);
  digitalWrite(MPIN_BUZZER,LOW);
  digitalWrite(MPIN_RELE1,LOW);
  digitalWrite(MPIN_RELE2,LOW);

  motorEntintado=new hw039(MPIN_PWM_HBRIDGE_LV1,MPIN_PWM_HBRIDGE_LV2,MPIN_PWM_HBRIDGE_LV3);
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  //motorEntintado->setSpeed(-100);
  
 // analogWrite(MPIN_PWM_HBRIDGE_LV1,100);
  setPointEntintador=5;
  pidEntintador.SetOutputLimits(-(255-PWM_DEAD_ZONE),(255-PWM_DEAD_ZONE));
  pidEntintador.SetMode(AUTOMATIC);

   stepper1.setMaxSpeed(2000.0);
   stepper1.setAcceleration(1000);
   stepper2.setMaxSpeed(400.0);
   stepper2.setAcceleration(200.0);
  
  
 
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
    if(testSecuenciador==0){
      motorEntintado->setSpeed(0);
    }else{
      motorEntintado->setSpeed(deadZoneMotor(outputEntintador));
    }
    
    
 }
 if(digitalRead(MPIN_IN_BTN1)==LOW){//si se aprieta el boton inicio la secuencia de movimiento
  // stepper1.moveTo(1000); //isrunning para saber si esta ejecutando un movimiento el motor
   if(testSecuenciador==0){testSecuenciador=1;}
   // stepper1.moveTo(1000);//bajo la manigueta
  }


  switch (testSecuenciador)
  {
  case 1:
    /* code */
    digitalWrite(MPIN_STEPPER_ENABLE,LOW);
    stepper1.moveTo(-1000);//bajo la manigueta
    testSecuenciador++;
    break;
  case 2:
    if (!stepper1.isRunning()){testSecuenciador++;digitalWrite(MPIN_STEPPER_ENABLE,HIGH);}
    /* code */
    break;
  case 3:
    setPointEntintador=30;//pongo el setpoint en 30cm
    testSecuenciador++;
    /* code */
    break;
  case 4:
    if ((inputEntintador <= setPointEntintador+3)   &&  (inputEntintador>= setPointEntintador - 3))//llegue a la posicion del setpoint
    {
      testSecuenciador++;
    }
    /* code */
    break;
  case 5:
    digitalWrite(MPIN_STEPPER_ENABLE,LOW);
    stepper1.moveTo(0);//subo la manigueta
    testSecuenciador++;
    /* code */
    break;
  case 6:
   if (!stepper1.isRunning()){testSecuenciador++;digitalWrite(MPIN_STEPPER_ENABLE,HIGH);}
    /* code */
    break;
  case 7:
  setPointEntintador=5;//pongo el setpoint en 0cm
    testSecuenciador++;
  break;
  case 8:
   if ((inputEntintador <= setPointEntintador+3)   &&  (inputEntintador>= setPointEntintador - 3))//llegue a la posicion del setpoint
    {
      testSecuenciador=0;
    }
  break;
  
  default:
    break;
  }





  stepper1.run();//Stepper ejecucion de pasos
  stepper2.run(); 
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
  if(digitalRead(MPIN_IN_BTN1)==LOW){
    u8g2.drawCircle(10,10,5);
  }
  
  u8g2.drawStr(10,25,String(testSecuenciador).c_str());
  
  
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

double deadZoneMotor(double dZspeed){

  if(dZspeed<0.5 && dZspeed>(-0.5)){
    dZspeed=0;
  }else{
    if(dZspeed>0){
      dZspeed=dZspeed+PWM_DEAD_ZONE;
    }else{
      dZspeed=dZspeed-PWM_DEAD_ZONE;
    }
    
  }
  return dZspeed;



}