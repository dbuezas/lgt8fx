# LGT MCUs in Arduino boards

LGT8F88A, LGT8Fx8D and LGT8Fx8P main differences

<br>

| - | - | LGT8F88A | LGT8Fx8D | LGT8Fx8P |
| :-----------: | :----: | :-------: | :---------: | :--------:|
| Progmem | KB | 8 | 4/8/16/32 | 8/16/32 |
| RAM | KB | 1 | 0.5/1/1/2 | 1/1/2 |
| Data Flash (EEPROM) | KB | 504 byte | 1/2/4/progmem shared | 2/4/progmem shared |
| DAC (8 bit) |  | - | 2 | 1 |
| Internal Reference | V | 1.25/2.56 | 1.25/2.56 | 1.024/2.048/4.096 |
| Analog Comparators (AC) |  | 2 | 2 | 2 |
| AC resolution | mV | 6 | 12 | 10 |
| DAP Differential Amplifier | gain | 7.5/15/22.5/30 | - | 1/8/16/32 |
| ACD | bit | 10 | 12 | 12 |
| Max sysclk | MHz | 32 | 32 | 32 |
| Extras |  | Capacitive touch button controller, <br> Thermal sensor | 2x Operational Amplifier <br> fixed gain = +12 or -11 | DSC (16 bit arithmetic), <br> SPI buffer, SPI 2 wire input mode |
| Package |  | QFP32, SSOP28 | QFP32, SSOP28, SSOP24, SSOP20 | QFP48, QFP32, SSOP20 |

More docs: <br>
https://github.com/Edragon/LGT/tree/master/1-LGT8F328/DOCS <br>
https://github.com/Upcycle-Electronics/LGT8Fx-Datasheet/tree/master/LGT-ArchivedStuffDredgedFromTheInternet <br>
https://github.com/watterott/LGT8F328P-Testing/ <br>
