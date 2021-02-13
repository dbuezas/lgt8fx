/*
**
** Copyright (c) 2013,  LogicGreen techologies
** All rights reserved.
**
** project      : LGT8F328PBSPPrj
** filename     : lgt8f328p_spec.h
** version      : 1.1
** date         : Feb 01, 2017

VERSION HISTORY:
----------------
Version         : 1.1
Date            : Feb 14, 2021
Revised by      : github.com/LaZsolt
Description     : volatile char * converted to Arduino style _SFR_IO and _SFR_MEM
----------------
Version         : 1.0
Date            : Feb 01, 2017
Revised by      : LogicGreen Software Group
Description     : Original version.
**
*/

#ifndef _LGT8F328P_SPEC_H_
#define _LGT8F328P_SPEC_H_

//----------------------------------------------------------------
// DSU (Digital Signal Co-processor) Register Definition
//----------------------------------------------------------------
#define DSCR    _SFR_IO8(0x00)
#define DSUEN   7
#define DSMM    6
#define DSD1    5
#define DSD0    4
#define DSN     2
#define DSZ     1
#define DSC     0

#define DSIR    _SFR_IO8(0x01)

                // 16 bit access IOs
#define DSSD    _SFR_IO8(0x02)
#define DSDX    _SFR_IO8(0x10)
#define DSDY    _SFR_IO8(0x11)
#define DSAL    _SFR_IO8(0x38)
#define DSAH    _SFR_IO8(0x39)

//----------------------------------------------------------------
// GPIO Group C Register Definition
//----------------------------------------------------------------
#define PC7     7
#define PINC7   7
#define DDC7    7
#define PORTC7  7

//----------------------------------------------------------------
// GPIO Group E Register Definition
//----------------------------------------------------------------
#define IPINE   _SFR_IO8(0x0C)
#define IDDRE   _SFR_IO8(0x0D)
#define IPORTE  _SFR_IO8(0x0E)

#define EPINE   _SFR_MEM8(0xA7)
#define EDDRE   _SFR_MEM8(0xA8)
#define EPORTE  _SFR_MEM8(0xA9)

//#define USE_EIO_EREG

#ifdef USE_EIO_EREG
#define PINE    EPINE
#define DDRE    EDDRE
#define PORTE   EPORTE
#else
#define PINE    IPINE
#define DDRE    IDDRE
#define PORTE   IPORTE
#endif

#define PINE0   0
#define PINE1   1
#define PINE2   2
#define PINE3   3
#define PINE4   4
#define PINE5   5
#define PINE6   6
#define PINE7   7

#define DDE0    0
#define DDE1    1
#define DDE2    2
#define DDE3    3
#define DDE4    4
#define DDE5    5
#define DDE6    6
#define DDE7    7

#define PORTE0  0
#define PORTE1  1
#define PORTE2  2
#define PORTE3  3
#define PORTE4  4
#define PORTE5  5
#define PORTE6  6
#define PORTE7  7

                // Port E pullup control?
#define OCPUE   _SFR_MEM8(0xE1)

#define PE7     7
#define PE6     6
#define PE5     5
#define PE4     4
#define PE3     3
#define PE2     2
#define PE1     1
#define PE0     0

//----------------------------------------------------------------
// GPIO Group F Register Definition
//----------------------------------------------------------------
#define PINF  _SFR_IO8(0x12)
#define PINF0   0
#define PINF1   1
#define PINF2   2
#define PINF3   3
#define PINF4   4
#define PINF5   5
#define PINF6   6
#define PINF7   7

#define DDRF    _SFR_IO8(0x13)
#define DDF0    0
#define DDF1    1
#define DDF2    2
#define DDF3    3
#define DDF4    4
#define DDF5    5
#define DDF6    6
#define DDF7    7

#define PORTF   _SFR_IO8(0x14)
#define PORTF0  0
#define PORTF1  1
#define PORTF2  2
#define PORTF3  3
#define PORTF4  4
#define PORTF5  5
#define PORTF6  6
#define PORTF7  7

#define PF7     7
#define PF6     6
#define PF5     5
#define PF4     4
#define PF3     3
#define PF2     2
#define PF1     1
#define PF0     0

//----------------------------------------------------------------
// SPI (Serial Periphral Interface) Register Definition
//----------------------------------------------------------------
#define SPFR  _SFR_IO8(0x19)
#define RDFULL  7
#define RDEMPT  6
#define RDPTR1  5
#define RDPTR0  4
#define WRFULL  3
#define WREMPT  2
#define WRPTR1  1
#define WRPTR0  0

// Bits of SPSR
#define DUAL    2

