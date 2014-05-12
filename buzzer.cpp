#include "buzzer.h"
#include <ChibiOS_AVR.h>

bool Buzzer::s_Initialized = false;

Buzzer::BuzzerTone Buzzer::s_BuzzerTone = Buzzer::ToneConstant;
unsigned long Buzzer::s_ToneTime[4] = {0, 0, 0, 0};
byte Buzzer::s_ToneStage = 0;
bool Buzzer::s_State = false;

RTC Buzzer::s_RTC;

Buzzer::Buzzer()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        setTone(ToneOff);
    }
}

Buzzer::~Buzzer()
{

}

void Buzzer::setTone(Buzzer::BuzzerTone tone)
{
    if (tone == ToneOff) {
        s_State = false;
    } else if (tone == ToneConstant) {
        s_State = true;
    } else if (tone == Tone2Short) {
        s_ToneTime[0] = c_Tone2ShortOnTime1;
        s_ToneTime[1] = c_Tone2ShortOffTime1;
        s_ToneTime[2] = c_Tone2ShortOnTime2;
        s_ToneTime[3] = c_Tone2ShortOffTime2;
        s_State = true;
    } else if (tone == Tone50DC) {
        s_ToneTime[0] = c_Tone50DCOnTime1;
        s_ToneTime[1] = c_Tone50DCOffTime1;
        s_ToneTime[2] = c_Tone50DCOnTime1;
        s_ToneTime[3] = c_Tone50DCOffTime1;
        s_State = true;
    } else if (tone == ToneSalute) {
        s_ToneTime[0] = c_ToneSaluteOnTime1;
        s_ToneTime[1] = c_ToneSaluteOffTime1;
        s_ToneTime[2] = c_ToneSaluteOnTime2;
        s_ToneTime[3] = c_ToneSaluteOffTime2;
        s_State = true;
    } else {
        return;
    }

    s_BuzzerTone = tone;
    s_ToneStage = 0;

    s_RTC.setSQEnabled(s_State);
}

void Buzzer::salute()
{
    setTone(ToneSalute);
}

void Buzzer::turnOn()
{
    setTone(ToneConstant);
}

void Buzzer::turnOff()
{
    setTone(ToneOff);
}

void Buzzer::run()
{
    while (1) {
        if (s_BuzzerTone == ToneConstant) {
            s_State = true;
            s_RTC.setSQEnabled(s_State);
            chThdSleepMilliseconds(50);
            continue;
        }
        if (s_BuzzerTone == ToneOff) {
            s_State = false;
            s_RTC.setSQEnabled(s_State);
            chThdSleepMilliseconds(50);
            continue;
        }

        chThdSleepMilliseconds(s_ToneTime[s_ToneStage] + 1);
        s_State = !s_State;
        s_ToneStage++;

        if (s_ToneStage > 3) {
            if (s_BuzzerTone == ToneSalute) {
                setTone(ToneOff);
            }
            s_ToneStage = 0;
        }

        s_RTC.setSQEnabled(s_State);
    }
}
