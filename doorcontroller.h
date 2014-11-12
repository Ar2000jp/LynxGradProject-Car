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

#ifndef DOORCONTROLLER_H
#define DOORCONTROLLER_H

#include <Arduino.h>

class DoorController
{
public:
    DoorController();
    ~DoorController();

    void enableAutoClose();
    void disableAutoClose();
    void open();
    void close();
    bool getOpen();
    void update();

private:
    static bool s_Initialized;
    static bool s_AutoCloseEnabled;
    static bool s_Open;
    static unsigned long s_PrevUpdateTime;
    static const unsigned int c_AutoCloseDelay = 10000;
    static const byte c_DoorLockPin = 8;

};

#endif // DOORCONTROLLER_H
