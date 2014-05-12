/*
 Team Lynx Graduation Project.
 Car system code.
 */

#include <stdint.h>

#include <SPI.h>
#include <RH_RF22.h>
#include <RHReliableDatagram.h>
#include <ChibiOS_AVR.h>

#include <Wire.h>
#include <I2Cdev.h>
#include <DS1307.h>
#include <SFE_BMP180.h>
#include <MultiLCD.h>
#include <PString.h>

#include "globals.h"
#include "alarm.h"
#include "myserial.h"
#include "radio.h"
#include "debug.h"
#include "buzzer.h"
#include "leds.h"
#include "bmp180.h"
#include "digitalsensor.h"
#include "usrangesensor.h"
#include "mymutexes.h"
#include "voltmeter.h"

// const byte c_I2CSCLPin = 21;
// const byte c_I2CSDAPin = 20;

const byte c_MQ7COPin = 15;
const byte c_MQ2GasPin = 16;
const byte c_RainPin = 14;
const byte c_PIRPin = 13;//FIXME

unsigned int G_PrevAlarmsState = 0;
unsigned int G_AlarmsState = 0;

const byte c_UnderVoltAlarm = 1;
const byte c_RainAlarm = 2;
const byte c_TemperatureAlarm = 3;
const byte c_DistanceTooCloseAlarm = 4;
const byte c_MotionAlarm = 5;
const byte c_COAlarm = 6;
const byte c_GasAlarm = 7;

const byte c_DistanceTooClose = 30;

// const byte c_nRadioSSPin = 53;
// const byte c_nSDSSPin = ;
const byte c_nLCDSSPin = 5;
const byte c_nLCDResetPin = 4;
const byte c_nLCDDCPin = 6;

const unsigned int c_UpdateInterval = 5000;
unsigned long G_LastUpdateTime = 0;
byte G_AlarmID = 0;

Alarm G_Alarm;
Radio G_Radio;
Debug G_Debug;
Buzzer G_Buzzer;
LEDs G_LEDs;
RTC G_RTC;
BMP180 G_BMP180;
LCD_ILI9341 G_LCD;
// MPU6050 G_AccelGyro(0x69);
USRangeSensor G_USRS;
Voltmeter G_VM;

DigitalSensor G_GasSensor(c_MQ2GasPin, 2000);
DigitalSensor G_COSensor(c_MQ7COPin, 2000);
DigitalSensor G_RainSensor(c_RainPin, 2000, true);
DigitalSensor G_PIRSensor(c_PIRPin, 2000);

char lcdCharArray[32];
PString lcdString(lcdCharArray, sizeof(lcdCharArray));

uint8_t G_RadioBuf[RH_RF22_MAX_MESSAGE_LEN];
uint8_t G_RadioBufLen = sizeof(G_RadioBuf);

static WORKING_AREA(waRedLEDThread, 64);

static msg_t RedLEDThread(void* arg)
{
    G_LEDs.run(LEDs::LEDRed);
    return 0;
}

static WORKING_AREA(waGreenLEDThread, 64);

static msg_t GreenLEDThread(void* arg)
{
    G_LEDs.run(LEDs::LEDGreen);
    return 0;
}

static WORKING_AREA(waBuzzerThread, 64);

static msg_t BuzzerThread(void* arg)
{
    G_Buzzer.run();
    return 0;
}

static WORKING_AREA(waBMP180Thread, 64);

static msg_t BMP180Thread(void* arg)
{
    G_BMP180.run();
    return 0;
}

static WORKING_AREA(waLCDThread, 256);

