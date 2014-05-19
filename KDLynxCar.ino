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
#include "windowcontroller.h"
#include "doorcontroller.h"

const byte c_MQ7COPin = 15;
const byte c_MQ2GasPin = 16;
const byte c_RainPin = 14;
const byte c_PIRPin = A8;

const byte c_nLCDSSPin = 5;
const byte c_nLCDResetPin = 4;
const byte c_nLCDDCPin = 6;

const byte c_WindowOnPin = A2;
const byte c_WindowDirPin = A3;

const byte c_CarOnPin = 34;
const byte c_CarReversePin = 35;

const byte c_DistanceVeryClose = 60;
const byte c_DistanceTooClose = 30;

unsigned int G_PrevAlarmsState = 0;
unsigned int G_AlarmsState = 0;
// unsigned int G_PrevLCDAlarmsState = 0;
unsigned int G_LCDAlarmsState = 0;
byte G_AlarmID = 0;
// bool G_AlarmsSilenced = false;
//bool G_RemoteReachable = true;

enum Alarms {c_UnderVoltAlarm = 1, c_RainAlarm, c_TemperatureAlarm, c_DistanceVeryCloseAlarm,
             c_DistanceTooCloseAlarm, c_MotionAlarm, c_COAlarm, c_GasAlarm
            };

const unsigned int c_RemoteUpdateInterval = 3000;
unsigned long G_RemoteLastUpdateTime = 0;


#ifdef DEBUG
Debug G_Debug;
#endif

Alarm G_Alarm;
Radio G_Radio;
Buzzer G_Buzzer;
LEDs G_LEDs;
RTC G_RTC;
BMP180 G_BMP180;
LCD_ILI9341 G_LCD;
USRangeSensor G_USRS;
Voltmeter G_VM;
WindowController G_WinC(c_WindowOnPin, c_WindowDirPin, 1000);
DoorController G_DoorController;

