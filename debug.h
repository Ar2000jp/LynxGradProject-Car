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

#ifndef DEBUG_H
#define DEBUG_H

#include "leds.h"
#include "buzzer.h"
#include "alarm.h"
#include "myserial.h"
#include "radio.h"
#include "globals.h"
#include "rtc.h"

class Debug
{
public:
    Debug();
    ~Debug();

    void start();
    void printHelp();

private:
    static bool s_DebugMode;

    static LEDs s_LEDs;
    static Buzzer s_Buzzer;
    static Alarm s_Alarm;
    static Radio s_Radio;
    static RTC s_RTC;

};

#endif
