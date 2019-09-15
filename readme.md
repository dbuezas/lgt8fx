# How to install

Add the package to the arduino Boards Manager Urls:

- Go to Preferences
  - paste this url in Boards Manager URL: `https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json`
- Go to Tools/Board/Boards manager
  - Type `lgt8fx` in the search box
  - install lgt8fx

Now the boards appear in the IDE and you can also select the clock speed.

<img src="./docs/arduino/boards_manager.png" alt="Boards Manager" width="400"/>
<img src="./docs/arduino/variants.png" alt="Variants" width="400"/>
<img src="./docs/arduino/clock.png" alt="Clock" width="400"/>

32Mhz is twice as fast as a conventional arduino nano!

# Features

- [x] [Fast_IO update](./lgt8f/libraries/lgt328p/examples/lgt8fx8p_fastio/lgt8fx8p_fastio.ino)
- [x] [Automatic prescaler setup](./lgt8f/cores/lgt8f/main.cpp#L126)
- [x] [In Menu support of 1, 2, 4, 8, 16 and 32 Mhz](./lgt8f/boards.txt)
- [x] [Digital Analog Converter](./lgt8f/libraries/lgt328p/examples/dac0_sinus/dac0_sinus.ino)
- [x] [Voltage References](./lgt8f/libraries/lgt328p/examples/adc_i2v56/adc_i2v56.ino) INTERNAL1V024/INTERNAL2V048/INTERNAL4V096/DEFAULT/EXTERNAL (useful for example for analogRead or DAC analogWrite via analogReference(xxx));
- [ ] Analog Comparator
- [x] [Differential Amplifier](./docs/differential-amplifier/readme.md). See this [Example](./lgt8f/libraries/differential_amplifier/examples/all_vs_all/all_vs_all.ino).
- [ ] Computation Accelerator
- [x] [SoftwareSerial @32Mhz](./lgt8f/libraries/SoftwareSerial/SoftwareSerial.cpp#L60) (stable up to 230400 baud)

# Differences to original core [Larduino_HSP v3.6c](https://github.com/Edragon/LGT/tree/master/HSP%20Patch%20File/Larduino_HSP_3.6c/Larduino_HSP_v3.6c)

- Support 32 Mhz and other clock speeds
- Differential Amplifier API
- Better Boards Menu
- Installation via Board Manager Urls
- SoftwareSerial @32Mhz
- FastIO ported from https://github.com/LGTMCU/Larduino_HSP

# Power consumption @ 5v

- 32Mhz 32.6mA
- 16Mhz 27.8mA
- 8Mhz 25.4mA
- 4Mhz 24.3mA
- 2Mhz 23.4mA
- 1Mhz 22.8mA

# Example boards:

## LGT8F328P-LQFP32

<img src="./docs/boards/wavgat_pro_mini.png" alt="Wavgat Pro Mini pseudo-clone" width="200"/>
<img src="./docs/boards/pro_mini_LQFP32.png" alt="Pro Mini style" width="200"/>
<img src="./docs/boards/nano_LQFP32.png" alt="Nano style" width="200"/>
<img src="./docs/boards/uno_LQFP32.jpeg" alt="Uno style" width="200"/>
<img src="./docs/boards/TTGO_XI_8F328P-U_nano_V3.0_LQFP32.png" alt="Wemos Nano style" width="200"/>

## LGT8F328P-SSOP20

<img src="./docs/boards/ssop20.png" alt="Pro Mini simil" width="200"/>

## LGT8F328P-LQFP48

<img src="./docs/boards/uno_LQFP48.png" alt="Uno style" width="200"/>

# Docs & links

- Core is based on [Larduino_HSP v3.6c](https://github.com/Edragon/LGT/tree/master/HSP%20Patch%20File/Larduino_HSP_3.6c/Larduino_HSP_v3.6c) with fastIO backported from https://github.com/LGTMCU/Larduino_HSP
- And inspired from Ralph Bacon's video: https://youtu.be/Myfeqrl3QP0 (Check his channel, he's uploaded a lot of great videos)
- Great place to gather data about this boards: https://github.com/RalphBacon/LGT8F328P-Arduino-Clone-Chip-ATMega328P
- [(Chinese) LGT8FX8P_databook_V1.04](./docs/LGT8FX8P_databook_v1.0.4.ch.pdf)
- [(English) LGT8FX8P_databook_V1.04](./docs/LGT8FX8P_databook_v1.0.4.en.pdf) thanks to [#metallurge](https://github.com/RalphBacon/LGT8F328P-Arduino-Clone-Chip-ATMega328P/issues/2#issuecomment-517952757)
- [Work on the differential amplifier](./docs/differential-amplifier/readme.md)
- "Forbiden tech from China has arrived" https://www.avrfreaks.net/forum/forbiden-tech-china-has-arrived?page=all
- Larduino ISP https://github.com/Edragon/LGT/tree/master/Toolchain/ISP/LarduinoISP-master

# Disclamer

I have no association with Arduino, Logic Green, Atmel or anything. I just wanted to have a convenient way to use these boards and get them to work without hacks at max speed

# Download stats
http://www.somsubhra.com/github-release-stats/?username=dbuezas&repository=lgt8fx

# Thanks

- [#Larduino_HSP](https://github.com/LGTMCU/Larduino_HSP) for doing 99.9999% of the work
- [#RalphBacon](https://github.com/RalphBacon) introducing most of us to the board
- [#dcfusor](https://github.com/dcfusor) for help with fast io backporting
- [#HI-SEBA](https://github.com/HI-SEBA) for help with software serial
- [#dwillmore](https://github.com/dwillmore) for creating the wiki, serial adapter troubleshooting and more examples of boards
