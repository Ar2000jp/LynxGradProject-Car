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
    static const byte c_TrigPin = 36;
    static const byte c_EchoPin = 37;

};

#endif // USRANGESENSOR_H