//----------------------------------------------------------------
// AC0 (Analog Comparator 0) Register Definition
//----------------------------------------------------------------
#define C0SR    _SFR_IO8(0x30)
#define C0D     7
#define C0BG    6
#define C0O     5
#define C0I     4
#define C0IE    3
#define C0IC    2
#define C0IS1   1
#define C0IS0   0

#define C0XR    _SFR_IO8(0x31)
#define C0OE    6
#define C0HSYE  5
#define C0PS0   4
#define C0WKE   3
#define C0FEN   2
#define C0FS1   1
#define C0FS0   0

#define C0TR    _SFR_IO8(0x32)

//----------------------------------------------------------------
// AC1 (Analog Comparator 1) Register Definition
//----------------------------------------------------------------
#define C1SR    _SFR_IO8(0x0F)
#define C1D     7
#define C1BG    6
#define C1O     5
#define C1I     4
#define C1IE    3
#define C1IC    2
#define C1IS1   1
#define C1IS0   0

#define C1XR    _SFR_IO8(0x1A)
#define C1OE    6
#define C1HSYE  5
#define C1PS0   4
#define C1WKE   3
#define C1FEN   2
#define C1FS1   1
#define C1FS0   0

#define C1TR    _SFR_IO8(0x3B)

//----------------------------------------------------------------
// EEP (E2PROM Controller) Register Definition
//----------------------------------------------------------------
#define E2PDL   _SFR_IO8(0x20)
#define E2PDH   _SFR_IO8(0x3A)

#define E2PD0   _SFR_IO8(0x20)
#define E2PD1   _SFR_IO8(0x3A)
#define E2PD2   _SFR_IO8(0x37)
#define E2PD3   _SFR_IO8(0x3C)

#define ECCR    _SFR_IO8(0x36)
#define ECS     0
#define CP0     2
#define CP1     3
#define SWM     4
#define ERN     5
#define EEN     6
#define EWEN    7

//----------------------------------------------------------------
// TC0 (Timer Counter 0) Register Definition
//----------------------------------------------------------------
#define TCCR0C  _SFR_IO8(0x29)
#define DSX0    _SFR_IO8(0x29)
#define DSX07   7
#define DSX06   6
#define DSX05   5
#define DSX04   4
#define DSX01   1
#define DSX00   0

#define DTR0    _SFR_IO8(0x2F)

// Bits of TCCR0B
#define OC0AS   5
#define DTEN0   4

// Bits of TIFR0
#define OC0A    7
#define OC0B    6

//----------------------------------------------------------------
// TC1 (Timer Counter 1) Register Definition
//----------------------------------------------------------------
#define TCCR1D  _SFR_MEM8(0x83)
#define DSX17   7
#define DSX16   6
#define DSX15   5
#define DSX14   4
#define DSX11   1
#define DSX10   0

#define DTR1    _SFR_MEM16(0x8C)
#define DTR1L   _SFR_MEM8(0x8C)
#define DTR1H   _SFR_MEM8(0x8D)

#define PSSR    _SFR_MEM8(0xE2)
#define PSS1    7
#define PSR1    0

#define TCKSR   _SFR_MEM8(0xEC)
#define TCKCSR  _SFR_MEM8(0xEC)
#define F2XEN   6
#define TC2XF1  5
#define TC2XF0  4
#define TC2XS1  1
#define TC2XS0  0

// Bits of TCCR1C
#define DOC1B   5
#define DOC1A   4
#define DTEN1   3

// Bits of TIFR1
#define OC1A    7
#define OC1B    6

//----------------------------------------------------------------
// TC2 (Timer Counter 2) Register Definition
//----------------------------------------------------------------

// Bits of ASSR
#define INTCK   7

// Bits of TIFR2
#define OC2A    7
#define OC2B    6

//----------------------------------------------------------------
// TC3 (Timer Counter 3) Register Definition
//----------------------------------------------------------------
#define TIFR3   _SFR_IO8(0x18)
#define OC3A    7
#define OC3B    6
#define ICF3    5
#define OCF3C   3
#define OCF3B   2
#define OCF3A   1
#define TOV3    0

#define TIMSK3  _SFR_MEM8(0x71)
#define ICIE3   5
#define OCIE3C  3
#define OCIE3B  2
#define OCIE3A  1
#define TOIE3   0

#define TCCR3A  _SFR_MEM8(0x90)
#define COM3A1  7
#define COM3A0  6
#define COM3B1  5
#define COM3B0  4
#define COM3C1  3
#define COM3C0  2
#define WGM31   1
#define WGM30   0

#define TCCR3B  _SFR_MEM8(0x91)
#define ICNC3   7
#define ICES3   6
#define WGM33   4
#define WGM32   3
#define CS32    2
#define CS31    1
#define CS30    0

