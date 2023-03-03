#include "hw039.h"


//void Rectangle::set_values (int x, int y) {
 // width = x;
 // height = y;
//}

hw039::hw039(uint32_t enPin, uint32_t lPin, uint32_t rPin){
    this->enpin=enPin;
    this->lPin=lPin;
    this->rPin=rPin;

    pinMode(this->lPin,OUTPUT);//inicializo el pinout del puente h
    pinMode(this->rPin,OUTPUT);
    digitalWrite(this->lPin,LOW);
    digitalWrite(this->rPin,LOW);

    motorOn=false;
    duty=0;
    isClock=true;
    //analogWrite(this->enpin,duty);
}

  void hw039::setSpeed(int16_t speed){
    
    if(speed>0){
        duty=speed;
        digitalWrite(lPin,LOW);
        digitalWrite(rPin,HIGH);
        analogWrite(enpin,duty);
        isClock=true;
        motorOn=true;

    }else if(speed<0){
        duty=-speed;
        digitalWrite(lPin,HIGH);
        digitalWrite(rPin,LOW);
        analogWrite(enpin,duty);
        isClock=false;
        motorOn=true;
    }else if(speed==0){
        digitalWrite(lPin,LOW);
        digitalWrite(rPin,LOW);
        analogWrite(enpin,0);
        motorOn=false;
    }


  }

  bool hw039::isMotorOn(void){
    return motorOn;
  }

  bool hw039::isClockWise(void){
    return isClock;
  }