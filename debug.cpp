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

#include "debug.h"
#include "globals.h"

#include <ChibiOS_AVR.h>

bool Debug::s_DebugMode = false;

LEDs Debug::s_LEDs;
Buzzer Debug::s_Buzzer;
Alarm Debug::s_Alarm;
Radio Debug::s_Radio;
RTC Debug::s_RTC;

Debug::Debug()
{

}

Debug::~Debug()
{

}

void Debug::start()
{
    byte param = 0;
    s_DebugMode = true;

    // Print help
    printHelp();

    while (s_DebugMode == true) {
        param = 0;

        chThdSleepMilliseconds(100);

        // Process data from Serial
        if (Serial.available() < 1) {
            chThdSleepMilliseconds(100);
            continue;
        }

        byte inChar = Serial.read();
        switch (inChar) {
        case 'h':
            printHelp();
            break;

        case 'a':
            param = Serial.parseInt();
            s_Alarm.setLevel((Alarm::AlarmLevel)param);
            Serial.print("Setting Alarm to level ");
            Serial.println(param);
            break;

        case 'b':
            param = Serial.parseInt();
            s_Buzzer.setTone((Buzzer::BuzzerTone)param);
            Serial.print("Setting Buzzer to tone ");
            Serial.println(param);
            break;

        case 'r':
            param = Serial.parseInt();
            s_LEDs.setPattern(LEDs::LEDRed, (LEDs::BlinkType)param);
            Serial.println("Setting Red LED to pattern ");
            Serial.println(param);
            break;

        case 'g':
            param = Serial.parseInt();
            s_LEDs.setPattern(LEDs::LEDGreen, (LEDs::BlinkType)param);
            Serial.println("Setting Green LED to pattern ");
            Serial.println(param);
            break;

        case 't':
            Serial.println("Setting time.");
            s_RTC.setHours12(Serial.parseInt(), 0);
            s_RTC.setMinutes(Serial.parseInt());
            s_RTC.setSeconds(Serial.parseInt());
            s_RTC.setPM(Serial.parseInt());
            break;

        case 'd':
            Serial.println("Setting date.");
            s_RTC.setYear(Serial.parseInt());
            s_RTC.setMonth(Serial.parseInt());
            s_RTC.setDay(Serial.parseInt());
            break;

        case 'w':
            param = Serial.parseInt();
            Serial.println("Setting day of week.");
            s_RTC.setDayOfWeek(param);
            break;

        case 'q':
            Serial.println("Exiting debug mode.");
            s_DebugMode = false;
            break;

        default:
            Serial.println("Invalid command");
        }

        chThdSleepMilliseconds(100);

        // Drain buffer before looping.
        drainSerial();
    }
}

void Debug::printHelp()
{
    Serial.println("Debug mode:");
    Serial.println("Command\tAction");
    Serial.println("h\tPrint help");
    Serial.println("a#\tSet Alarm to level #");
    Serial.println("b#\tBuzz buzzer using level #");
    Serial.println("g#\tBlink Green LED using level #");
    Serial.println("r#\tBlink Red LED using level #");
    Serial.println("t# # # #\tSet time (Hours12 Minutes Seconds AMPM(PM = 1))");
    Serial.println("d# # #\tSet date (Year Month Day)");
    Serial.println("w#\tSet day of week");
    Serial.println("q\tExit debug mode");
}
