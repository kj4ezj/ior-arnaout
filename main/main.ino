/*******************************************************************************
// Copyright © 2025 Zach Butler
// Use of this software is governed by the Business Source License v1.1 (BUSL)
// included in the LICENSE file. The license must be kept with this code.
*******************************************************************************/

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_ST7789.h> // display driver
#include <SPI.h>

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

uint16_t failures = 0;
uint16_t intake = 0;
uint16_t output = 0;
uint8_t position = 0;

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello, world!"));

    // init buttons
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLDOWN);
    pinMode(2, INPUT_PULLDOWN);

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
    } else if (digitalRead(1)) {
        intake += 5;
        write = true;
    } else if (digitalRead(2)) {
        write = true;
        if (intake > 4) intake -= 5;
        else if (intake > 0) intake = 0;
        else write = false;
    }
    if (write == true) update();
    delay(150);
}
