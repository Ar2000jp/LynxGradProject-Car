#include "bmp180.h"

#include <ChibiOS_AVR.h>

bool BMP180::s_Initialized;
double BMP180::s_Temp;
double BMP180::s_Pressure;

SFE_BMP180 BMP180::s_BMP180;

BMP180::BMP180()
{

}

BMP180::~BMP180()
{

}

void BMP180::init()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        if (!s_BMP180.begin()) {
            Serial.println("BMP180 init failed");
            while (1); // Pause forever.
        }
    }
}

void BMP180::run()
{
    byte waitTime = 0;

    while (1) {
        if (s_Initialized == false) {
            chThdSleepMilliseconds(400);
            continue;
        }

        chThdSleepMilliseconds(400);
        waitTime = s_BMP180.startTemperature();
        if (waitTime == 0) {
#ifdef DEBUG
            Serial.println("BMP180 start temp. error");
#endif
            continue;
        }
        chThdSleepMilliseconds(waitTime + 10);

        if (s_BMP180.getTemperature(s_Temp) == 0) {
#ifdef DEBUG
            Serial.println("BMP180 temp. error");
#endif
            continue;
        }

        chThdSleepMilliseconds(400);
        waitTime = s_BMP180.startPressure(3);
        if (waitTime == 0) {
#ifdef DEBUG
            Serial.println("BMP180 start pressure error");
#endif
            continue;
        }
        chThdSleepMilliseconds(waitTime + 10);

        if (s_BMP180.getPressure(s_Pressure, s_Temp) == 0) {
#ifdef DEBUG
            Serial.println("BMP180 pressure error");
#endif
            continue;
        }
    }
}

double BMP180::getPressure()
{
    return s_Pressure;
}

double BMP180::getTemperature()
{
    return s_Temp;
}

double BMP180::getAltitude()
{
// TODO: Calculate altitude from Temp. & Absolute pressure
    return 0.0;
}
