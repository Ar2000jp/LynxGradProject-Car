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

#ifndef VOLTMETER_H
#define VOLTMETER_H

#include "Arduino.h"

class Voltmeter
{
public:
    Voltmeter();
    ~Voltmeter();

    void update();
    bool getUndervolt();

private:
    static const byte c_VoltmeterPin = A0;
    static const unsigned int c_Threshold = 512;
    static const unsigned int c_DebounceTime = 2000;

    static unsigned long s_PrevTime;
    static bool s_State;
    static bool s_Debouncing;

};

#endif // VOLTMETER_H
