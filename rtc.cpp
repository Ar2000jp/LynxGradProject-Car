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

#include "rtc.h"

#include "Wire.h"
#include "I2Cdev.h"
#include "DS1307.h"

bool RTC::s_Initialized = false;
DS1307 RTC::s_RTC;

RTC::RTC()
{

}

RTC::~RTC()
{

}

void RTC::init()
{
    if (s_Initialized == false) {
        s_Initialized = true;

        s_RTC.initialize();

        // verify connection
        if (s_RTC.testConnection() == false) {
            Serial.println("DS1307 connection failed");
            while (1);
        }

        s_RTC.setSquareWaveRate(1); // 4kHz

        if (s_RTC.getClockRunning() != true) {
            s_RTC.setClockRunning(true);
        }

        // Switch to 12 Hour mode
        if (s_RTC.getMode() != 1) {
            s_RTC.setMode(1);
        }

        // Hardware bug workaround.
        byte seconds;
        for (int i = 0; i < 4; i++) {
            seconds = s_RTC.getSeconds();
            delay(300);
            if (seconds != s_RTC.getSeconds()) {
                // Clock is running
                return;
            }
            delay(400);
            if (seconds != s_RTC.getSeconds()) {
                // Clock is running
                return;
            }
            delay(400);
            if (seconds != s_RTC.getSeconds()) {
                // Clock is running
                return;
            }

            // Toggle clock
            s_RTC.setClockRunning(false);
            delay(10);
            s_RTC.setClockRunning(true);
        }
#ifdef DEBUG
        Serial.println("Unable to start RTC clock.");
#endif
    }
}

void RTC::setSQEnabled(bool state)
{
    if (s_Initialized == false) {
        return;
    }
    s_RTC.setSquareWaveRate(1); // 4kHz
    s_RTC.setSquareWaveEnabled(state);
}

unsigned long RTC::getTime()
{
    if (s_Initialized == false) {
        return 0;
    }
    return s_RTC.getSeconds();
}

void RTC::setTime(unsigned long time)
{
    if (s_Initialized == false) {
        return;
    }
    s_RTC.setSeconds(time);
}

unsigned int RTC::getYear()
{
    return s_RTC.getYear();
}

byte RTC::getMonth()
{
    return s_RTC.getMonth();
}

byte RTC::getDay()
{
    return s_RTC.getDay();
}

byte RTC::getDayOfWeek()
{
    s_RTC.getDayOfWeek();
}

bool RTC::getPM()
{
    return s_RTC.getAMPM();
}

byte RTC::getHours12()
{
    return s_RTC.getHours12();
}

byte RTC::getMinutes()
{
    return s_RTC.getMinutes();
}

byte RTC::getSeconds()
{
    return s_RTC.getSeconds();
}

void RTC::setDayOfWeek(byte dayOfWeek)
{
    s_RTC.setDayOfWeek(dayOfWeek);
}

void RTC::setSeconds(byte seconds)
{
    s_RTC.setSeconds(seconds);
}

void RTC::setMinutes(byte minutes)
{
    s_RTC.setMinutes(minutes);
}

void RTC::setHours12(byte hours12, bool pm)
{
    s_RTC.setHours12(hours12, pm);
}

void RTC::setDay(byte day)
{
    s_RTC.setDay(day);
}

void RTC::setMonth(byte month)
{
    s_RTC.setMonth(month);
}

void RTC::setYear(unsigned int year)
{
    s_RTC.setYear(year);
}

void RTC::setPM(bool pm)
{
    s_RTC.setAMPM(pm);
}
