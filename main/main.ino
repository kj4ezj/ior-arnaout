/*******************************************************************************
// Copyright © 2025 Zach Butler
// Use of this software is governed by the Business Source License v1.1 (BUSL)
// included in the LICENSE file. The license must be kept with this code.
*******************************************************************************/

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_ST7789.h> // display driver
#include <SPI.h>

#define INTAKE_MODE 0
#define OUTPUT_MODE 1

#define PIN_E1_ADD 5
#define PIN_E1_SUB 6
#define PIN_E2_ADD 9
#define PIN_E2_SUB 10
#define PIN_E3_ADD 11
#define PIN_E3_SUB 12
#define PIN_FAIL_ADD 17
#define PIN_FAIL_SUB 18
#define PIN_ONBOARD_RST 0
#define PIN_ONBOARD_ADD 1
#define PIN_ONBOARD_SUB 2
#define PIN_RST 15

uint8_t deviceMode = INTAKE_MODE;

class Button {
    private:
        uint8_t _pin;
        uint16_t _shiftReg;
        unsigned long _state;

    public:
        Button(uint8_t pin) : _pin(pin), _shiftReg(0), _state(0) {
            pinMode(_pin, INPUT_PULLDOWN);
        }

        bool isPressed() {
            _shiftReg = (_shiftReg << 1) | digitalRead(_pin);
            if (!_state && _shiftReg == 0xFFFF) {
                return _state = millis() + 400;
            } else if (_state && _shiftReg == 0x0000) {
                _state = 0;
            } else if (_state && _state + 100 < millis()) {
                return _state = millis();
            }
            return false;
        }

        bool isReleased() {
            isPressed();
            return (_state == 0);
        }
};

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

uint16_t failures = 0;
uint32_t milliliters = 0;
uint8_t position = 0;
Button* swE1Add;
Button* swE1Sub;
Button* swE2Add;
Button* swE2Sub;
Button* swE3Add;
Button* swE3Sub;
Button* swFailAdd;
Button* swFailSub;
Button* swOnboardAdd;
Button* swOnboardSub;
Button* swRst;

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello, world!"));

    // init buttons
    pinMode(PIN_ONBOARD_RST, INPUT_PULLUP);
    swE1Add = new Button(PIN_E1_ADD);
    swE1Sub = new Button(PIN_E1_SUB);
    swE2Add = new Button(PIN_E2_ADD);
    swE2Sub = new Button(PIN_E2_SUB);
    swE3Add = new Button(PIN_E3_ADD);
    swE3Sub = new Button(PIN_E3_SUB);
    swFailAdd = new Button(PIN_FAIL_ADD);
    swFailSub = new Button(PIN_FAIL_SUB);
    swOnboardAdd = new Button(PIN_ONBOARD_ADD);
    swOnboardSub = new Button(PIN_ONBOARD_SUB);
    swRst = new Button(PIN_RST);

    const unsigned long BOOT_DELAY = millis() + 32;
    while (millis() < BOOT_DELAY) {
        if (swOnboardAdd->isPressed()) {
            deviceMode = INTAKE_MODE;
            break;
        } else if (swOnboardSub->isPressed()) {
            deviceMode = OUTPUT_MODE;
            break;
        }
    }

    // turn on LCD backlight
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
    pinMode(TFT_I2C_POWER, OUTPUT);
    digitalWrite(TFT_I2C_POWER, HIGH);
    delay(10);

    // initialize ST7789 240x135 TFT LCD display
    display.init(135, 240);
    display.setRotation(3);
    display.setTextWrap(false);
    display.fillScreen(ST77XX_BLACK);

    // wait for button release after mode switch
    if (!(swOnboardAdd->isReleased() && swOnboardSub->isReleased())) {
        display.setTextColor(ST77XX_GREEN);
        display.setTextSize(3);
        display.setCursor(10, 0);
        display.print(F("Mode set:"));
        display.setCursor(80, 55);
        if (deviceMode == INTAKE_MODE) {
            display.setTextColor(ST77XX_BLUE);
            display.print(F("Intake"));
        } else {
            display.setTextColor(ST77XX_YELLOW);
            display.print(F("Output"));
        }
        display.setCursor(80, 110);
        display.setTextColor(ST77XX_RED);
        display.print(F("Let go..."));
        while (!(swOnboardAdd->isReleased() && swOnboardSub->isReleased())) {
            delayMicroseconds(1000);
        }
        display.fillScreen(ST77XX_BLACK);
    }

    // print measurement
    printFluids();
    display.setCursor(200, 51);
    display.setTextSize(3);
    display.println("mL");

    // print failures
    display.setCursor(15, 115);
    display.setTextColor(ST77XX_RED);
    display.setTextSize(2);
    display.print(F("Times "));
    if (deviceMode == INTAKE_MODE) {
        display.printf(F("inct:"));
    } else {
        display.printf(F("missed:"));
    }
    printFailures();
}

void printFailures() {
    // print failures
    display.setTextColor(ST77XX_RED, ST77XX_BLACK);
    display.setTextSize(2);
    if (deviceMode == INTAKE_MODE) {
        display.setCursor(159, 115);
    } else {
        display.setCursor(183, 115);
    }
    display.printf("%u", failures);
}

void printFluids() {
    // print measurement
    display.setCursor(15, 30);
    if (deviceMode == INTAKE_MODE) {
        display.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
    } else {
        display.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    }
    display.setTextSize(6);
    display.printf("%5u", milliliters);
}

void loop() {
    bool writeFailures = false;
    bool writeFluids = false;
    if (swE1Add->isPressed()) {
        milliliters += 1;
        writeFluids = true;
    } else if (swE1Sub->isPressed()) {
        writeFluids = true;
        if (milliliters > 0) milliliters -= 1;
        else writeFluids = false;
    } else if (swE2Add->isPressed()) {
        milliliters += 10;
        writeFluids = true;
    } else if (swE2Sub->isPressed()) {
        writeFluids = true;
        if (milliliters > 9) milliliters -= 10;
        else if (milliliters > 0) milliliters = 0;
        else writeFluids = false;
    } else if (swE3Add->isPressed()) {
        milliliters += 100;
        writeFluids = true;
    } else if (swE3Sub->isPressed()) {
        writeFluids = true;
        if (milliliters > 99) milliliters -= 100;
        else if (milliliters > 0) milliliters = 0;
        else writeFluids = false;
    } else if (swFailAdd->isPressed()) {
        failures += 1;
        writeFailures = true;
    } else if (swFailSub->isPressed()) {
        writeFailures = true;
        if (failures > 0) failures -= 1;
        else writeFailures = false;
    } else if (!digitalRead(PIN_ONBOARD_RST) || swRst->isPressed()) {
        failures = 0;
        milliliters = 0;
        writeFailures = true;
        writeFluids = true;
    } else if (swOnboardAdd->isPressed()) {
        milliliters += 5;
        writeFluids = true;
    } else if (swOnboardSub->isPressed()) {
        writeFluids = true;
        if (milliliters > 4) milliliters -= 5;
        else if (milliliters > 0) milliliters = 0;
        else writeFluids = false;
    }
    if (writeFluids == true) printFluids();
    if (writeFailures == true) printFailures();
    delayMicroseconds(1000);
}
