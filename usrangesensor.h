#ifndef USRANGESENSOR_H
#define USRANGESENSOR_H

#include "Arduino.h"

#include "globals.h"

class USRangeSensor
{
public:
    USRangeSensor();
    ~USRangeSensor();

    byte getDistance();
    void update();

private:
    static bool s_Initialized;
    static byte s_Distance;
    static unsigned long s_Duration;
    static const byte c_TrigPin = A6;
    static const byte c_EchoPin = A7;
    static unsigned long s_PrevUpdateTime;
    static const unsigned int c_UpdateInterval = 1000;

};

#endif // USRANGESENSOR_H
