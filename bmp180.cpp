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

#include "bmp180.h"

#include <ChibiOS_AVR.h>

bool BMP180::s_Initialized;
double BMP180::s_Temp;
double BMP180::s_Pressure;

SFE_BMP180 BMP180::s_BMP180;

BMP180::BMP180()
{

}

BMP180::~BMP180()
{

}

void BMP180::init()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        if (!s_BMP180.begin()) {
            Serial.println("BMP180 init failed");
            while (1); // Pause forever.
        }
    }
}

void BMP180::run()
{
    byte waitTime = 0;

    while (1) {
        if (s_Initialized == false) {
            chThdSleepMilliseconds(2000);
            continue;
        }

        chThdSleepMilliseconds(2000);
        waitTime = s_BMP180.startTemperature();
        if (waitTime == 0) {
#ifdef DEBUG
            Serial.println("BMP180 start temp. error");
#endif
            continue;
        }
        chThdSleepMilliseconds(waitTime + 10);

        if (s_BMP180.getTemperature(s_Temp) == 0) {
#ifdef DEBUG
            Serial.println("BMP180 temp. error");
#endif
            continue;
        }

        chThdSleepMilliseconds(400);
        waitTime = s_BMP180.startPressure(3);
        if (waitTime == 0) {
#ifdef DEBUG
            Serial.println("BMP180 start pressure error");
#endif
            continue;
        }
        chThdSleepMilliseconds(waitTime + 10);

        if (s_BMP180.getPressure(s_Pressure, s_Temp) == 0) {
#ifdef DEBUG
            Serial.println("BMP180 pressure error");
#endif
            continue;
        }
    }
}

double BMP180::getPressure()
{
    return s_Pressure;
}

double BMP180::getTemperature()
{
    return s_Temp;
}
