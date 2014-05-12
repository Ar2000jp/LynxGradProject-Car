#ifndef BMP180_H
#define BMP180_H

#include <SFE_BMP180.h>
#include "globals.h"

class BMP180
{
public:
    BMP180();
    ~BMP180();

    void init();
    void run();
    double getTemperature();
    double getPressure();
    double getAltitude();

private:
    static bool s_Initialized;
    static double s_Temp;
    static double s_Pressure;

    static SFE_BMP180 s_BMP180;

};

#endif // BMP180_H
