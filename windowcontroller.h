#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <Arduino.h>

class WindowController
{
public:
    WindowController(byte onPin, byte directionPin, unsigned int cycleTime);
    ~WindowController();

    void open();
    void close();
    bool getOpen();
    void update();

private:

    byte m_OnPin;
    byte m_DirectionPin;
    bool m_Acting;
    unsigned int m_CycleTime;
    unsigned long m_PrevTime;
    bool m_WindowOpen;

};

#endif // WINDOWCONTROLLER_H