static msg_t LCDThread(void* arg)
{
    while (1) {
        // Print time
        chThdSleepMilliseconds(100);
        sprintf(lcdCharArray, "%02d:%02d:%02d %s", G_RTC.getHours12(),
                G_RTC.getMinutes(), G_RTC.getSeconds(), (G_RTC.getPM() ? "PM" : "AM"));

        lockSPI();
        G_LCD.setCursor(100, 1);
        G_LCD.print(lcdString);
        unlockSPI();

        // Print day of week and date
        chThdSleepMilliseconds(100);
        lcdString.begin();
        switch (G_RTC.getDayOfWeek()) {
        case 0:
            lcdString.print("Huh?");
            break;

        case 1:
            lcdString.print("Sunday");
            break;

        case 2:
            lcdString.print("Monday");
            break;

        case 3:
            lcdString.print("Tuesday");
            break;

        case 4:
            lcdString.print("Wednesday");
            break;

        case 5:
            lcdString.print("Thursday");
            break;

        case 6:
            lcdString.print("Friday");
            break;

        case 7:
            lcdString.print("Saturday");
            break;
        }
        lcdString.print("      "); // Clear some space for next part

        sprintf(lcdCharArray + 10, "%02d/%02d/%02d", G_RTC.getDay(),
                G_RTC.getMonth(), G_RTC.getYear());

        lockSPI();
        G_LCD.setCursor(70, 4);
        G_LCD.print(lcdString);
        unlockSPI();

        // Print temperature
        chThdSleepMilliseconds(100);

        lcdString.begin();
        lcdString.print(G_BMP180.getTemperature(), 1);
        lcdString += " C";

        lockSPI();
        G_LCD.setCursor(60, 7);
        G_LCD.print(lcdString);
        unlockSPI();

        // Print pressure
        chThdSleepMilliseconds(100);

        lcdString.begin();
        lcdString.print(G_BMP180.getPressure(), 1);
        lcdString += " mbar";

        lockSPI();
        G_LCD.setCursor(180, 7);
        G_LCD.print(lcdString);
        unlockSPI();

        // Print distance from USRangeSensor on reverse
        chThdSleepMilliseconds(100);
        lcdString.begin();

        if (G_RTC.getSeconds() % 2) {//TODO: use car reverse flag
            lcdString = "Distance = ";
            lcdString.print(G_USRS.getDistance());
            lcdString += " cm    ";
        } else {
            lcdString.print("                        ");
        }

        lockSPI();
        if (G_USRS.getDistance() < c_DistanceTooClose) {
            G_LCD.setTextColor(RGB16_RED);
        }
        G_LCD.setCursor(60, 10);
        G_LCD.print(lcdString);
        unlockSPI();

        // Print flashing red "Alarm!" if an alarm is raised
        chThdSleepMilliseconds(100);

        lcdString.begin();

        if ((G_AlarmsState > 0) && (G_RTC.getSeconds() % 2)) {
            lcdString.print("   Alarm! Alarm! Alarm!   ");
        } else {
            lcdString.print("                          ");
        }

        lockSPI();
        G_LCD.setCursor(40, 13);
        G_LCD.setTextColor(RGB16_RED);
        G_LCD.print(lcdString);
        unlockSPI();

        // Print alarms
        chThdSleepMilliseconds(100);

        // 1st line
        lcdString.begin();
        if (bitRead(G_AlarmsState, c_GasAlarm)) {
            lcdString.print("Combustible Gas!  ");//18
        } else {
            lcdString.print("                  ");
        }

        lockSPI();
        G_LCD.setTextColor(RGB16_WHITE);
        G_LCD.setCursor(2, 16);
        G_LCD.print(lcdString);
        unlockSPI();

        // 2nd line
        lcdString.begin();
        if (bitRead(G_AlarmsState, c_MotionAlarm)) {
            lcdString.print("Motion detected!  ");//18
        } else {
            lcdString.print("                  ");
        }

        lockSPI();
        G_LCD.setCursor(2, 19);
        G_LCD.print(lcdString);
        unlockSPI();

        // 3rd line
        lcdString.begin();
        if (bitRead(G_AlarmsState, c_RainAlarm)) {
            lcdString.print("Rain!  ");//7
        } else {
            lcdString.print("       ");
        }
        if (bitRead(G_AlarmsState, c_TemperatureAlarm)) {
            lcdString.print("Temp. too high!  ");//17
        } else {
            lcdString.print("                 ");
        }

        lockSPI();
        G_LCD.setCursor(2, 22);
        G_LCD.print(lcdString);
        unlockSPI();

        // 4th line
        lcdString.begin();
        if (bitRead(G_AlarmsState, c_COAlarm)) {
            lcdString.print("High CO level!  ");//16
        } else {
            lcdString.print("                ");
        }
        if (bitRead(G_AlarmsState, c_UnderVoltAlarm)) {
            lcdString.print("Battery low!  ");//14
        } else {
            lcdString.print("              ");
        }

        lockSPI();
        G_LCD.setCursor(2, 25);
        G_LCD.print(lcdString);
        unlockSPI();
    }

    return 0;
}

