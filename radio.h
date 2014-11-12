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

#ifndef RADIO_H
#define RADIO_H

#include <SPI.h>
#include <RH_RF22.h>
#include <RH_Serial.h>
#include <RHReliableDatagram.h>

class Radio
{
public:
    Radio();
    ~Radio();

    void init(byte commSysID);
    bool send(uint8_t* buf, uint8_t bufLen);
    bool recv(uint8_t* buf, uint8_t* bufLen);
    void run();
    void update();

private:
    static void radioISR();

    static bool s_Initialized;
    volatile static bool s_RadioInterrupt;
    static byte s_CommSysID;

    static const uint8_t c_RCAddress = 1;
    static const uint8_t c_CarAddress = 2;

    static const byte c_RadioInterruptNo = 0;

    static RH_RF22 s_RadioDriver;
    static RH_Serial s_SerialDriver;
    static RHReliableDatagram s_RadioManager;
    static RHReliableDatagram s_SerialManager;

};

#endif
