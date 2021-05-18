adapted: https://github.com/dbuezas/lgt8fx/

source: https://github.com/brother-yan/LGTISP

# LGTISP

LGT8Fx8P ISP download protocol implementation

## introduction

This is an implementation of LGT8Fx8P ISP download protocol. You can make arduino board as ISP for programing LGT8Fx8P.

## usage

1. Make a LGTISP:
   1. Select SERIAL_RX_BUFFER_SIZE to 250 in Menu: Tools/Arduino as ISP/SERIAL_RX_BUFFER_SIZE to 250
   2. Burn LGTISP into another LGT328p board.
   3. Select Default in Menu: Tools/Arduino as ISP/Default
   4. Short `RESET` pin and `VCC` pin of ISP board to avoid bootloader executing.
   5. The board becomes a LGTISP now! Connect `SWD` and `SWC`. `RST` can be either connect or not.
      | Arduino Nano || LGT |
      |:-:|:-:|:-:|
      | D13 | -> | SWC |
      | D12 | -> | SWD |
      | D10 | -> | RST |
2. Burn bootloader into LGT8Fx8P:
   1. change **Programmer** to **AVR ISP**.
   2. click **Burn Bootloader** or upload sketches directly through the menu: **Sketch/Upload using programmer**

## Guide

[rickygai](https://github.com/rickygai) created a very detailed guide [here](https://github.com/rickygai/arduino/blob/main/LGT8F328P.pdf)

## author

brother_yan

## reference

[LarduinoISP](https://github.com/LGTMCU/LarduinoISP)  
[ISP4LGT8F328P](https://github.com/nicechao/ISP4LGT8F328P)
