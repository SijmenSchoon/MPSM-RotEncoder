/**
 * Quick hacky attempt to emulate the rotary encoder
 * of a MonoPrice Select Mini/Malyan M200.
 *
 * Set-up:
 *  Connect the Arduino pins 2, 3 and 4 to the blue, black
 *  and red cables of the control panel connector, and
 *  connect the orange cable to ground.
 *
 * Usage:
 *  Open up a serial monitor (115200 baud).
 *   - 'a':   Move counter-clockwise
 *   - 'd':   Move clockwise
 *   - space: Confirm option
 */

#include <Arduino.h>

const int PIN_ENCODER_B  = 2;
const int PIN_ENCODER_A  = 3;
const int PIN_PUSHBUTTON = 4;

const bool PHASES[4][2] = {{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 }};

int current_phase = 0;

void setup()
{
    Serial.begin(115200);

    // Switching to INPUT to float the pin, as
    // that works better than pulling them high.
    pinMode(PIN_PUSHBUTTON, INPUT);
    pinMode(PIN_ENCODER_A, INPUT);
    pinMode(PIN_ENCODER_B, INPUT);
}

void rotateEncoder(bool clockwise)
{
    for (int i = 0; i < 4; i++) {
        auto phase = PHASES[clockwise ? (4 - i) : i];
        pinMode(PIN_ENCODER_A, phase[0] ? INPUT : OUTPUT);
        pinMode(PIN_ENCODER_B, phase[1] ? INPUT : OUTPUT);
        delay(5);
    }
}

void loop()
{
    if (Serial.available()) {
        char input = Serial.read();
        switch (input) {
        case 'a':  // Counter-clockwise
            rotateEncoder(false);
            Serial.println("CCW");
            break;

        case 'd':  // Clockwise
            rotateEncoder(true);
            Serial.println("CW");
            break;

        case ' ':  // Select
            pinMode(PIN_PUSHBUTTON, OUTPUT); delay(10);
            pinMode(PIN_PUSHBUTTON, INPUT);  delay(10);
            Serial.println("OK");
            break;

        default:
            Serial.println("?");
        }
    }
}
