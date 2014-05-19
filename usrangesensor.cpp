#include "usrangesensor.h"

#include <ChibiOS_AVR.h>

bool USRangeSensor::s_Initialized = false;
byte USRangeSensor::s_Distance = 0;
unsigned long USRangeSensor::s_Duration = 0;
unsigned long USRangeSensor::s_PrevUpdateTime = 0;

USRangeSensor::USRangeSensor()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        pinMode(c_TrigPin, OUTPUT);
        pinMode(c_EchoPin, INPUT);
    }
}

USRangeSensor::~USRangeSensor()
{

}

byte USRangeSensor::getDistance()
{
    return s_Distance;
}

void USRangeSensor::update()
{
    if (millis() < (s_PrevUpdateTime + c_UpdateInterval)) {
        return;
    }

    s_PrevUpdateTime = millis();

    chThdSetPriority(HIGHPRIO);

    digitalWrite(c_TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(c_TrigPin, LOW);
    s_Duration = pulseIn(c_EchoPin, HIGH, 20000); // Timeout after 15 mS. More than enough for 150cm

    chThdSetPriority(NORMALPRIO);

    s_Distance = s_Duration / 58;

    if ((s_Distance <= 10) || (s_Distance >= 150)) {
        // Out of Range
        s_Distance = 255;
    }
}
