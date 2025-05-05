# Arnaout
Arnaout functional prototype on the [Adafruit ESP32-S3 Reverse TFT Feather](https://www.adafruit.com/product/5691).

> [!CAUTION]
> This project is copyrighted and released under the Business Source License v1.1 (BUSL).  
> See [`./LICENSE`](./LICENSE) for terms.

> [!TIP]
> The subfolder is an Arduino IDE impetus.

<!-- contents box begin -->
<table>
<tr/>
<tr>
<td>
<p/>
<div align="center">
<b>Contents</b>
</div>
<p/>
<!-- contents markdown begin -->

1. [Prerequisites](#prerequisites)
1. [Setup](#setup)
1. [Flashing](#flashing)
1. [License](#license)
1. [See Also](#see-also)

<!-- contents markdown end -->
<p/>
</td>
</tr>
</table>
<!-- contents box end -->

## Prerequisites
Your Linux user account must be in the `dialout` user group to flash the board over the virtual serial port. If you are not already, add yourself to this group...
```bash
sudo usermod -a -G dialout $USER
```
...then restart for the changes to take effect.

## Setup
This is an Arduino-compatible board.
1. Install the [Arduino IDEv2](https://github.com/arduino/arduino-ide).
    ```bash
    flatpak install -y cc.arduino.IDE2
    ```
1. Add the ESP32-S3 board support package (BSP).
    1. Arduino IDEv2 > Tools > Board > Board Manager.
    1. Search "esp32".
    1. Install `esp32` by Espressif Systems.
1. Add the Adafruit ST7789 library for the little display.
    1. Arduino IDEv2 > Tools > Manage Libraries...
    1. Search "Adafruit_ST7789".
    1. Install `Adafruit ST7735 and ST7789` by Adafruit with dependencies.
1. Add the Adafruit RA8875 for the big display.
    1. Arduino IDEv2 > Tools > Manage Libraries...
    1. Search "RA8875".
    1. Install `Adafruit RA8875` by Adafruit with dependencies.

## Flashing
Plug the board into your computer and open the Arduino IDE. Then:
1. Verify the correct board is set.  
    Tools > Board > esp32 > Adafruit Feather ESP32-S3 Reverse TFT
1. Verify the correct port is set.  
    Tools > Port > `/dev/ttyACM0`, or whatever serial port your board was assigned.
1. Enter download mode.  
    Hold down the `D0` button on the board, press the `Reset` button, then release `D0`.
1. Program the board.  
    You can hit the right arrow button in the top-left of the Arduino IDE, or use Sketch > Upload.

The board [will](https://github.com/espressif/arduino-esp32/issues/6762) reset automatically running the new code.

## License
The entire contents of this repo, including this `README.md`, are copyrighted and released under the Business Source License version 1.1 (BUSL) with the following license parameters:
- Additional Use Grant: None.
- Change Date: 4 years from the timestamp of each commit.
    - Each commit in this repository shall be considered a unique version of the
      Licensed Work under the Business Source License (BUSL).
- Change License: MIT
    - MIT is GPLv2- and GPLv3-compatible.
    - See [`./CHANGE_LICENSE`](./CHANGE_LICENSE) for a copy of the MIT license.

See the [`./LICENSE`](./LICENSE) file for more information.

## See Also
Here are some links I found helpful.
- Adafruit ESP32-S3 Reverse TFT Feather
    - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) on GitHub
    - [Adafruit ST7735 and ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library) on GitHub
    - [Guide](https://learn.adafruit.com/esp32-s3-reverse-tft-feather)
    - [Store Page](https://www.adafruit.com/product/5691)
- Adafruit RA8875
    - [Guide](https://learn.adafruit.com/ra8875-touch-display-driver-board/arduino)
- Arduino
    - [Arduino IDEv2](https://github.com/arduino/arduino-ide)
    - [ESP32 Core](https://github.com/arduino/arduino-esp32)
- Thingverse
    1. [Tiny Tester](https://www.thingiverse.com/thing:2253091) - a fast test print
    2. [All-In-One 3D Printer Test](https://www.thingiverse.com/thing:2975429) - micro

Adafruit invests time and resources providing open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

***
> **_Legal Notice_**  
> This repo contains assets created in collaboration with a large language model, machine learning algorithm, or weak artificial intelligence (AI). This notice is required in some countries.

Copyright © 2025 Zach Butler
