#include "alarm.h"

Alarm::AlarmLevel Alarm::s_AlarmLevel = Alarm::AlarmOff;

Alarm::Alarm()
{

}

Alarm::~Alarm()
{

}

void Alarm::setLevel(Alarm::AlarmLevel level)
{
    s_AlarmLevel = level;
    switch (level) {
    case 0:
        m_LEDs.turnOff(LEDs::LEDRed);
        m_Buzzer.turnOff();
        break;

    case 1:
        m_LEDs.setPattern(m_LEDs.LEDs::LEDRed, LEDs::Blink2Short);
        m_Buzzer.turnOff();
        break;

    case 2:
        m_LEDs.setPattern(m_LEDs.LEDs::LEDRed, LEDs::Blink50DC);
        m_Buzzer.turnOff();
        break;

    case 3:
        m_LEDs.setPattern(m_LEDs.LEDs::LEDRed, LEDs::Blink50DC);
        m_Buzzer.setTone(Buzzer::Tone2Short);
        break;

    case 4:
        m_LEDs.setPattern(m_LEDs.LEDs::LEDRed, LEDs::Blink50DC);
        m_Buzzer.setTone(Buzzer::Tone50DC);
        break;

    case 5:
        m_LEDs.setPattern(m_LEDs.LEDs::LEDRed, LEDs::Blink50DC);
        m_Buzzer.setTone(Buzzer::ToneConstant);
        break;

    default:
#ifdef DEBUG
        Serial.println("Invalid alarmLevel.");
#endif
        break;
    }
}

Alarm::AlarmLevel Alarm::getLevel()
{
    return s_AlarmLevel;
}

void Alarm::raiseLevel(Alarm::AlarmLevel level)
{
    if (getLevel() < level) {
        setLevel(level);
    }
}

void Alarm::lowerLevel(Alarm::AlarmLevel level)
{
    if (getLevel() > level) {
        setLevel(level);
    }
}
