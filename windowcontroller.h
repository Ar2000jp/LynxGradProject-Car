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

#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <Arduino.h>

class WindowController
{
public:
    WindowController(byte onPin, byte directionPin, unsigned int cycleTime);
    ~WindowController();

    void open();
    void close();
    bool getOpen();
    void update();

private:

    byte m_OnPin;
    byte m_DirectionPin;
    bool m_Acting;
    unsigned int m_CycleTime;
    unsigned long m_PrevTime;
    bool m_WindowOpen;

};

#endif // WINDOWCONTROLLER_H