#define TCCR3C  _SFR_MEM8(0x92)
#define FOC3A   7
#define FOC3B   6
#define DOC31   5
#define DOC30   4
#define DTEN3   3
#define DOC32   1
#define FOC3C   0

#define TCCR3D  _SFR_MEM8(0x93)
#define DSX37   7
#define DSX36   6
#define DSX35   5
#define DSX34   4
#define DSX31   1
#define DSX30   0

#define TCNT3   _SFR_MEM16(0x94)
#define TCNT3L  _SFR_MEM8(0x94)
#define TCNT3H  _SFR_MEM8(0x95)

#define ICR3    _SFR_MEM16(0x96)
#define ICR3L   _SFR_MEM8(0x96)
#define ICR3H   _SFR_MEM8(0x97)

#define OCR3A   _SFR_MEM16(0x98)
#define OCR3AL  _SFR_MEM8(0x98)
#define OCR3AH  _SFR_MEM8(0x99)

#define OCR3B   _SFR_MEM16(0x9A)
#define OCR3BL  _SFR_MEM8(0x9A)
#define OCR3BH  _SFR_MEM8(0x9B)

#define DTR3    _SFR_MEM16(0x9C)
#define DTR3L   _SFR_MEM8(0x9C)
#define DTR3H   _SFR_MEM8(0x9D)

#define OCR3C   _SFR_MEM16(0x9E)
#define OCR3CL  _SFR_MEM8(0x9E)
#define OCR3CH  _SFR_MEM8(0x9F)

// Bits of PSSR
#define PSS3    6
#define PSR3    1

//----------------------------------------------------------------
// PCI (PIN Change Interrupt) Register Definition
//----------------------------------------------------------------
// Bits of PCICR
#define PCIE4   4
#define PCIE3   3

// Bits of PCIFR
#define PCIF4   4
#define PCIF3   3

#define PCMSK3  _SFR_MEM8(0x73)
#define PCINT31 7
#define PCINT30 6
#define PCINT29 5
#define PCINT28 4
#define PCINT27 3
#define PCINT26 2
#define PCINT25 1
#define PCINT24 0

#define PCMSK4  _SFR_MEM8(0x74)
#define PCINT39 7
#define PCINT38 6
#define PCINT37 5
#define PCINT36 4
#define PCINT35 3
#define PCINT34 2
#define PCINT33 1
#define PCINT32 0

//----------------------------------------------------------------
// ADC (Analog to Digital Controller) Register Definition
//----------------------------------------------------------------

// bit of ADCSRB
#define ICTL    4

#define ADCSRC  _SFR_MEM8(0x7D)
#define OFEN    7
#define OFSF    6
#define SPN     5
#define AMEN    4
#define SPD     2
#define DIFS    1
#define ADTM    0

#define OFR0    _SFR_MEM8(0xA3)
#define OFR1    _SFR_MEM8(0xA4)

#define ADT0    _SFR_MEM16(0xA5)
#define ADT0L   _SFR_MEM8(0xA5)
#define ADT0H   _SFR_MEM8(0xA6)

#define ADT1    _SFR_MEM16(0xAA)
#define ADT1L   _SFR_MEM8(0xAA)
#define ADT1H   _SFR_MEM8(0xAB)

#define ADMSC   _SFR_MEM8(0xAC)
#define AMOF    7
#define AMFC3   3
#define AMFC2   2
#define AMFC1   1
#define AMFC0   0

#define ADCSRD  _SFR_MEM8(0xAD)
#define BGEN    7
#define REFS2   6
#define IVSEL1  5
#define IVSEL0  4
#define VDS2    2
#define VDS1    1
#define VDS0    0

#define DAPCR   _SFR_MEM8(0xDC)
#define DAPEN   7
#define GA1     6
#define GA0     5
#define DNS2    4
#define DNS1    3
#define DNS0    2
#define DPS1    1
#define DPS0    0

#define DAPTR   _SFR_MEM8(0xDD)

#define DAPTC   _SFR_MEM8(0xDE)
#define DAPTE   7

// Bits of ADCSRB
#define ACME01  7
#define ACME00  6
#define ACME11  5
#define ACME10  4
#define ACTS    3

// Bits of DIDR0
#define ADC7D   7
#define ADC6D   6
#define PE3D    7
#define PE1D    6
#define PC5D    5
#define PC4D    4
#define PC3D    3
#define PC2D    2
#define PC1D    1
#define PC0D    0

// Bits of DIDR1
#define PE7D    7
#define PE6D    6
#define PE0D    5
#define C0PD    4
#define PF0D    3
#define PC7D    2
#define PD7D    1
#define PD6D    0

