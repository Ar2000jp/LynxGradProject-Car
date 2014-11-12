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

#ifndef BMP180_H
#define BMP180_H

#include <SFE_BMP180.h>
#include "globals.h"

class BMP180
{
public:
    BMP180();
    ~BMP180();

    void init();
    void run();
    double getTemperature();
    double getPressure();

private:
    static bool s_Initialized;
    static double s_Temp;
    static double s_Pressure;

    static SFE_BMP180 s_BMP180;

};

#endif // BMP180_H
