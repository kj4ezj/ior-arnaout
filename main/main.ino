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

#define PIN_ONBOARD_RST 0
#define PIN_ONBOARD_ADD 1
#define PIN_ONBOARD_SUB 2

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

uint16_t failures = 2;
uint16_t milliliters = 0;
uint8_t position = 0;
Button* swAdd;
Button* swSub;

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello, world!"));

    // init buttons
    pinMode(PIN_ONBOARD_RST, INPUT_PULLUP);
    swAdd = new Button(PIN_ONBOARD_ADD);
    swSub = new Button(PIN_ONBOARD_SUB);

    const unsigned long BOOT_DELAY = millis() + 32;
    while (millis() < BOOT_DELAY) {
        if (swAdd->isPressed()) {
            deviceMode = INTAKE_MODE;
            break;
        } else if (swSub->isPressed()) {
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
    if (!(swAdd->isReleased() && swSub->isReleased())) {
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
        while (!(swAdd->isReleased() && swSub->isReleased())) {
            delayMicroseconds(1000);
        }
        display.fillScreen(ST77XX_BLACK);
    }

    // print measurement
    display.setCursor(15, 30);
    if (deviceMode == INTAKE_MODE) {
        display.setTextColor(ST77XX_BLUE);
    } else {
        display.setTextColor(ST77XX_YELLOW);
    }
    display.setTextSize(6);
    display.printf("%5u", milliliters);
    display.setCursor(200, 51);
    display.setTextSize(3);
    display.println("mL");

    // print failures
    display.setCursor(15, 115);
    display.setTextColor(ST77XX_RED);
    display.setTextSize(2);
    if (deviceMode == INTAKE_MODE) {
        display.printf("Inc. %u times.", failures);
    } else {
        display.printf("Unmeasured %u times.", failures);
    }
}

void update() {
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
    bool write = false;
    if (!digitalRead(PIN_ONBOARD_RST)) {
        milliliters = 0;
        write = true;
    } else if (swAdd->isPressed()) {
        milliliters += 5;
        write = true;
    } else if (swSub->isPressed()) {
        write = true;
        if (milliliters > 4) milliliters -= 5;
        else if (milliliters > 0) milliliters = 0;
        else write = false;
    }
    if (write == true) update();
    delayMicroseconds(1000);
}