#define DIDR2   _SFR_MEM8(0x76)
#define PB5D    6

//----------------------------------------------------------------
// DAC (Digital to Analog Controller) Register Definition
//----------------------------------------------------------------
#define DACON   _SFR_MEM8(0xA0)
#define DACEN   3
#define DAOE    2
#define DAVS1   1
#define DAVS0   0

#define DALR    _SFR_MEM8(0xA1)
#define DAL0    _SFR_MEM8(0xA1)

//----------------------------------------------------------------
// Config () Register Definition
//----------------------------------------------------------------
// Bits of MCUCR
#define FWKEN   7
#define FPDEN   6
#define EXRFD   5
#define IRLD    3
#define IFAIL   2
#define MWCE    0

// Bits of MCUSR
#define SWDD    7
#define OCDRF   4

// Bits of CLKPR
#define WCE     7
#define CLKOE1  6
#define CLKOE0  5
#define CLKOE   5

#define VDTCR   _SFR_MEM8(0x62)
//#define WCE   7
#define SWR     6
#define VDTS2   4
#define VDTS1   3
#define VDTS0   2
#define VDREN   1
#define VDTEN   0

//Bits of Power Reduce Register
#define PRUART0  1

#define PRR1    _SFR_MEM8(0x65)
#define PRWDT   5
#define PRTC3   3
#define PREFL   2
#define PRPCI   1

#define RCMCAL  _SFR_MEM8(0x66)
#define RCKCAL  _SFR_MEM8(0x67)

#define IVBASE  _SFR_MEM8(0x75)

#define IOCWK   _SFR_MEM8(0xAE)
#define IOCD7   7
#define IOCD6   6
#define IOCD5   5
#define IOCD4   4
#define IOCD3   3
#define IOCD2   2
#define IOCD1   1
#define IOCD0   0

#define DPS2R   _SFR_MEM8(0xAF)
#define DPS2E   3
#define LPRCE   2
#define TOS1    1
#define TOS0    0

#define VCAL    _SFR_MEM8(0xC8)
#define VCAL3   _SFR_MEM8(0xCC)
#define VCAL1   _SFR_MEM8(0xCD)
#define VCAL2   _SFR_MEM8(0xCE)

#define LDOCR   _SFR_MEM8(0xCF)
//#define WCE   7
#define PDEN    3
#define VSEL2   2
#define VSEL1   1
#define VSEL0   0

#define HDR     _SFR_MEM8(0xE0)
#define HDR5    5
#define HDR4    4
#define HDR3    3
#define HDR2    2
#define HDR1    1
#define HDR0    0

#define PMX1    _SFR_MEM8(0xED)
#define C3AC    2
#define C2BF7   1
#define C2AF6   0

#define PMXCR   _SFR_MEM8(0xEE)
#define PMX0    _SFR_MEM8(0xEE)
//#define WCE   7
#define C1BF4   6
#define C1AF5   5
#define C0BF3   4
#define C0AC0   3
#define SSB1    2
#define TXD6    1
#define RXD5    0

#define PMX2    _SFR_MEM8(0xF0)
#define IOCR    _SFR_MEM8(0xF0)
//#define WCE   7
#define IOCE    7
#define STSC1   6
#define STSC0   5
#define XIEN    2
#define E6EN    1
#define REFIOEN 1
#define C6EN    0
#define RSTIOEN 0

#define PMCR    _SFR_MEM8(0xF2)
#define PMCE    7
#define CLKFS   6
#define CLKSS   5
#define WCLKS   4
#define WCES    4
#define OSCKEN  3
#define OSCMEN  2
#define RCKEN   1
#define RCMEN   0

#define GUID0   _SFR_MEM8(0xF3)
#define GUID1   _SFR_MEM8(0xF4)
#define GUID2   _SFR_MEM8(0xF5)
#define GUID3   _SFR_MEM8(0xF6)

#define _VECTOR(N) __vector_ ## N
#undef SPM_Ready_vect
#undef SPM_Ready_vect_num
#define ANALOG_COMP_0_vect_num  23
#define ANALOG_COMP_0_vect      _VECTOR(23)
#define ANALOG_COMP_1_vect_num  25
#define ANALOG_COMP_1_vect      _VECTOR(25)
#define PCINT3_vect_num         27
#define PCINT3_vect             _VECTOR(27)
#define PCINT4_vect_num         28
#define PCINT4_vect             _VECTOR(28)
#define TIMER3_vect_num         29
#define TIMER3_vect             _VECTOR(29)

#undef _VECTORS_SIZE
#define _VECTORS_SIZE (30 * 4)

#endif
/**********************************************************************************
***                                     EOF                                     ***
**********************************************************************************/