static WORKING_AREA(waRadioThread, 128);

static msg_t RadioThread(void* arg)
{
    while (1) {
        G_Radio.update();
    }
    return 0;
}

void mainThread()
{
    // Start Red LED thread
    chThdCreateStatic(waRedLEDThread, sizeof(waRedLEDThread),
                      NORMALPRIO, RedLEDThread, NULL);

    chThdCreateStatic(waGreenLEDThread, sizeof(waGreenLEDThread),
                      NORMALPRIO, GreenLEDThread, NULL);

    chThdCreateStatic(waBuzzerThread, sizeof(waBuzzerThread),
                      NORMALPRIO, BuzzerThread, NULL);

    chThdCreateStatic(waBMP180Thread, sizeof(waBMP180Thread),
                      NORMALPRIO, BMP180Thread, NULL);

    chThdCreateStatic(waLCDThread, sizeof(waLCDThread),
                      NORMALPRIO, LCDThread, NULL);

    chThdCreateStatic(waRadioThread, sizeof(waRadioThread),
                      NORMALPRIO, RadioThread, NULL);

    while (1) {
#ifdef DEBUG
        Serial.print("M1");
#endif

        //Serial.println(G_RTC.getTime());
        //chThdSleepMilliseconds(100);
        //Serial.println(G_BMP180.getPressure(), 2);
        //Serial.print("M");

//         chThdYield();//chThdSleepMilliseconds(100);
        chThdSleepMilliseconds(10);
        Serial.print("M2");

        // Check for data on Serial
        if (Serial.available() > 0) {
            if (Serial.read() == 'd') {
                G_Debug.start();
            } else {
                // Clear buffer if 'd' is not received.
                drainSerial();
            }
        }
        Serial.print("M3");
        chThdSleepMilliseconds(10);//         chThdYield();
        Serial.print("M4");

        // Check for messages from radio
        bool result = false;
        G_RadioBufLen = sizeof(G_RadioBuf);
        lockSPI();
        Serial.print("M5");
        result = G_Radio.recv(G_RadioBuf, &G_RadioBufLen);
        Serial.print("M6");
        unlockSPI();
        Serial.print("M7");

        if (result) {
            switch (G_RadioBuf[0]) {
            case 'c':
                // Alarm! Next byte is alarm level
                G_Alarm.setLevel((Alarm::AlarmLevel)(G_RadioBuf[1] - '0'));
                break;

            default:
                Serial.println("Invalid message.");
            }
        }
        Serial.print("M8");
        chThdSleepMilliseconds(10);//         chThdYield();
        Serial.print("M9");

        bitWrite(G_AlarmsState, c_GasAlarm, G_GasSensor.getValue());
        bitWrite(G_AlarmsState, c_COAlarm, G_COSensor.getValue());
        bitWrite(G_AlarmsState, c_MotionAlarm, G_PIRSensor.getValue());
        bitWrite(G_AlarmsState, c_RainAlarm, G_RainSensor.getValue());
        bitWrite(G_AlarmsState, c_DistanceTooCloseAlarm, G_USRS.getDistance() <= c_DistanceTooClose);
        bitWrite(G_AlarmsState, c_UnderVoltAlarm, G_VM.getUndervolt());

        if (G_AlarmsState != G_PrevAlarmsState) {
            G_PrevAlarmsState = G_AlarmsState;
            G_AlarmID++;

            if (G_AlarmsState >= 8) {
                G_Alarm.setLevel(Alarm::AlarmFatal);
            } else if (G_AlarmsState >= 4) {
                G_Alarm.setLevel(Alarm::AlarmDanger);
            } else if (G_AlarmsState > 2) {
                G_Alarm.setLevel(Alarm::AlarmWarning);
            } else if (G_AlarmsState >= 1) {
                G_Alarm.setLevel(Alarm::AlarmAttention);
            } else if (G_AlarmsState == 0) {
                G_Alarm.setLevel(Alarm::AlarmOff);
            }
        }
        Serial.print("M10");
        chThdSleepMilliseconds(10);//         chThdYield();
        Serial.print("M11");
        if (millis() > (G_LastUpdateTime + c_UpdateInterval)) {
            // Didn't send a status message in a while.
            G_LastUpdateTime = millis();
            //G_AlarmID++; //FIXME
            G_RadioBuf[0] = 's';
            G_RadioBuf[1] = G_AlarmID;
            G_RadioBuf[2] = G_Alarm.getLevel();
            G_LEDs.turnOn(LEDs::LEDGreen);
            Serial.print("M12");
            lockSPI();
            Serial.print("M13");
            if (!G_Radio.send(G_RadioBuf, 3)) {
                G_Alarm.raiseLevel(Alarm::AlarmRadioSignal);
            }
            Serial.print("M14");
            unlockSPI();
            Serial.print("M15");
            G_LEDs.turnOff(LEDs::LEDGreen);
        }
        Serial.print("M16");
        chThdSleepMilliseconds(10);//         chThdYield();
        Serial.print("M17");
        //if(G_USRS.getDistance() != 0)
        {
//             Serial.print("Distance: ");
//             Serial.println(G_USRS.getDistance());
            G_USRS.getDistance();
        }
        Serial.print("M18");
        // If the 'F' key is pressed we lower the alarm level in the remote. Like a silence button.
//         if (G_Key == 'F') {
//             G_Alarm.lowerLevel(Alarm::AlarmAttention);
//
//             //TODO: Add code to check up on alarms with car
//         } else {
//             G_Alarm.setLevel((Alarm::AlarmLevel)(G_Key - '0'));
//             // First byte is 'c', which means command. 2nd byte is the key code.
//             G_RadioBuf[0] = 'c';
//             G_RadioBuf[1] = G_Key;
//             G_LEDs.turnOn(LEDs::LEDGreen);
//             if (!G_Radio.send(G_RadioBuf, 2)) {
//                 G_Alarm.raiseLevel(Alarm::AlarmRadioSignal);
//             }
//             G_LEDs.turnOff(LEDs::LEDGreen);
//         }

        chThdSleepMilliseconds(10);//         chThdYield();
        Serial.print("M19");
        G_GasSensor.update();
        G_COSensor.update();
        G_RainSensor.update();
        G_PIRSensor.update();
        G_USRS.update();
        Serial.print("M20");
    }
}

void setup()
{
    // Init Serial with PC
    Serial.begin(115200);
    Serial.setTimeout(100);

    // Init backup comm. channel
    Serial1.begin(9600);
    Serial1.setTimeout(100);

    pinMode(40, INPUT_PULLUP);
    if (digitalRead(40) == false) {
        G_Radio.switchCommSystem(1);
    }

    // Init LCD
    G_LCD.begin();
    G_LCD.clear();
    G_LCD.setFont(FONT_SIZE_XLARGE);

    // Initialize Radio
    G_Radio.init();

    // Start I2C bus
    Wire.begin();

    // Init RTC
    G_RTC.init();

    // Init BMP180
    G_BMP180.init();

    // Welcome pattern
    G_Buzzer.salute();
    G_LEDs.setPattern(LEDs::LEDGreen, LEDs::Blink2ShortStop);
    G_LEDs.setPattern(LEDs::LEDRed, LEDs::Blink2ShortStop);

    Serial.println("Send 'd' to enter debug mode.");

    // start ChibiOS
    chBegin(mainThread);
}

void loop()
{
    // Never reached. Does nothing.
}
