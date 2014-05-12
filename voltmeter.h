#ifndef VOLTMETER_H
#define VOLTMETER_H

#include "Arduino.h"

class Voltmeter
{
public:
    Voltmeter();
    ~Voltmeter();

private:
    void update();
    bool getUndervolt();

    static const byte c_VoltmeterPin = A0;
    static const unsigned int c_Threshold = 512;
    static const unsigned int c_DebounceTime = 2000;

    static unsigned long s_PrevTime;
    static bool s_State;
    static bool s_Debouncing;

};

#endif // VOLTMETER_H