DigitalSensor G_GasSensor(c_MQ2GasPin, 2000, true);
DigitalSensor G_COSensor(c_MQ7COPin, 2000, true);
DigitalSensor G_RainSensor(c_RainPin, 2000, true);
DigitalSensor G_PIRSensor(c_PIRPin, 2000);
DigitalSensor G_CarOn(c_CarOnPin, 100);
DigitalSensor G_CarReverse(c_CarReversePin, 100);

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
    byte prevSeconds = 200;
    byte prevMinutes = 200;
    byte prevDay = 200;
    double prevTemp = 2000;
    double prevPressure = 2000;
    byte prevDistance = 255;
    bool prevReverse = false;
    unsigned int prevLCDAlarmsState = 0;

    while (1) {
        chThdSleepMilliseconds(50);
        // Print the rest of the time
        if (G_RTC.getMinutes() != prevMinutes) {
            prevMinutes = G_RTC.getMinutes();

            sprintf(lcdCharArray, "%02d:%02d:   %s", G_RTC.getHours12(),
                    G_RTC.getMinutes(), (G_RTC.getPM() ? "PM" : "AM"));

            lockSPI();
            G_LCD.setCursor(100, 1);
            G_LCD.print(lcdString);
            unlockSPI();
        }

        // Print seconds from time.
        if (G_RTC.getSeconds() != prevSeconds) {
            prevSeconds = G_RTC.getSeconds();

            sprintf(lcdCharArray, "%02d", G_RTC.getSeconds());

            lockSPI();
            G_LCD.setCursor(153, 1);
            G_LCD.print(lcdString);
            unlockSPI();
        }

        chThdSleepMilliseconds(50);
        // Print day of week and date
        if (G_RTC.getDay() != prevDay) {
            prevDay = G_RTC.getDay();

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
        }

        chThdSleepMilliseconds(50);
        // Print temperature
        if (G_BMP180.getTemperature() != prevTemp) {
            prevTemp = G_BMP180.getTemperature();

            lcdString.begin();
            lcdString.print(G_BMP180.getTemperature(), 1);
            lcdString += " C";

            lockSPI();
            G_LCD.setCursor(60, 7);
            G_LCD.print(lcdString);
            unlockSPI();
        }

        chThdSleepMilliseconds(50);
        // Print pressure
        if (G_BMP180.getPressure() != prevPressure) {
            prevPressure = G_BMP180.getPressure();

            lcdString.begin();
            lcdString.print(G_BMP180.getPressure(), 1);
            lcdString += " mbar";

            lockSPI();
            G_LCD.setCursor(180, 7);
            G_LCD.print(lcdString);
            unlockSPI();
        }

        chThdSleepMilliseconds(50);
        // Print distance from USRangeSensor on reverse
        if ((G_CarOn.getValue() == true) && (G_CarReverse.getValue() == true)) {
            prevReverse = true;
            if (G_USRS.getDistance() != prevDistance) {
                prevDistance = G_USRS.getDistance();

                lcdString.begin();

                lcdString = "Distance = ";
                if (G_USRS.getDistance() > 150) {
                    lcdString += "???";
                } else {
                    lcdString.print(G_USRS.getDistance());
                }
                lcdString += " cm    ";

                lockSPI();
                if (G_USRS.getDistance() < c_DistanceTooClose) {
                    G_LCD.setTextColor(RGB16_RED);
                }
                G_LCD.setCursor(60, 10);
                G_LCD.print(lcdString);
                G_LCD.setTextColor(RGB16_WHITE);
                unlockSPI();
            }
        } else {
            if (prevReverse == true) {
                prevReverse = false;

                lcdString.begin();
                lcdString.print("                        ");

                lockSPI();
                G_LCD.setCursor(60, 10);
                G_LCD.print(lcdString);
                unlockSPI();
            }
        }

        chThdSleepMilliseconds(50);
        // Print flashing red "Alarm!" if an alarm is raised
        lcdString.begin();

        if ((G_AlarmsState != 0) && (G_RTC.getSeconds() % 2)) {
            lcdString.print("   Alarm! Alarm! Alarm!   ");
        } else {
            lcdString.print("                          ");
        }

        lockSPI();
        G_LCD.setCursor(40, 13);
        G_LCD.setTextColor(RGB16_RED);
        G_LCD.print(lcdString);
        G_LCD.setTextColor(RGB16_WHITE);
        unlockSPI();

        chThdSleepMilliseconds(50);
        // Print alarms
        if (G_LCDAlarmsState != prevLCDAlarmsState) {
            prevLCDAlarmsState = G_LCDAlarmsState;

            // 1st line
            lcdString.begin();
            if (bitRead(G_LCDAlarmsState, c_GasAlarm)) {
                lcdString.print("Combustible Gas!  ");//18
            } else {
                lcdString.print("                  ");
            }

            lockSPI();
            G_LCD.setCursor(2, 16);
            G_LCD.print(lcdString);
            unlockSPI();

            chThdSleepMilliseconds(30);
            // 2nd line
            lcdString.begin();
            if (bitRead(G_LCDAlarmsState, c_MotionAlarm)) {
                lcdString.print("Motion detected!  ");//18
            } else {
                lcdString.print("                  ");
            }

            lockSPI();
            G_LCD.setCursor(2, 19);
            G_LCD.print(lcdString);
            unlockSPI();

            chThdSleepMilliseconds(30);
            // 3rd line
            lcdString.begin();
            if (bitRead(G_LCDAlarmsState, c_RainAlarm)) {
                lcdString.print("Rain!  ");//7
            } else {
                lcdString.print("       ");
            }
            if (bitRead(G_LCDAlarmsState, c_TemperatureAlarm)) {
                lcdString.print("Temp. too high!  ");//17
            } else {
                lcdString.print("                 ");
            }

            lockSPI();
            G_LCD.setCursor(2, 22);
            G_LCD.print(lcdString);
            unlockSPI();

            chThdSleepMilliseconds(30);
            // 4th line
            lcdString.begin();
            if (bitRead(G_LCDAlarmsState, c_COAlarm)) {
                lcdString.print("High CO level!  ");//16
            } else {
                lcdString.print("                ");
            }
            if (bitRead(G_LCDAlarmsState, c_UnderVoltAlarm)) {
                lcdString.print("Battery low!  ");//14
            } else {
                lcdString.print("              ");
            }

            lockSPI();
            G_LCD.setCursor(2, 25);
            G_LCD.print(lcdString);
            unlockSPI();
        }
#ifdef DEBUG
        Serial.println("L");
#endif
    }

    return 0;
}

