/*******************************************************************************
// Copyright © 2025 Zach Butler
// Use of this software is governed by the Business Source License v1.1 (BUSL)
// included in the LICENSE file. The license must be kept with this code.
*******************************************************************************/

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_ST7789.h> // display driver
#include <SPI.h>

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello, world!"));

    // turn on LCD backlight
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
    delay(10);

    // initialize ST7789 240x135 TFT LCD display
    display.init(135, 240);
    display.setRotation(3);
    display.setTextWrap(false);
    display.fillScreen(ST77XX_BLACK);

    // print example text
    display.setCursor(15, 10);
    display.setTextColor(ST77XX_BLUE);
    display.setTextSize(6);
    display.println("400 mL");
}

void loop() {
    delay(1000);
}
