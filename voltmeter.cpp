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

#include "voltmeter.h"
#include "globals.h"

#include <Arduino.h>

unsigned long Voltmeter::s_PrevTime = 0;
bool Voltmeter::s_State = false;
bool Voltmeter::s_Debouncing = false;

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
