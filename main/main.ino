/*******************************************************************************
// Copyright © 2025 Zach Butler
// Use of this software is governed by the Business Source License v1.1 (BUSL)
// included in the LICENSE file. The license must be kept with this code.
*******************************************************************************/

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_ST7789.h> // display driver
#include <SPI.h>

class Button {
    public:
        Button(uint8_t pin) : _pin(pin), _state(0) {
            pinMode(_pin, INPUT_PULLDOWN);
        }

        bool isPressed() {
            _state = (_state << 1) | digitalRead(_pin);
            return (_state == 0x8000);
        }

    private:
        uint8_t _pin;
        uint16_t _state;
};

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

uint16_t failures = 0;
uint16_t intake = 0;
uint16_t output = 0;
uint8_t position = 0;
Button* swAdd;
Button* swSub;

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello, world!"));

    // init buttons
    pinMode(0, INPUT_PULLUP);
    swAdd = new Button(1);
    swSub = new Button(2);

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

    update();
}

void update() {
    display.fillScreen(ST77XX_BLACK);

    // print intake
    display.setCursor(15, 10);
    display.setTextColor(ST77XX_BLUE);
    display.setTextSize(6);
    display.printf("%5u", intake);
    display.setCursor(200, 31);
    display.setTextSize(3);
    display.println("mL");

    // print output
    display.setCursor(15, 60);
    display.setTextColor(ST77XX_YELLOW);
    display.setTextSize(6);
    display.printf("%5u", output);
    display.setCursor(200, 81);
    display.setTextSize(3);
    display.println("mL");

    // print failures
    display.setCursor(15, 115);
    display.setTextColor(ST77XX_RED);
    display.setTextSize(2);
    display.printf("%u missed", failures);
}

void loop() {
    bool write = false;
    if (!digitalRead(0)) {
        intake = 0;
        write = true;
    } else if (swAdd->isPressed()) {
        intake += 5;
        write = true;
    } else if (swSub->isPressed()) {
        write = true;
        if (intake > 4) intake -= 5;
        else if (intake > 0) intake = 0;
        else write = false;
    }
    if (write == true) update();
    delayMicroseconds(1000);
}
