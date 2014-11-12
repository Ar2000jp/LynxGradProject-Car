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
