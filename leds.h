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

#ifndef LEDS_H
#define LEDS_H

#include "Arduino.h"
#include "globals.h"

class LEDs
{
public:
    enum LEDColor {LEDRed = 0, LEDGreen};
    enum BlinkType {BlinkOff = 0, BlinkNone, Blink2Short, Blink50DC, Blink2ShortStop};

    LEDs();
    ~LEDs();

    void setPattern(LEDColor color, BlinkType blinkType);
    void turnOn(LEDColor color);
    void turnOff(LEDColor color);
    void run(LEDColor color);

private:
    static const byte c_LEDCount = 2;

    static bool s_Initialized;

    static BlinkType s_BlinkType[c_LEDCount];
    static unsigned long s_BlinkTime[c_LEDCount][4];
    static bool s_State[c_LEDCount];
    static byte s_BlinkStage[c_LEDCount];

    static const unsigned int c_Blink2ShortStopOnTime1 = 10;
    static const unsigned int c_Blink2ShortStopOffTime1 = 300;
    static const unsigned int c_Blink2ShortStopOnTime2 = 10;
    static const unsigned int c_Blink2ShortStopOffTime2 = 480;

    static const unsigned int c_Blink2ShortOnTime1 = 100;
    static const unsigned int c_Blink2ShortOffTime1 = 100;
    static const unsigned int c_Blink2ShortOnTime2 = 100;
    static const unsigned int c_Blink2ShortOffTime2 = 700;

    static const unsigned int c_Blink50DCOnTime1 = 300;
    static const unsigned int c_Blink50DCOffTime1 = 300;

    static const byte c_nLEDPin[c_LEDCount];

};

#endif
