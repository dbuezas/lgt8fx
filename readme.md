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

- [x] PWM & Timers update
- [x] Fast_IO update
- [x] Automatic prescaler setup
- [x] In Menu support of 1, 2, 4, 8, 16 and 32 Mhz
- [x] Digital Analog Converter
- [x] Voltage References INTERNAL1V024/INTERNAL2V048/INTERNAL4V096/DEFAULT/EXTERNAL (useful for example for analogRead or DAC analogWrite via analogReference(xxx));
- [ ] Analog Comparator
- [ ] [Differential Amplifier](./docs/differential-amplifier/readme.md)
- [ ] Computation Accelerator

# Power consumption

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

## LGT8F328P-SSOP20

<img src="./docs/boards/ssop20.png" alt="Pro Mini simil" width="200"/>

## LGT8F328P-LQFP48

<img src="./docs/boards/uno_LQFP48.png" alt="Uno style" width="200"/>

# Docs & links

- Core is based on: https://github.com/LGTMCU/Larduino_HSP
- Great place to gather data about this boards: https://github.com/RalphBacon/LGT8F328P-Arduino-Clone-Chip-ATMega328P
- [(Chinese) LGT8FX8P_databook_V1.04](./docs/LGT8FX8P_databook_v1.0.4.ch.pdf)
- [(English) LGT8FX8P_databook_V1.04](./docs/LGT8FX8P_databook_v1.0.4.en.pdf)
- [Work on the differential amplifier](./docs/differential-amplifier/readme.md)

# Disclamer

I have no association with Arduino, Logic Green, Atmel or anything. I just wanted to have a convenient way to use these boards and get them to work without hacks at max speed
