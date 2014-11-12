/*
 * This file is part of LynxGradProject.
 *
 * LynxGradProject is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LynxGradProject is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LynxGradProject.  If not, see <http://www.gnu.org/licenses/>.
 */

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
