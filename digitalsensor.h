#ifndef DIGITALSENSOR_H
#define DIGITALSENSOR_H

#include <Arduino.h>

#include "globals.h"

class DigitalSensor
{
public:
    DigitalSensor(byte pinNo, unsigned int debounceTime, bool activeLow = false);
    ~DigitalSensor();

    bool getValue();
    void update();

private:
    byte m_PinNo;
    unsigned int m_DebounceTime;
    unsigned long m_PrevTime;
    bool m_State;
    bool m_Debouncing;
    bool m_ActiveLow;

};

#endif // DIGITALSENSOR_H
