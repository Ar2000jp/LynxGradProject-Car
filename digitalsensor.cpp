#include "digitalsensor.h"

#include <Arduino.h>

DigitalSensor::DigitalSensor(byte pinNo, unsigned int debounceTime, bool activeLow)
{
    m_PinNo = pinNo;
    m_DebounceTime = debounceTime;
    m_ActiveLow = activeLow;
    m_PrevTime = 0;
    m_State = activeLow;
    m_Debouncing = false;
    pinMode(m_PinNo, INPUT);
}

DigitalSensor::~DigitalSensor()
{

}

bool DigitalSensor::getValue()
{
    if (m_ActiveLow) {
        return !m_State;
    } else {
        return m_State;
    }
}

void DigitalSensor::update()
{
    if (digitalRead(m_PinNo) != m_State) {
        if (m_Debouncing == false) {
            m_Debouncing = true;
            m_PrevTime = millis();
            return;
        } else if (millis() > (m_PrevTime + m_DebounceTime)) {
            m_State = digitalRead(m_PinNo);
            m_Debouncing = false;
        }
    } else {
        m_Debouncing = false;
    }
}
