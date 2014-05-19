#ifndef ALARM_H
#define ALARM_H

#include "leds.h"
#include "buzzer.h"
#include "globals.h"

class Alarm
{
public:
    enum AlarmLevel {AlarmOff = 0, AlarmRadioSignal, AlarmAttention, AlarmWarning, AlarmDanger, AlarmFatal};

    Alarm();
    ~Alarm();

    void setLevel(AlarmLevel level);
    AlarmLevel getLevel();
    void raiseLevel(AlarmLevel level);
    void lowerLevel(AlarmLevel level);
    void enableAutoSilence();
    void disableAutoSilence();
    void update();

private:
    static AlarmLevel s_AlarmLevel;
    static unsigned long s_LastAlarmTime;
    static const unsigned long c_AutoSilenceDelay = 10000;
    static bool s_AutoSilence;
    Buzzer m_Buzzer;
    LEDs m_LEDs;
};

#endif
