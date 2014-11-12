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

#include "usrangesensor.h"

#include <ChibiOS_AVR.h>

bool USRangeSensor::s_Initialized = false;
byte USRangeSensor::s_Distance = 0;
unsigned long USRangeSensor::s_Duration = 0;
unsigned long USRangeSensor::s_PrevUpdateTime = 0;

USRangeSensor::USRangeSensor()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        pinMode(c_TrigPin, OUTPUT);
        pinMode(c_EchoPin, INPUT);
    }
}

USRangeSensor::~USRangeSensor()
{

}

byte USRangeSensor::getDistance()
{
    return s_Distance;
}

void USRangeSensor::update()
{
    if (millis() < (s_PrevUpdateTime + c_UpdateInterval)) {
        return;
    }

    s_PrevUpdateTime = millis();

    chThdSetPriority(HIGHPRIO);

    digitalWrite(c_TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(c_TrigPin, LOW);
    s_Duration = pulseIn(c_EchoPin, HIGH, 20000); // Timeout after 15 mS. More than enough for 150cm

    chThdSetPriority(NORMALPRIO);

    s_Distance = s_Duration / 58;

    if ((s_Distance <= 10) || (s_Distance >= 150)) {
        // Out of Range
        s_Distance = 255;
    }
}
