#ifndef LowPower_h
#define LowPower_h

enum period_t
{
    SLEEP_64MS,
    SLEEP_128MS,
    SLEEP_256MS,
    SLEEP_512MS,
    SLEEP_1S,
    SLEEP_2S,
    SLEEP_4S,
    SLEEP_8S,
    SLEEP_16S,
    SLEEP_32S,
    SLEEP_FOREVER = 0xFF
};

enum hfosc_t
{
    HFOSC_OFF,
    HFOSC_ON
};

enum bod_t
{
    BOD_OFF,
    BOD_ON
};

enum adc_t
{
    ADC_OFF,
    ADC_ON
};

enum timer5_t
{
    TIMER5_OFF,
    TIMER5_ON
};

enum timer4_t
{
    TIMER4_OFF,
    TIMER4_ON
};

enum timer3_t
{
    TIMER3_OFF,
    TIMER3_ON
};

enum timer2_t
{
    TIMER2_OFF,
    TIMER2_ON
};

enum timer1_t
{
    TIMER1_OFF,
    TIMER1_ON
};

enum timer0_t
{
    TIMER0_OFF,
    TIMER0_ON
};

enum spi_t
{
    SPI_OFF,
    SPI_ON
};

enum usart0_t
{
    USART0_OFF,
    USART0_ON
};

enum usart1_t
{
    USART1_OFF,
    USART1_ON
};

enum usart2_t
{
    USART2_OFF,
    USART2_ON
};

enum usart3_t
{
    USART3_OFF,
    USART3_ON
};

enum twi_t
{
    TWI_OFF,
    TWI_ON
};

enum usb_t
{
    USB_OFF,
    USB_ON
};

enum idle_t
{
    IDLE_0,
    IDLE_1,
    IDLE_2
};

enum pcic_t
{
    PCIC_OFF,
    PCIC_ON
};

enum flashctl_t
{
    FLASHCTL_OFF,
    FLASHCTL_ON
};

enum wdtcc_t
{
    WDTCC_OFF,
    WDTCC_ON
};

#if defined(__LGT8F__)

#undef  SLEEP_MODE_PWR_SAVE
#undef  SLEEP_MODE_EXT_STANDBY
#define SLEEP_MODE_EXT_STANDBY (0x02<<1)
#undef  SLEEP_MODE_PWR_DOWN
#define SLEEP_MODE_PWR_DOWN    (0x03<<1)
#define SLEEP_MODE_DPS1        (0x03<<1)
#define SLEEP_MODE_DPS0        (0x06<<1)
#if defined(__LGT8FX8E__)
  #define SLEEP_MODE_DPS2        (0x04<<1)
#endif

#endif

class LowPowerClass
{
    public:
        #if defined (__AVR__)

            #if (defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega88__)) && ! defined(__LGT8F__)
                void    idle(period_t period, adc_t adc, timer2_t timer2,
                             timer1_t timer1, timer0_t timer0, spi_t spi,
                             usart0_t usart0, twi_t twi);
                void    adcNoiseReduction(period_t period, adc_t adc, timer2_t timer2) __attribute__((optimize("-O1")));
                void    powerDown        (period_t period, adc_t adc, bod_t bod) __attribute__((optimize("-O1")));
                void    powerSave        (period_t period, adc_t adc, bod_t bod, timer2_t timer2) __attribute__((optimize("-O1")));
                void    powerStandby     (period_t period, adc_t adc, bod_t bod) __attribute__((optimize("-O1")));
                void    powerExtStandby  (period_t period, adc_t adc, bod_t bod, timer2_t timer2) __attribute__((optimize("-O1")));
            #elif defined (__LGT8FX8E__)
                void    idle(period_t period, adc_t adc, timer2_t timer2,
                             timer1_t timer1, timer0_t timer0, spi_t spi,
                             usart0_t usart0, twi_t twi, pcic_t pcic, flashctl_t flasctl);
                void    adcNoiseReduction(period_t period, adc_t adc, timer2_t timer2);
                void    powerDown        (period_t period, adc_t adc, bod_t bod);
                void    powerStandby     (period_t period, adc_t adc, bod_t bod, timer2_t timer2);
                void    powerExtStandby  (period_t period, adc_t adc, bod_t bod, timer2_t timer2);
            #elif defined (__LGT8FX8P__)
                void    idle(period_t period, adc_t adc, timer3_t timer3, timer2_t timer2,
                             timer1_t timer1, timer0_t timer0, spi_t spi,
                             usart0_t usart0, twi_t twi, pcic_t pcic, flashctl_t flasctl);
                void    adcNoiseReduction(period_t period, adc_t adc, timer2_t timer2);
                void    powerDown        (period_t period, adc_t adc, bod_t bod);
                void    powerStandby     (period_t period, adc_t adc, bod_t bod, timer2_t timer2);
                void    powerExtStandby  (period_t period, adc_t adc, bod_t bod, timer2_t timer2);
                void    deepSleep2       (period_t period);
            #else
                #error "Please ensure chosen MCU is either LGT8F328P or LGT8F328D."
            #endif

        #else

            #error "Processor architecture is not supported."

        #endif
};

extern LowPowerClass LowPower;


#if defined(__LGT8F__) && ! defined(s_unlockWrite)

  #if defined(__LGT8FX8P__)
  
      #define s_unlockWrite(sfr,val)   \
	    do {                           \
          uint8_t uwtmp = val;         \
          if (&sfr == &PMX1)           \
            { PMX0 = 0x80; }           \
          else                         \
            { _SFR_BYTE(sfr) = 0x80; } \
          _SFR_BYTE(sfr) = uwtmp;      \
        } while (0)

  #else
             // LGT8F238D
      #define s_unlockWrite(sfr,val)   \
        do {                           \
          uint8_t  uwtmp = val;        \
          _SFR_BYTE(sfr) = 0x80;       \
          _SFR_BYTE(sfr) = uwtmp;      \
        } while (0)

  #endif

  #define unlockWrite(r,v)      \
    do {                        \
      uint8_t uw_o_sreg = SREG; \
      cli();                    \
      s_unlockWrite(r,v);       \
      SREG = uw_o_sreg;         \
    } while (0)

#endif

#endif

