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

#include "radio.h"

#include <Arduino.h>
#include <ChibiOS_AVR.h>

#include "globals.h"
#include "mymutexes.h"

RHReliableDatagram Radio::s_RadioManager(s_RadioDriver, c_CarAddress);
RHReliableDatagram Radio::s_SerialManager(s_SerialDriver, c_CarAddress);

RH_RF22 Radio::s_RadioDriver;
RH_Serial Radio::s_SerialDriver(Serial1);

bool Radio::s_Initialized = false;
volatile bool Radio::s_RadioInterrupt = false;
byte Radio::s_CommSysID = 0;

Radio::Radio()
{

}

Radio::~Radio()
{

}

void Radio::init(byte commSysID)
{
    if (s_Initialized == false) {
        s_Initialized = true;
        s_CommSysID = commSysID;

        if (s_CommSysID == 0) {
            if (s_RadioManager.init()) {
                s_RadioManager.setTimeout(100);
                s_RadioManager.setRetries(2);

                // Use a reasonable comm. speed
                s_RadioDriver.setModemConfig(RH_RF22::FSK_Rb9_6Fd45);

                // Use max Tx power
                s_RadioDriver.setTxPower(RH_RF22_TXPOW_20DBM);

                attachInterrupt(c_RadioInterruptNo, radioISR, FALLING); // Take over interrupts
            } else {
                Serial.println("Radio init failed. Switching to Serial1.");
            }
        } else {
            if (s_SerialManager.init()) {
                s_SerialManager.setTimeout(100);
                s_SerialManager.setRetries(5);
            } else {
                Serial.println("Serial1 init failed.");
            }
        }
    }
}

bool Radio::send(uint8_t* buf, uint8_t bufLen)
{
    if (s_CommSysID == 0) {
        if (!s_RadioManager.sendtoWait(buf, bufLen, c_RCAddress)) {
#ifdef DEBUG
            Serial.println("sendtoWait failed");
#endif
            return false;
        } else {
#ifdef DEBUG
            Serial.println("sendtoWait succeeded");
#endif
        }
    } else {
        if (!s_SerialManager.sendtoWait(buf, bufLen, c_RCAddress)) {
#ifdef DEBUG
            Serial.println("sendtoWait failed");
#endif
            return false;
        } else {
#ifdef DEBUG
            Serial.println("sendtoWait succeeded");
#endif
        }
    }

    return true;
}

bool Radio::recv(uint8_t* buf, uint8_t* bufLen)
{
    uint8_t from;

    if (s_CommSysID == 0) {
        if (s_RadioManager.available() < 1) {
            return false;
        }

        if (!s_RadioManager.recvfromAckTimeout(buf, bufLen, 300, &from)) {
            return false;
        }
    } else {
        if (s_SerialManager.available() < 1) {
            return false;
        }

        if (!s_SerialManager.recvfromAckTimeout(buf, bufLen, 300, &from)) {
            return false;
        }
    }

#ifdef DEBUG
    Serial.print("got msg from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);
#endif

    if (from != c_RCAddress) {
        return false;
    }

    return true;
}

void Radio::radioISR()
{
    s_RadioInterrupt = true;
}

void Radio::update()
{
    if (s_RadioInterrupt == false) {
        chThdSleepMilliseconds(1);

#ifdef DEBUG
        Serial.print("RIf");
#endif
    } else {
        lockSPI();
        s_RadioInterrupt = false;
        s_RadioDriver.handleInterrupt();
        unlockSPI();

#ifdef DEBUG
        Serial.println("RIt");
#endif
    }
}
