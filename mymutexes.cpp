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

#include "mymutexes.h"

#include <Arduino.h>
#include <ChibiOS_AVR.h>

// Mutexes
MUTEX_DECL(G_SPIMutex);

void lockSPI()
{
    chMtxLock(&G_SPIMutex);
}

void unlockSPI()
{
    chMtxUnlock();
}
