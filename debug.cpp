#include "debug.h"
#include "globals.h"

#include <ChibiOS_AVR.h>

bool Debug::s_DebugMode = false;

LEDs Debug::s_LEDs;
Buzzer Debug::s_Buzzer;
Alarm Debug::s_Alarm;
Radio Debug::s_Radio;

Debug::Debug()
{

}

Debug::~Debug()
{

}

void Debug::start()
{
    byte param = 0;
    s_DebugMode = true;

    // Print help
    printHelp();

    while (s_DebugMode == true) {
        param = 0;

        chThdYield();

        // Process data from Serial
        if (Serial.available() < 1) {
            chThdYield();
            continue;
        }

        byte inChar = Serial.read();
        switch (inChar) {
        case 'h':
            printHelp();
            break;

        case 'a':
            param = Serial.parseInt();
            s_Alarm.setLevel((Alarm::AlarmLevel)param);
            Serial.print("Setting Alarm to level ");
            Serial.println(param);
            break;

        case 'b':
            param = Serial.parseInt();
            s_Buzzer.setTone((Buzzer::BuzzerTone)param);
            Serial.print("Setting Buzzer to tone ");
            Serial.println(param);
            break;

        case 'r':
            param = Serial.parseInt();
            s_LEDs.setPattern(LEDs::LEDRed, (LEDs::BlinkType)param);
            Serial.println("Setting Red LED to pattern ");
            Serial.println(param);
            break;

        case 'g':
            param = Serial.parseInt();
            s_LEDs.setPattern(LEDs::LEDGreen, (LEDs::BlinkType)param);
            Serial.println("Setting Green LED to pattern ");
            Serial.println(param);
            break;

        case 's':
            param = Serial.parseInt();
            Serial.println("Switching comm. system.");
            s_Radio.switchCommSystem(param);
            break;

        case 'q':
            Serial.println("Exiting debug mode.");
            s_DebugMode = false;
            break;

        default:
            Serial.println("Invalid command");
        }

        chThdYield();

        // Drain buffer before looping.
        drainSerial();
    }
}

void Debug::printHelp()
{
    Serial.println("Debug mode:");
    Serial.println("Command\tAction");
    Serial.println("h\tPrint help");
    Serial.println("a#\tSet Alarm to level #");
    Serial.println("b#\tBuzz buzzer using level #");
    Serial.println("g#\tBlink Green LED using level #");
    Serial.println("r#\tBlink Red LED using level #");
    Serial.println("s#\tSwitch comm. system");
    Serial.println("q\tExit debug mode");
}
