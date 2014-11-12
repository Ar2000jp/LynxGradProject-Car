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

#include "Arduino.h"
#include "myserial.h"
#include "globals.h"

void drainSerial()
{
    char dummyBuf[8];
    while (Serial.available() > 0) {
        Serial.readBytes(dummyBuf, 8);
    }
}

void drainSerial1()
{
    char dummyBuf[8];
    while (Serial1.available() > 0) {
        Serial1.readBytes(dummyBuf, 8);
    }
}
