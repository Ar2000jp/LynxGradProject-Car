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

#ifndef ALARM_H
#define ALARM_H

#include "leds.h"
#include "buzzer.h"
#include "globals.h"

class Alarm
{
public:
    enum AlarmLevel {AlarmOff = 0, AlarmRadioSignal, AlarmAttention, AlarmWarning, AlarmDanger, AlarmFatal};

    Alarm();
    ~Alarm();

    void setLevel(AlarmLevel level);
    AlarmLevel getLevel();
    void raiseLevel(AlarmLevel level);
    void lowerLevel(AlarmLevel level);
    void enableAutoSilence();
    void disableAutoSilence();
    void update();

private:
    static AlarmLevel s_AlarmLevel;
    static unsigned long s_LastAlarmTime;
    static const unsigned long c_AutoSilenceDelay = 10000;
    static bool s_AutoSilence;
    Buzzer m_Buzzer;
    LEDs m_LEDs;
};

#endif
