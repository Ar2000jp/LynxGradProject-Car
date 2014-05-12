#ifndef RTC_H
#define RTC_H

#include "DS1307.h"

#include "globals.h"

class RTC
{
public:
    RTC();
    ~RTC();

    void init();

    void setSQEnabled(bool state);

    unsigned long getTime();
    void setTime(unsigned long time);

    unsigned int getSeconds();
    unsigned int getMinutes();
    unsigned int getHours12();
    unsigned int getDayOfWeek();
    unsigned int getDay();
    unsigned int getMonth();
    unsigned int getYear();
    bool getPM();

private:
    static bool s_Initialized;
    static DS1307 s_RTC;

};

#endif // RTC_H