static WORKING_AREA(waRadioThread, 256);

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
        Serial.print("M");
#endif

#ifdef DEBUG
        chThdSleepMilliseconds(10);
        // Check for data on Serial
        if (Serial.available() > 0) {
            if (Serial.read() == 'd') {
                G_Debug.start();
            } else {
                // Clear buffer if 'd' is not received.
                drainSerial();
            }
        }
#endif

        chThdSleepMilliseconds(10);
        // Check for messages from radio
        bool result = false;
        G_RadioBufLen = sizeof(G_RadioBuf);
        lockSPI();
#ifdef DEBUG
        Serial.print("M5");
#endif
        result = G_Radio.recv(G_RadioBuf, &G_RadioBufLen);
#ifdef DEBUG
        Serial.print("M6");
#endif
        unlockSPI();
#ifdef DEBUG
        Serial.print("M7");
#endif

        // Got a message from the remote, and it's a command.
        if (result && (G_RadioBuf[0] == 'c')) {
            switch (G_RadioBuf[1]) {
            case '0':
                //Open Doors
                G_DoorController.open();
                break;

            case '1':
                // Close doors
                G_DoorController.close();
                break;

            case '4':
                // Open Windows
                if (G_CarOn.getValue() == true) {
                    G_WinC.open();
                }
                break;

            case '5':
                // Close Windows
                G_WinC.close();
                break;

            case 'C':
                if ((G_GasSensor.getValue() == false) && (G_COSensor.getValue() == false)) {
                    G_Alarm.lowerLevel(Alarm::AlarmAttention);
                }
                break;

            case 'D':
                G_Alarm.setLevel(Alarm::AlarmOff);
                G_AlarmsState = 0;
                G_PrevAlarmsState = 1;
                G_LCDAlarmsState = 0;
                break;
            }
        }
#ifdef DEBUG
        else {
            Serial.println("Invalid message.");
        }
