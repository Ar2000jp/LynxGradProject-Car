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

#include "doorcontroller.h"

bool DoorController::s_Initialized = false;
bool DoorController::s_AutoCloseEnabled = false;
bool DoorController::s_Open = false;
unsigned long DoorController::s_PrevUpdateTime = 0;

DoorController::DoorController()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        pinMode(c_DoorLockPin, OUTPUT);
        close();
    }
}

DoorController::~DoorController()
{

}

void DoorController::open()
{
    s_Open = true;
    digitalWrite(c_DoorLockPin, LOW);
    s_PrevUpdateTime = millis();
}

void DoorController::close()
{
    s_Open = false;
    digitalWrite(c_DoorLockPin, HIGH);
}

void DoorController::enableAutoClose()
{
    s_AutoCloseEnabled = true;
    s_PrevUpdateTime = millis();
}

void DoorController::disableAutoClose()
{
    s_AutoCloseEnabled = false;
}

void DoorController::update()
{
    if ((s_Open == true) && (s_AutoCloseEnabled == true) && (millis() >= (s_PrevUpdateTime + c_AutoCloseDelay))) {
        close();
    }
}

bool DoorController::getOpen()
{
    return s_Open;
}
