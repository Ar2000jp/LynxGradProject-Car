#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"

#include "rtc.h"
#include "globals.h"

class Buzzer
{
public:
    enum BuzzerTone {ToneOff = 0, ToneConstant, Tone2Short, Tone50DC, ToneSalute};

    Buzzer();
    ~Buzzer();

    void setTone(BuzzerTone tone);
    void salute();
    void turnOn();
    void turnOff();
    void run();

private:
    static bool s_Initialized;

    static BuzzerTone s_BuzzerTone;
    static unsigned long s_ToneTime[4];
    static bool s_State;
    static byte s_ToneStage;
    static RTC s_RTC;

    static const unsigned int c_ToneSaluteOnTime1 = 10;
    static const unsigned int c_ToneSaluteOffTime1 = 300;
    static const unsigned int c_ToneSaluteOnTime2 = 10;
    static const unsigned int c_ToneSaluteOffTime2 = 680;

    static const unsigned int c_Tone2ShortOnTime1 = 100;
    static const unsigned int c_Tone2ShortOffTime1 = 100;
    static const unsigned int c_Tone2ShortOnTime2 = 100;
    static const unsigned int c_Tone2ShortOffTime2 = 700;

    static const unsigned int c_Tone50DCOnTime1 = 300;
    static const unsigned int c_Tone50DCOffTime1 = 300;

};

#endif
