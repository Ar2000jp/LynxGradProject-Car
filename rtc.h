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

#ifndef RTC_H
#define RTC_H

#include "DS1307.h"

#include "globals.h"

class RTC
{
public:
    RTC();
    ~RTC();

    void init();

    void setSQEnabled(bool state);

    void setTime(unsigned long time);
    void setSeconds(byte seconds);
    void setMinutes(byte minutes);
    void setHours12(byte hours12, bool pm);
    void setDayOfWeek(byte dayOfWeek);
    void setDay(byte day);
    void setMonth(byte month);
    void setYear(unsigned int year);
    void setPM(bool pm);

    unsigned long getTime();
    byte getSeconds();
    byte getMinutes();
    byte getHours12();
    byte getDayOfWeek();
    byte getDay();
    byte getMonth();
    unsigned int getYear();
    bool getPM();

private:
    static bool s_Initialized;
    static DS1307 s_RTC;

};

#endif // RTC_H
