adapted: https://github.com/dbuezas/lgt8fx/
source: https://github.com/brother-yan/LGTISP

# LGTISP
LGT8Fx8P ISP download protocol implementation

## introduction
This is an implementation of LGT8Fx8P ISP download protocol. You can make arduino board as ISP for programing LGT8Fx8P.

## usage
1. Make a LGTISP:
   0. Select SERIAL_RX_BUFFER_SIZE to 250 in Menu: Tools/Arduino as ISP/SERIAL_RX_BUFFER_SIZE to 250
   1. Burn LGTISP into arduino board.
   1.1 Select Default in Menu: Tools/Arduino as ISP/Default
   2. Short `RESET` pin and `VCC` pin of arduino board to avoid bootloader executing.
   3. The arduino board becomes a LGTISP now! Connect `SWD` and `SWC`. `RST` can be either connect or not.
      | Arduino Nano || LGT |
      |:-:|:-:|:-:|
      | D13 | -> | SWC |
      | D12 | -> | SWD |
      | D10 | -> | RST |
2. Burn bootloader into LGT8Fx8P:
   1. install Larduino.
   2. change **Board** to LGT8Fx8P.
   3. change **Programmer** to **AVR ISP**.
   4. click **Burn Bootloader**!

## advanced usage
Explore yourself!

## author
brother_yan

## reference
[LarduinoISP](https://github.com/LGTMCU/LarduinoISP)  
[ISP4LGT8F328P](https://github.com/nicechao/ISP4LGT8F328P)
