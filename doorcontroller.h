#ifndef DOORCONTROLLER_H
#define DOORCONTROLLER_H

#include <Arduino.h>

class DoorController
{
public:
    DoorController();
    ~DoorController();

    void enableAutoClose();
    void disableAutoClose();
    void open();
    void close();
    bool getOpen();
    void update();

private:
    static bool s_Initialized;
    static bool s_AutoCloseEnabled;
    static bool s_Open;
    static unsigned long s_PrevUpdateTime;
    static const unsigned int c_AutoCloseDelay = 10000;
    static const byte c_DoorLockPin = 8;

};

#endif // DOORCONTROLLER_H
