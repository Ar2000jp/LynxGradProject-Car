#ifndef DEBUG_H
#define DEBUG_H

#include "leds.h"
#include "buzzer.h"
#include "alarm.h"
#include "myserial.h"
#include "radio.h"
#include "globals.h"
#include "rtc.h"

class Debug
{
public:
    Debug();
    ~Debug();

    void start();
    void printHelp();

private:
    static bool s_DebugMode;

    static LEDs s_LEDs;
    static Buzzer s_Buzzer;
    static Alarm s_Alarm;
    static Radio s_Radio;
    static RTC s_RTC;

};

#endif
