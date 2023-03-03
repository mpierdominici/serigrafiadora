#ifndef MY_TIMER_H_
#define MY_TIMER_H_
#include<Arduino.h>
#include<stdint.h>


class myTimer
{
  public:
    myTimer(uint32_t seconds = 0); // lo canvie a ms
    bool timeOver(void);
    void setNewTime(uint32_t seconds_);
    void showInfo();
    bool timeOverSinReset(void);
    uint32_t seconds;
    uint32_t startTime;
    void resetTimer(void);

};

#endif//MNP_ENCODER_H_
