# How to install

Add the package to the arduino Boards Manager Urls:

- Go to Preferences
  - paste this url in Boards Manager URL: `https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json`
- Go to Tools/Board/Boards manager
  - Type `lgt8fx` in the search box
  - install lgt8fx

Now the boards appear in the IDE. You can also select clock speeds, [upload speeds and generic boards with external cystal](./docs/arduino/readme.md).

<img src="./docs/arduino/boards_manager.png" alt="Boards Manager" width="550">
<img src="./docs/arduino/variants.png" alt="Variants" width="550">
<img src="./docs/arduino/clock.png" alt="Clock" width="550">
<img src="./docs/arduino/clock-source.png" alt="External Clock" width="550">

32Mhz is twice as fast as a conventional arduino nano! Actually _even faster_ as many operations take less clock cycles than in the atmega328p. Check out some [benchmarks](https://github.com/dbuezas/lgt8fx/discussions/77)

# Recommended read

Tutorials and details about this board:
[LGT8F328P - LQFP32 Boards • Wolles Elektronikkiste](https://wolles-elektronikkiste.de/en/lgt8f328p-lqfp32-boards)

# Discussions, ideas, questions and show your project

- Explore the discussions: https://github.com/dbuezas/lgt8fx/discussions

# Features

- [x] [Fast_IO update](./lgt8f/libraries/lgt328p/examples/lgt8fx8p_fastio/lgt8fx8p_fastio.ino)
- [x] [In-menu external clock support](./docs/external-clock/readme.md) (by [#seisfeld](https://github.com/seisfeld))
- [x] [In menu generic boards support with 1, 2, 4, 8, 12, 16 and 32 Mhz](./lgt8f/boards.txt)
- [x] [Automatic prescaler setup](./lgt8f/cores/lgt8f/main.cpp#L128)
- [x] [Digital Analog Converter](./lgt8f/libraries/lgt328p/examples/dac0_sinus/dac0_sinus.ino)
- [x] [Voltage References](./lgt8f/libraries/lgt328p/examples/adc_i2v56/adc_i2v56.ino) INTERNAL1V024/INTERNAL2V048/INTERNAL4V096/DEFAULT/EXTERNAL (useful for example for analogRead or DAC analogWrite via analogReference(xxx));
- [ ] Analog Comparator (page 224 of datasheet v1.0.4)
- [x] [Differential Amplifier](./docs/differential-amplifier/readme.md). See this [Example](./lgt8f/libraries/differential_amplifier/examples/all_vs_all/all_vs_all.ino).
- [ ] Computation Accelerator (page 52 of datasheet v1.0.4) [Work by others](https://www.avrfreaks.net/comment/2272366#comment-2272366)
- [x] [SoftwareSerial at any clock speed](https://github.com/dbuezas/lgt8fx/pull/26). Updated implementation without timing tables by [#jg1uaa](https://github.com/jg1uaa)
- [x] [2 to 6 high current 80ma IO pins](https://github.com/dbuezas/lgt8fx/issues/21#issuecomment-657020605) (thanks [#rokweom](https://github.com/rokweom))
- [x] [328p Arduino ISP](https://github.com/dbuezas/lgt8fx/blob/master/lgt8f/libraries/LarduinoISP/readme.md) (from [#brother-yan](https://github.com/brother-yan/LGTISP))
- [x] [SSOP20 328p Support](https://github.com/dbuezas/lgt8fx/pull/16) (by [#LaZsolt](https://github.com/LaZsolt))
- [x] [Accurate delayMicroseconds](https://github.com/dbuezas/lgt8fx/issues/18) (by [#LaZsolt](https://github.com/LaZsolt))
- [x] [Faster Analog Read](https://github.com/dbuezas/lgt8fx/pull/32) (by [#jayzakk](https://github.com/jayzakk))
- [x] [Fixed analogReference](https://github.com/dbuezas/lgt8fx/issues/27) (reported by [#macron0](https://github.com/macron0))
- [x] [Enabled AREF pin as A10 analog input](https://github.com/dbuezas/lgt8fx/pull/36) (by [#jayzakk](https://github.com/jayzakk))
- [x] [Power reduce register definitions](https://github.com/dbuezas/lgt8fx/pull/46) (by [#KooLru](https://github.com/KooLru))
- [x] [Support for interrupts on all timers and comparators](https://github.com/dbuezas/lgt8fx/pull/40) (by [#jayzakk](https://github.com/jayzakk))
- [x] [Arduino EEPROM API](https://github.com/dbuezas/lgt8fx/pull/64) (by [#SuperUserNameMan](https://github.com/SuperUserNameMan)
- [ ] Lock bits. It seems this micro controller doesn't have flash lock bits.

# Timers
This chip has more timers, each with more features than the atmega328p.
Explore, configure and visualize timers for both atmega328p and lgt328p with this online tool [https://github.com/dbuezas/arduino-web-timers](https://github.com/dbuezas/arduino-web-timers)
<img width="800" alt="image" src="https://user-images.githubusercontent.com/777196/221049698-d2a51443-73c8-42d7-8e85-b46f25c42dc7.png">


# Differences to original core [Larduino_HSP v3.6c](https://github.com/Edragon/LGT/tree/master/1-LGT8F328/SDK/Arduino/HSP/HSP%20Patch%20File/Larduino_HSP_3.6c/Larduino_HSP_v3.6c)

- Support 32 Mhz and other clock speeds
- Selectable ADC resolution (Arduino compatibile default is 10 bits)
- Differential Amplifier API
- Better Boards Menu
- Installation via Board Manager Urls
- SoftwareSerial @32Mhz
- FastIO ported from https://github.com/LGTMCU/Larduino_HSP

# Power consumption @ 5v

| Clock | Pro mini style w/o power LED | Pro mini style | Nano style |
| ----- | ---------------------------- | -------------- | ---------- |
| 32MHz | 12.7mA                       | 15.0mA         | 32.6mA     |
| 16MHz | 9.2mA                        | 11.5mA         | 27.8mA     |
| 8MHz  | 7.1mA                        | 9.4mA          | 25.4mA     |
| 4MHz  | 5.9mA                        | 8.2mA          | 23.3mA     |
| 2MHz  | 5.3mA                        | 7.6mA          | 23.4mA     |
| 1MHz  | 5.0mA                        | 7.3mA          | 22.8mA     |

# Example boards:

## *-->LGT8F328P-SSOP20*

#### [Pro Mini Style SSOP20](../../wiki/Pro-Mini-Style---SSOP20)

<img src="./docs/boards/ssop20.jpg" alt="Pro Mini ssop20" width="200"> <img src="./docs/boards/ssop20_back.jpg" alt="Pro Mini ssop20" width="200"> <img src="./docs/boards/ssop20.png" alt="Pro Mini ssop20" width="200">

<img src="./docs/boards/lgt8f328p_pro_mini_ssop20_pinout.png" alt="Nano style" width="800">
Pinout by [wollewald](https://github.com/wollewald)

## *-->LGT8F328P-LQFP32*

#### [WAVGAT Pro Mini Pseudo Clone](../../wiki/WAVGAT-Pro-Mini-Pseudo-Clone)

<img src="./docs/boards/WAVGAT_Pro_Mini_Clone.jpg" alt="Wavgat Pro Mini pseudo-clone" width="200"> <img src="./docs/boards/wavgat_back.jpg" alt="Wavgat Pro Mini pseudo-clone" width="200"> Chip marked as AVGA328P

#### Pro Mini style

<img src="./docs/boards/pro_mini_LQFP32.png" alt="Pro Mini style" width="200">
<img src="./docs/boards/lgt8f328p_pro_mini_qfp32l_pinout.png" alt="Nano style" width="800">

Pinout by [wollewald](https://github.com/wollewald) Visit his Electronic projects blog here https://wolles-elektronikkiste.de/en/

#### [Nano Style](../../wiki/Nano-Style)

<img src="./docs/boards/nano.jpg" alt="Nano style" width="200"> <img src="./docs/boards/nano_back.jpg" alt="Nano style" width="200">

<img src="./docs/boards/nano_LQFP32.png" alt="Nano style" width="200">
<img src="./docs/boards/lgt8f328p_nano_qfp32l_pinout.png" alt="Nano style" width="800">

Pinout by [wollewald](https://github.com/wollewald)

Like this pinout?

[Create](./docs/boards/pinouts) more pinouts for the other boards!

#### [Wemos TTGO XI](../../wiki/Wemos-TTGO-XI)

<img src="./docs/boards/wemos.jpg" alt="Wemos Nano style" width="200"> <img src="./docs/boards/wemos back.jpg" alt="Wemos Nano style" width="200">

<img src="./docs/boards/TTGO_XI_8F328P-U_nano_V3.0_LQFP32.png" alt="Wemos Nano style" width="200">

#### Uno Style

<img src="./docs/boards/uno_LQFP32.jpeg" alt="Uno style" width="200">

## *-->LGT8F328D-LQFP32*

#### LARDUINO Uno style

<img src="./docs/boards/larduino_uno-front.jpg" alt="Larduino Uno style" width="200"> <img src="./docs/boards/larduino_uno-back.jpg" alt="Larduino Uno style" width="210">

## *-->LGT8F328P-LQFP48*

#### Nano Style

<img src="https://user-images.githubusercontent.com/777196/232343332-d8d93c6b-80b1-4865-9910-bfa864a91081.png" alt="Nano style" width="200">
<img src="./docs/boards/lgt8f328p_nano_qfp48l_pinout.png" alt="Nano style" width="800">
Pinout by [wollewald](https://github.com/wollewald)

#### Uno Style

<img src="./docs/boards/uno_LQFP48.png" alt="Uno style" width="200">

# Docs & links

- Check the [Wiki](https://github.com/dbuezas/lgt8fx/wiki) for more content by contribuitors
- And you may also find something in the closed [Issues](https://github.com/dbuezas/lgt8fx/issues?utf8=%E2%9C%93&q=is%3Aissue)
- Core is based on [Larduino_HSP v3.6c](https://github.com/Edragon/LGT/tree/master/1-LGT8F328/SDK/Arduino/HSP/HSP%20Patch%20File/Larduino_HSP_3.6c/Larduino_HSP_v3.6c) with fastIO backported from https://github.com/LGTMCU/Larduino_HSP
- And inspired from Ralph Bacon's video: https://youtu.be/Myfeqrl3QP0 (Check his channel, he's uploaded a lot of great videos)
- Great place to gather data about this boards: https://github.com/RalphBacon/LGT8F328P-Arduino-Clone-Chip-ATMega328P
- Datasheet [(Chinese) LGT8FX8P_databook_V1.04](./docs/LGT8FX8P_databook_v1.0.4.ch.pdf)
- Datasheet [(English) LGT8FX8P_databook_V1.04](./docs/LGT8FX8P_databook_v1.0.4.en.pdf) thanks to [#metallurge](https://github.com/RalphBacon/LGT8F328P-Arduino-Clone-Chip-ATMega328P/issues/2#issuecomment-517952757)
- Datasheet [(English) LGT8FX8P_databook_V1.05](https://github.com/watterott/LGT8F328P-Testing/raw/master/LGT8FX8P_databook_v1.0.5-English.pdf) By [Watterrott](https://github.com/watterott/LGT8F328P-Testing)
- [Instruction set clk vs avr](https://docs.google.com/spreadsheets/d/1EzwMkWOIMNDqnjpbzuchsLx5Zq_j927tvAPgvmSuP6M/edit?usp=sharing) By unknown, claim if you are the author
- [Work on the differential amplifier](./docs/differential-amplifier/readme.md)
- "Forbiden tech from China has arrived" https://www.avrfreaks.net/forum/forbiden-tech-china-has-arrived?page=all
- Larduino ISP for 328d https://github.com/Edragon/LGT/tree/master/Toolchain/LarduinoISP-master
- https://www.eevblog.com/forum/projects/anyone-here-interested-in-the-logic-green-avrs-lgt8f328p/
- http://coultersmithing.com/forums/viewtopic.php?f=6&t=1149
- LGT SDK Builder
  - "very useful during development, hopefully others can find it helpful too." @sengit
  - Document page : http://www.lgtic.com/2017/11/11/lgtsdk_builder/
  - Download link : http://www.lgtic.com/upload/tools/lgtsdk_builder/LGTSDK_Builder_latest.rar

PS: Just want to say thanks for this git, helped me a lot!
# Disclamer

I have no association with Arduino, Logic Green, Atmel or anything. I just wanted to have a convenient way to use these boards and get them to work without hacks at max speed

# Automated Releases

Each time a PR is merged, the release action will be triggered.

The following steps are executed
* Generate the release artefacts (the lgt8f-1.0.x.zip file)
* Update the `package_lgt8fx_index.json` file with the new version
* Commit and push those changes to the repository
* Generate a release with the artefact.

PRs that only touch the `readme.md` file, or the `/docs` folder will NOT trigger a release.

The action can be found here: https://github.com/dbuezas/lgt8fx/blob/master/.github/workflows/release.yml

# Download stats

https://tooomm.github.io/github-release-stats/?username=dbuezas&repository=lgt8fx

# Thanks

- [#Larduino_HSP](https://github.com/LGTMCU/Larduino_HSP) for doing 90% of the work
- [#RalphBacon](https://github.com/RalphBacon) introducing most of us to the board
- [#dcfusor](https://github.com/dcfusor) for help with fast io backporting
- [#HI-SEBA](https://github.com/HI-SEBA) for help with software serial
- [#dwillmore](https://github.com/dwillmore) for creating the wiki, serial adapter troubleshooting, more examples of boards and wemos-TTGO-XI board support
- [#seisfeld](https://github.com/seisfeld) for adding in-menu support for an external oscillator
- [#jg1uaa](https://github.com/jg1uaa) for the updated Software Serial without timing tables and missing methods
- [#LaZsolt](https://github.com/LaZsolt) for adding SSOP20 lgt8f328p support and accurate delayMicroseconds
- [#jayzakk](https://github.com/jayzakk) for fixing the ADC prescaler for faster analog reads
- [#wollewald](https://github.com/wollewald) for all the pinout diagrms 

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/dbuezas)
