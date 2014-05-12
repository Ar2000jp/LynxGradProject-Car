#include "rtc.h"

#include "Wire.h"
#include "I2Cdev.h"
#include "DS1307.h"

bool RTC::s_Initialized = false;
DS1307 RTC::s_RTC;

RTC::RTC()
{

}

RTC::~RTC()
{

}

void RTC::init()
{
    if (s_Initialized == false) {
        s_Initialized = true;

        s_RTC.initialize();

        // verify connection
        if (s_RTC.testConnection() == false) {
            Serial.println("DS1307 connection failed");
            //while (1);
        }

        s_RTC.setSquareWaveRate(1); // 4kHz

        if (s_RTC.getClockRunning() != true) {
            s_RTC.setClockRunning(true);
        }

        // Hardware bug workaround.
        byte seconds;
        for (int i = 0; i < 4; i++) {
            seconds = s_RTC.getSeconds();
            delay(300);
            if (seconds != s_RTC.getSeconds()) {
                // Clock is running
                return;
            }
            delay(400);
            if (seconds != s_RTC.getSeconds()) {
                // Clock is running
                return;
            }
            delay(400);
            if (seconds != s_RTC.getSeconds()) {
                // Clock is running
                return;
            }

            // Toggle clock
            s_RTC.setClockRunning(false);
            delay(10);
            s_RTC.setClockRunning(true);
        }

        Serial.println("Unable to start RTC clock.");
    }
}

void RTC::setSQEnabled(bool state)
{
    if (s_Initialized == false) {
        return;
    }
    s_RTC.setSquareWaveRate(1); // 4kHz
    s_RTC.setSquareWaveEnabled(state);
}

long unsigned int RTC::getTime()
{
    if (s_Initialized == false) {
        return 0;
    }
    return s_RTC.getSeconds();
}

void RTC::setTime(long unsigned int time)
{
    if (s_Initialized == false) {
        return;
    }
    s_RTC.setSeconds(time);
}

unsigned int RTC::getYear()
{
    return s_RTC.getYear();
}

unsigned int RTC::getMonth()
{
    return s_RTC.getMonth();
}

unsigned int RTC::getDay()
{
    return s_RTC.getDay();
}

unsigned int RTC::getDayOfWeek()
{
    s_RTC.getDayOfWeek();
}

bool RTC::getPM()
{
    return s_RTC.getAMPM();
}

unsigned int RTC::getHours12()
{
    return s_RTC.getHours12();
}

unsigned int RTC::getMinutes()
{
    return s_RTC.getMinutes();
}

unsigned int RTC::getSeconds()
{
    return s_RTC.getSeconds();
}
