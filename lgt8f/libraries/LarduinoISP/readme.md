adapted: https://github.com/dbuezas/lgt8fx/

source: https://github.com/brother-yan/LGTISP

# LGTISP

LGT8Fx8P ISP download protocol implementation

## introduction

This is an implementation of LGT8Fx8P ISP download protocol. You can make arduino board as ISP for programing LGT8Fx8P.

## usage

1. Prepare a board to use as Programmer:
   1. In the menu select: `Tools`/`Arduino as ISP`/`SERIAL_RX_BUFFER_SIZE to 250`
   2. Open `File`/`Examples`/`LarduinoISP`/`LarduinoISP`
   3. Upload the sketch.
   4. Go back to normal serial buffer size: `Tools`/`Arduino as ISP`/`Default`
   5. Put a capacitor (e.g 10uF) between the `RESET` pin and `VCC` pin of ISP board to resets while uploading. Alternatively, short `RESET` and `VCC`, but make sure to NOT press the reset button in the board.
   6. Congratulations, you have a LGTISP Programmer now!
2. Connect the programmer to the target board
   1. Connect `SWD` and `SWC`. `RST` can be either connect or not.
      | Arduino Nano || LGT |
      |:-:|:-:|:-:|
      | D13 | -> | SWC |
      | D12 | -> | SWD |
      | D10 | -> | RST |
3. Burn bootloader into LGT8Fx8P:
   1. change **Programmer** to **AVR ISP**.
   2. click **Burn Bootloader** or upload sketches directly through the menu: **Sketch/Upload using programmer**
   3. Now the target board can receive sketches via serial
4. Burn sketches directly through the programmer:
   1. Open the desired sketch
   2. Configure the board from `Tools` as normal
   3. Change **Programmer** to **AVR ISP**.
   4. With everything wired, in the menu go to `Sketch`/`Upload using Programmer` (alternatively hold shift while clicking the upload button)
   5. Congratulations! the sketch is now uploaded without a bootloader! Uploading via serial should't be possible anymore on that board (unless you Burn the bootloader again)

## Guide

[rickygai](https://github.com/rickygai) created a very detailed guide [here](https://github.com/rickygai/arduino/blob/main/LGT8F328P.pdf)

## author

ISP code: brother_yan
This page: dbuezas

## reference

[LarduinoISP](https://github.com/LGTMCU/LarduinoISP)  
[ISP4LGT8F328P](https://github.com/nicechao/ISP4LGT8F328P)
