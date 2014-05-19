#include "doorcontroller.h"

bool DoorController::s_Initialized = false;
bool DoorController::s_AutoCloseEnabled = false;
bool DoorController::s_Open = false;
unsigned long DoorController::s_PrevUpdateTime = 0;

DoorController::DoorController()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        pinMode(c_DoorLockPin, OUTPUT);
        close();
    }
}

DoorController::~DoorController()
{

}

void DoorController::open()
{
    s_Open = true;
    digitalWrite(c_DoorLockPin, LOW);
    s_PrevUpdateTime = millis();
}

void DoorController::close()
{
    s_Open = false;
    digitalWrite(c_DoorLockPin, HIGH);
}

void DoorController::enableAutoClose()
{
    s_AutoCloseEnabled = true;
    s_PrevUpdateTime = millis();
}

void DoorController::disableAutoClose()
{
    s_AutoCloseEnabled = false;
}

void DoorController::update()
{
    if ((s_Open == true) && (s_AutoCloseEnabled == true) && (millis() >= (s_PrevUpdateTime + c_AutoCloseDelay))) {
        close();
    }
}

bool DoorController::getOpen()
{
    return s_Open;
}
