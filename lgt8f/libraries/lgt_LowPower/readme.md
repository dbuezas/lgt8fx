# Low Power library for LogicGreen LGT8F328P

## Table of contents
[Similarities and differences of sleep modes between ATmega328P and LGT8F328P](#similarities-and-differences-of-sleep-modes-between-atmega328p-and-lgt8f328p)  <br> 
[How to use the LowPower library](#the-lgt-lowpower-library) <br> 
[Low-Power library function list for Logicgreen microcontrollers](#low-power-library-function-list-for-logicgreen-microcontrollers)

## Similarities and differences of sleep modes between ATmega328P and LGT8F328P

Except for the Idle mode and the ADC Noise Reduction mode the LogicGreen MCUs sleep modes work completely differently than ATmega328 sleep modes. Even the names are different in the databooks. (DPS0, DPS1, DPS2)

The table sorted by the ATmega328P sleep modes. The most similar sleep modes are in the same rows.
| code |  ATmega328P  |  LGT8F328P   | code | 328P databook name |   |   328D code    | 328D databook name |
| :--: | ------------ | ------------ | :--: | ------------------ | - | :------------: | ------------------ |
| 000  |   **Idle**   |     Idle     | 000  |        Idle        |   |      000       |        Idle        |
| 001  |**ADC Noise<br>Reduction**| ADC Noise<br>Reduction | 001 | ADC Noise<br>Reduction | | 001 | ADC Noise<br>Reduction |
| 010  |**Power-Down**|  Power-Down  | 011  |   Power-down DPS1  |   |      011       |  Power/Off S1 mode |
| 011  | Power-Save **<sup>*</sup>**| - | - |          -         |   |       -        |          -         |
| 110  | **Standby**  |   Standby    | 110  |   Power-down DPS0  |   |      110       |  Power/Off S0 mode |
| 111  |**Extended Standby**| Extended Standby| 010 | Power-save |   |      010       |   Power/Down mode  |
|  -   |       -      |**Deep Sleep**| 111  |   Power-down DPS2  |   |      100       |  **<sup>**</sup>** |

_* Not implemented in LGT LowPower library._ <br> 
_** LGT8F328D has no such mode._

<br>

In the LGT8Fx MCU there are more analog modules than ATmega328. (ADC, AC0, AC1, DAC0, LVD, DAP, IVREF and the internal voltage division circuit) When entering sleep modes, analog modules will not disabled by the MCU and will not even disabled by LowPower class functions. <br>
If the Timer2 was enabled before sleep instruction, it remain enabled by the MCU in all sleep modes except DPS2. <br>
If the watchdog timer was enabled before sleep instruction, it will be enabled in most of the sleep modes except DPS2. <br>

If a level triggered interrupt is used for wake-up from power-down, the required level must be held long enough for the MCU to complete the wake-up to trigger the level interrupt. If the level disappears before the end of the start-up time, the MCU will still wake up, but no interrupt will be generated.

- **If** the triggered interrupt required **level not held long enough** for the MCU to complete the wake-up to trigger the level interrupt, there is a filter circuit that extends the short signal for the internal wake up circuit to avoid waking up without executing interrupt. Set-up of filter circuit can be done via FWKPEN of MCUCR register. The default filter length is 260 microseconds which can be sortened to 32 µs.

Using LGT8F328x MCU the Interrupt Service Routine of the WDT used for timing the sleep period is not available. <br>
<br>
**Power-Down / DPS1 mode** shut down all modules and clocks <br>
**Standy / DPS0 mode** power off all core functions (digital and analog) <br>
**Extended Standby** disable most clocks and switch main clock to rc32k

#### Wake-up sources during sleep
| **LGT8F328P**  |   PC INT  |INT0/1<br>level|INT0/1<br>edge| TWI addr. match|Timer2| WDT |PD level change|
| -------------- | :-------: | :-----------: | :----------: | :------------: | :--: | :-: | :-----------: |
| **Power-Down** |    Yes    |      Yes      |              |       Yes      |  Yes |     |      Yes      |
|  **Standby**   |    Yes    |      Yes      |      Yes     |       Yes      |  Yes | Yes |      Yes      |
|**Ext.standby** |    Yes    |      Yes      |      Yes     |       Yes      |  Yes | Yes |      Yes      |
|**Deep Sleep**  |           |               |              |                |      |     |      Yes      |

| **LGT8F328D**  |   PCINT   |INT0/1<br>level|INT0/1<br>edge| TWI addr. match|Timer2| WDT |PD level change|
| -------------- | :-------: | :-----------: | :----------: | :------------: | :--: | :-: | :-----------: |
| **Power-Down** |           |      Yes      |              |                |      |     |               |
|  **Standby**   |           |      Yes      |              |                |      |     |               |
|**Ext.standby** |    Yes    |      Yes      |      Yes     |       Yes      |      | Yes |               |


<br><br>

## The LGT LowPower library

For ease of use, the names of the ATmega328 sleep modes have been retained in the function names.
[There is a table above about what is the simialirities with ATmega328 sleep modes](#similarities-and-differences-of-sleep-modes-between-atmega328p-and-lgt8f328p)

- For proper sleep period timing, the WDT useing the internal 32 kHz RC oscillator. (LGT8F328D 32 kHz oscillator not calibrated by factory)
- Follow the design guide about pull up resistors, connected and unconnected pin levels to reduce current consumption.
- Turn off all unused modules before entering any powerSleep modes. Only the ADC can be turned off with LowPower class. [Watch the additional disabling functions for internal modules](#additional-disabling-functions-for-internal-modules)
- **WDT ISR not available.**
<br>

Basic usage:
`LowPower.sleep_mode( SLEEP_period, list of devices to turn off );`

### List of Low-Power library functions for Logicgreen microcontrollers.

 _Don't forget to enable module interrupts that wakes up the MCU before calling any LowPower sleep_modes._
 _--Be aware about any interrupt flag bits. E.g. after one analogRead done the ADIF bit of ADCSRA remain set. So the interrupt flag breaks the sleep._

`LowPower.idle(SLEEP_period,ADC_ON,TIMER3_OFF,TIMER2_OFF,TIMER1_OFF,TIMER0_ON,SPI_OFF,USART0_OFF,TWI_OFF,PCIC_OFF,FLASHCTL_ON)`
  --This function stops the code execution core for the specified time and also turn off the listed devices. <br>
  --The main usage of this function when CPU is waiting for an event like FLASH/EEPROM write, or another internal or external interrupt. <br>
  --When Pin change or External interrupts are not in use in Idle mode, the Pin Change and Interrupt module (PCI) clock can be turned off: PCIC_OFF <br>
  The Pin Change and Interrupt module uses the I/O clock which is halted in all sleep modes except Idle mode, like ATmega328 does. <br>
<br>

`LowPower.adcNoiseReduction( SLEEP_period, ADC_ON, TIMER2_OFF )` <br>
  --Putting microcontroller into ADC noise reduction state. <br>
  (Turning off the ADC module is basically removing the purpose of this low power mode.) <br>
<br>

 _--The functions below stops the clocks of peripherial modules so there's no point to left under powered modules during sleep which has no clock, like ADC._

`LowPower.powerExtStandby( SLEEP_period, ADC_OFF, BOD_OFF, TIMER2_OFF )` <br>
  --LGT8F power save sleep mode <br>
  --Wake up sources: Watchdog, External interrupt 0/1, Pin Change Interrupt, Timer2, Two-Wire serial Interface address match, External reset <br>
<br>

`LowPower.powerStandby( SLEEP_period, ADC_OFF, BOD_OFF, TIMER2_OFF )` <br>
  --LGT8F **D**ee**P** **S**leep mode **0** <br>
  --Wake up sources: Watchdog, External interrupt 0/1, Pin Change Interrupt, Timer2, Two-Wire serial Interface address match, External reset <br>
<br>

`LowPower.powerDown( SLEEP_period, ADC_OFF, BOD_OFF )` <br>
  --LGT8F **D**ee**P** **S**leep mode **1** <br>
  --Same as DSP0, but since of all system clock are turned off the INT0, INT1 edge detect interrupts not working.<br>
  --Wake up sources: Watchdog, External level interrupt 0/1, Port D level change, Timer2. <br>

**_The five sleep modes above can be activated for this periods: SLEEP_64MS, SLEEP_128MS, SLEEP_256MS, SLEEP_512MS, SLEEP_1S, SLEEP_2S, SLEEP_4S, SLEEP_8S, SLEEP_16S, SLEEP_32S, SLEEP_FOREVER_**  <br>
<br>

`LowPower.DeepSleep2(SLEEP_period)` <br>
  --LGT8F Deep sleep mode 2 <br>
  --This function completely turn off MCU for the specified time or forever and also turn off the listed devices <br>
  --Wake up sources: Port D level change. Wake up from DPS2 identical with reset. All register and RAM data will loss. <br>
  --Available sleep periods: SLEEP_128MS, SLEEP_256MS, SLEEP_512MS, SLEEP_1S, SLEEP_FOREVER <br>
<br>
<br>

**List of ATmega328P compatibile power switching functions**
| Function | enable | disable |
| - | - | - |
| Turn on/off SPI clock | power_spi_enable() | power_spi_disable() |
| Turn on/off Timer0 clock | power_timer0_enable() | power_timer0_disable() |
| Turn on/off Timer1 clock | power_timer1_enable() | power_timer1_disble() |
| Turn on/off Timer2 clock | power_timer2_enable() | power_timer2_disable() |
| Turn on/off TWI clock | power_twi_enable() | power_twi_disable() |
| Turn on/off USART0 clock | power_usart0_enable() | power_usart0_disable() |
| All of above and below | power_all_enable() | power_all_disable() |

**List of extended power switching functions for LGT MCUs**

| Function | enable | disable |
| - | - | - |
| Turn on/off Timer3 clock | power_timer3_enable() | power_timer3_disable() |
| Turn on/off Pin Change Interrupt module clock | power_pci_enable() | power_pci_disable() |
| Turn on/off Flash controller clock | power_flashctrl_enable() | power_flashctrl_disable() |
| Turn on/off WDT counter clock | power_wdtcc_enable() | power_wdtcc_disable() |
<br>

#### Additional disabling functions for internal modules

(Functions that disable modules are not visible in the table, but you could replace the '\_enable' to '\_disable' in the function names.)
| Function | enable, disable | code |
| - | - | - |
| Turn on/off Low Voltage Detection circuit <br> ( ATmega328P BOD equivalent ) <br> Don't forget to put `noInterrupt()`<br>before this code block | power2_lvd_enable() | uint8_t tmp_power2_lvd_disable = VDTCR & ~(1 << VDTEN); <br> VDTCR = 0x80; <br> VDTCR = tmp_power2_lvd_disable; |
| Turn on/off SPI interface | power2_spi_enable() | `SPCR \|= (uint8_t) (1 << SPE);` |
| Turn on/off receiver+transmitter module | power2_usart0_enable() | `UCSRB0 \|= (uint8_t) (1<<RXEN0)\|(1<<TXEN0);` |
| Turn on/off TWI interface | power2_twi_enable() | `TWCR  \|= (uint8_t) (1 << TWCR)` |
| Turn on/off ADC module | power2_adc_enable() | `ADCSRA \|= (uint8_t) (1 << ADEN)` |
| Turn on/off SWD interface | power2_swd_enable() | MCUSR = 0xFF; <br> MCUSR = 0x3F; |
| **------ LGT8F328P functions ------** | **enable, disable** | **code** |
| Turn on/off Analog comparator 0 | power2_ac0_enable() | `C0SR  &= (uint8_t)~(1 << C0D)` |
| Turn on/off Analog comparator 1 | power2_ac1_enable() | `C1SR  &= (uint8_t)~(1 << C1D)` |
| Turn on/off DAC module | power2_dac_enable() | `DACON \|= (uint8_t) (1 << DACEN)` |
| Turn on/off Differential Amplifier | power2_dap_enable() | `DAPCR \|= (uint8_t) (1 << DAPEN)` |
| Turn on/off Arithmetic Accelerator (uDSC) | power2_uDSC_enable() | `DSCR  \|= (uint8_t) (1 << DSUEN)` |
| **------ LGT8F328D functions ------** | **enable, disable** | **code** |
| Turn on/off Analog comparator 0 | power2_ac0_enable() | `AC0SR &= (uint8_t)~(1 << ACD)` |
| Turn on/off Analog comparator 1 | power2_ac1_enable() | `AC1SR &= (uint8_t)~(1 << ACD)` |
| Turn on/off Op amplifier 0 | power2_opa0_enable() | `OP0CRA \|= (uint8_t) (1 << OPAEN)` |
| Turn on/off Op amplifier 1 | power2_opa1_enable() | `OP1CRA \|= (uint8_t) (1 << OPAEN)` |