#endif

        chThdSleepMilliseconds(10);
        if (G_GasSensor.getValue() == true) {
            bitSet(G_AlarmsState, c_GasAlarm);
            G_WinC.open();
        }

        if (G_COSensor.getValue() == true) {
            bitSet(G_AlarmsState, c_COAlarm);
            G_WinC.open();
        }

        if ((G_CarOn.getValue() == false) && (G_DoorController.getOpen() == false) && (G_PIRSensor.getValue() == true)) {
            bitSet(G_AlarmsState, c_MotionAlarm);
        }
        if ((G_CarOn.getValue() == true) || (G_DoorController.getOpen() == true)) {
            bitClear(G_AlarmsState, c_MotionAlarm);
        }

        if ((G_CarOn.getValue() == false) && (G_WinC.getOpen() == true) && (G_RainSensor.getValue() == true)) {
            bitSet(G_AlarmsState, c_RainAlarm);
            G_WinC.close();
        }

        if ((G_CarOn.getValue() == true) && (G_CarReverse.getValue() == true)) {
            if (G_USRS.getDistance() <= c_DistanceTooClose) {
                bitSet(G_AlarmsState, c_DistanceTooCloseAlarm);
                bitClear(G_AlarmsState, c_DistanceVeryCloseAlarm);
            } else if (G_USRS.getDistance() <= c_DistanceVeryClose) {
                bitSet(G_AlarmsState, c_DistanceVeryCloseAlarm);
                bitClear(G_AlarmsState, c_DistanceTooCloseAlarm);
            } else {
                bitClear(G_AlarmsState, c_DistanceVeryCloseAlarm);
                bitClear(G_AlarmsState, c_DistanceTooCloseAlarm);
            }
        } else {
            bitClear(G_AlarmsState, c_DistanceVeryCloseAlarm);
            bitClear(G_AlarmsState, c_DistanceTooCloseAlarm);
        }

        bitWrite(G_AlarmsState, c_UnderVoltAlarm, G_VM.getUndervolt());

        // Copy only extra alarms
        G_LCDAlarmsState |= G_AlarmsState;

        // Process new alarms
        if (G_AlarmsState != G_PrevAlarmsState) {
            G_PrevAlarmsState = G_AlarmsState;
            // Increment Alarm ID if alarm state changed. Remote only sounds on new alarm IDs.
            G_AlarmID++;
//             G_AlarmsSilenced = false;//TODO: Silencing alarms

            // Map alarms to alarm levels
            if (bitRead(G_AlarmsState, c_GasAlarm) || bitRead(G_AlarmsState, c_COAlarm)) {
                G_Alarm.setLevel(Alarm::AlarmFatal);
                G_Alarm.disableAutoSilence();
            } else if (bitRead(G_AlarmsState, c_MotionAlarm) || bitRead(G_AlarmsState, c_DistanceTooCloseAlarm)) {
                G_Alarm.setLevel(Alarm::AlarmDanger);
                G_Alarm.enableAutoSilence();
            } else if (bitRead(G_AlarmsState, c_RainAlarm) || bitRead(G_AlarmsState, c_DistanceVeryCloseAlarm)) {
                G_Alarm.setLevel(Alarm::AlarmWarning);
                G_Alarm.enableAutoSilence();
            } else if (bitRead(G_AlarmsState, c_UnderVoltAlarm)) {
                G_Alarm.setLevel(Alarm::AlarmAttention);
                G_Alarm.enableAutoSilence();
            } else {
                G_Alarm.setLevel(Alarm::AlarmOff);
            }
        }

        chThdSleepMilliseconds(10);
        if (millis() > (G_RemoteLastUpdateTime + c_RemoteUpdateInterval)) {
            // Didn't send a status message in a while.
            G_RemoteLastUpdateTime = millis();
            G_RadioBuf[0] = 's';
            if (G_CarOn.getValue() == true) {
                G_AlarmID++;
                G_RadioBuf[1] = (uint8_t) Alarm::AlarmOff;
            } else {
                G_RadioBuf[1] = G_Alarm.getLevel();
            }
            G_RadioBuf[2] = G_AlarmID;
            G_RadioBuf[3] = 'm';
//            G_LEDs.turnOn(LEDs::LEDGreen);
#ifdef DEBUG
            Serial.print("M12");
#endif
            lockSPI();
#ifdef DEBUG
            Serial.print("M13");
#endif
            if (!G_Radio.send(G_RadioBuf, 4)) {
//                 G_Alarm.raiseLevel(Alarm::AlarmRadioSignal);
            }
#ifdef DEBUG
            Serial.print("M14");
#endif
            unlockSPI();
#ifdef DEBUG
            Serial.print("M15");
#endif
//            G_LEDs.turnOff(LEDs::LEDGreen);
        }

        chThdSleepMilliseconds(10);
        G_GasSensor.update();
        G_COSensor.update();
        G_RainSensor.update();
        G_PIRSensor.update();
        G_CarOn.update();
        G_CarReverse.update();
        G_DoorController.update();
        G_Alarm.update();
        G_VM.update();
        G_WinC.update();

        // Only update USRangeSensor on reverse
        if ((G_CarOn.getValue() == true) && (G_CarReverse.getValue() == true)) {
            G_USRS.update();
        }
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

    // Init LCD
    G_LCD.begin();
    G_LCD.clear();
    G_LCD.setFont(FONT_SIZE_XLARGE);

    // Initialize Radio
    pinMode(40, INPUT_PULLUP);
    G_Radio.init(digitalRead(40) == false); // Switch to Serial1 if pin 40 is low

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

#ifdef DEBUG
    Serial.println("Send 'd' to enter debug mode.");
#endif

    // start ChibiOS
    chBegin(mainThread);
}

void loop()
{
    // Never reached. Does nothing.
}
