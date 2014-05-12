#include "leds.h"
#include <ChibiOS_AVR.h>

bool LEDs::s_Initialized = false;

LEDs::BlinkType LEDs::s_BlinkType[c_LEDCount] = {BlinkOff, BlinkOff};
unsigned long LEDs::s_BlinkTime[c_LEDCount][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
bool LEDs::s_State[c_LEDCount] = {false, false};
byte LEDs::s_BlinkStage[c_LEDCount] = {0, 0};

const byte LEDs::c_nLEDPin[c_LEDCount] = {11, 12};

LEDs::LEDs()
{
    if (s_Initialized == false) {
        s_Initialized = true;

        for (byte index = 0; index < c_LEDCount; index++) {
            pinMode(c_nLEDPin[index], OUTPUT);
            turnOff((LEDColor)index);
        }
    }
}

LEDs::~LEDs()
{

}

void LEDs::setPattern(LEDs::LEDColor color, LEDs::BlinkType blinkType)
{
    if (blinkType == BlinkOff) {
        s_State[color] = false;
    } else if (blinkType == BlinkNone) {
        s_State[color] = true;
    } else if (blinkType == Blink2Short) {
        s_BlinkTime[color][0] = c_Blink2ShortOnTime1;
        s_BlinkTime[color][1] = c_Blink2ShortOffTime1;
        s_BlinkTime[color][2] = c_Blink2ShortOnTime2;
        s_BlinkTime[color][3] = c_Blink2ShortOffTime2;
        s_State[color] = true;
    } else if (blinkType == Blink50DC) {
        s_BlinkTime[color][0] = c_Blink50DCOnTime1;
        s_BlinkTime[color][1] = c_Blink50DCOffTime1;
        s_BlinkTime[color][2] = c_Blink50DCOnTime1;
        s_BlinkTime[color][3] = c_Blink50DCOffTime1;
        s_State[color] = true;
    } else if (blinkType == Blink2ShortStop) {
        s_BlinkTime[color][0] = c_Blink2ShortStopOnTime1;
        s_BlinkTime[color][1] = c_Blink2ShortStopOffTime1;
        s_BlinkTime[color][2] = c_Blink2ShortStopOnTime2;
        s_BlinkTime[color][3] = c_Blink2ShortStopOffTime2;
        s_State[color] = true;
    } else {
        return;
    }

    s_BlinkType[color] = blinkType;
    s_BlinkStage[color] = 0;

    digitalWrite(c_nLEDPin[color], !s_State[color]);
}

void LEDs::turnOn(LEDs::LEDColor color)
{
    setPattern(color, BlinkNone);
}

void LEDs::turnOff(LEDs::LEDColor color)
{
    setPattern(color, BlinkOff);
}

void LEDs::run(LEDs::LEDColor color)
{
    while (1) {
        if (s_BlinkType[color] == BlinkNone) {
            s_State[color] = true;
            digitalWrite(c_nLEDPin[color], !s_State[color]);
            chThdSleepMilliseconds(50);
            continue;
        }
        if (s_BlinkType[color] == BlinkOff) {
            s_State[color] = false;
            digitalWrite(c_nLEDPin[color], !s_State[color]);
            chThdSleepMilliseconds(50);
            continue;
        }

        chThdSleepMilliseconds(s_BlinkTime[color][s_BlinkStage[color]] + 1);
        s_State[color] = !s_State[color];
        s_BlinkStage[color]++;

        if (s_BlinkStage[color] > 3) {
            if (s_BlinkType[color] == Blink2ShortStop) {
                turnOff((LEDColor)color);
            }
            s_BlinkStage[color] = 0;
        }

        digitalWrite(c_nLEDPin[color], !s_State[color]);
    }
}
