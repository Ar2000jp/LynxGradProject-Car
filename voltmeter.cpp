#include "voltmeter.h"
#include "globals.h"

#include <Arduino.h>

Voltmeter::Voltmeter()
{
    pinMode(c_VoltmeterPin, INPUT);
    s_PrevTime = 0;
    s_State = false;
    s_Debouncing = false;
}

Voltmeter::~Voltmeter()
{

}

bool Voltmeter::getUndervolt()
{
    return s_State;
}

void Voltmeter::update()
{
    if ((analogRead(c_VoltmeterPin) <= c_Threshold) != s_State) {
        if (s_Debouncing == false) {
            s_Debouncing = true;
            s_PrevTime = millis();
            return;
        } else if (millis() > (s_PrevTime + c_DebounceTime)) {
            s_State = (analogRead(c_VoltmeterPin) <= c_Threshold);
            s_Debouncing = false;
        }
    } else {
        s_Debouncing = false;
    }
}
