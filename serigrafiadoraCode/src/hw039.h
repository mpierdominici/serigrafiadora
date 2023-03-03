#ifndef HW039_H
#define HW039_H
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

class hw039 {      
  public:

  hw039(uint32_t enPin, uint32_t lPin, uint32_t rPin);//constructor por defecto, recive como para metro los pines del puente h
  void setSpeed(int16_t speed);// speed>0 giro clockwise , speed<0 anti clockwise. 0 motor detenido
  bool isMotorOn(void);// true si el motor esta prendido
  bool isClockWise(void);//true si el sentido de giro es clockwise

  private:
  bool isClock;
  bool motorOn;
  uint32_t enpin;
  uint32_t lPin;
  uint32_t rPin;
  uint8_t duty;
};


#endif