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
