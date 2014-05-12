#include "Arduino.h"
#include "myserial.h"
#include "globals.h"

void drainSerial()
{
    char dummyBuf[8];
    while (Serial.available() > 0) {
        Serial.readBytes(dummyBuf, 8);
    }
}

void drainSerial1()
{
    char dummyBuf[8];
    while (Serial1.available() > 0) {
        Serial1.readBytes(dummyBuf, 8);
    }
}
