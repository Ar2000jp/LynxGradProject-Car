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

#include "windowcontroller.h"

WindowController::WindowController(byte onPin, byte directionPin, unsigned int cycleTime)
{
    m_OnPin = onPin;
    m_DirectionPin = directionPin;
    m_CycleTime = cycleTime;

    pinMode(m_OnPin, OUTPUT);
    pinMode(m_DirectionPin, OUTPUT);
    digitalWrite(m_OnPin, LOW);
    digitalWrite(m_DirectionPin, LOW);
}

WindowController::~WindowController()
{

}

void WindowController::open()
{
    if (m_WindowOpen == true) {
        return;
    }
    m_WindowOpen = true;

    m_PrevTime = millis();
    m_Acting = true;

    digitalWrite(m_DirectionPin, HIGH);
    delay(5);
    digitalWrite(m_OnPin, HIGH);

}

void WindowController::close()
{
    if (m_WindowOpen == false) {
        return;
    }
    m_WindowOpen = false;

    m_PrevTime = millis();
    m_Acting = true;

    digitalWrite(m_DirectionPin, LOW);
    delay(5);
    digitalWrite(m_OnPin, HIGH);

}

void WindowController::update()
{
    if (m_Acting == true && (millis() >= (m_PrevTime + m_CycleTime))) {
        m_Acting = false;

        digitalWrite(m_OnPin, LOW);
        delay(5);
        digitalWrite(m_DirectionPin, LOW);
    }
}

bool WindowController::getOpen()
{
    return m_WindowOpen;
}
