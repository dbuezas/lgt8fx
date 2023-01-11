/*
VERSION HISTORY:
----------------
New version     : 1.1
Date            : Feb 14, 2021
Revised by      : github.com/LaZsolt
Description     : volatile char * converted to Arduino style _SFR_IO and _SFR_MEM
*/

#ifndef _IOLGT8F88D_H_
#define _IOLGT8F88D_H_

//ADCSRB
#define ICTL    4
//MCUSR
#define PDRF    6
#define SWDD    7       // swd interface disable
//TCCR0B
#define OC0AS   5

#define GUID3   _SFR_MEM8(0xF6)
#define GUID2   _SFR_MEM8(0xF5)
#define GUID1   _SFR_MEM8(0xF4)
#define GUID0   _SFR_MEM8(0xF3)

#define PMXCR   _SFR_MEM8(0xEE)
#define PSSR    _SFR_MEM8(0xE2)
#define PSS1    7
#define PSR1    0

       // DIDR2, DIDR3, TKCSR is not present in LGT8Fx8D
//#define DIDR2 _SFR_MEM8(0xCE)
//#define DIDR3 _SFR_MEM8(0xCF)
//#define TKCSR _SFR_MEM8(0xCD)

#define TCKSR   _SFR_MEM8(0xEC)
#define TCKCSR  _SFR_MEM8(0xEC)
#define F2XEN   6
#define TC2XF1  5
#define TC2XF0  4
#define TC2XS1  1
#define TC2XS0  0

#define PINE    _SFR_MEM8(0xA7)
#define PINE0   0
#define PINE1   1
#define PINE2   2
#define PINE3   3
#define PINE4   4
#define PINE5   5
#define PINE6   6

#define DDRE    _SFR_MEM8(0xA8)
#define DDE0    0
#define DDE1    1
#define DDE2    2
#define DDE3    3
#define DDE4    4
#define DDE5    5
#define DDE6    6

#define PORTE   _SFR_MEM8(0xA9)
#define PORTE0  0
#define PORTE1  1
#define PORTE2  2
#define PORTE3  3
#define PORTE4  4
#define PORTE5  5
#define PORTE6  6
#define PE6     6
#define PE5     5
#define PE4     4
#define PE3     3
#define PE2     2
#define PE1     1
#define PE0     0

#define OCPUE   _SFR_MEM8(0x9F)

#define IOCR    _SFR_MEM8(0xF0)
#define RSTIOEN 0
#define REFIOEN 1
#define XIEN    2
#define DACEN0  3
#define DACEN1  4
#define STOSC0  5
#define STOSC1  6
#define IOCE    7

#define PMCR    _SFR_MEM8(0xF2)
#define RCMEN   0
#define RCKEN   1
#define SCMEN   2
#define OSCKEN  3
#define WCES    4
#define CLKSS   5
#define EXTEN   5
#define CLKFS   6
#define PMCE    7

#define VCAL    _SFR_MEM8(0xC8)
#define VCAL1   _SFR_MEM8(0xCD)
#define VCAL2   _SFR_MEM8(0xCE)
// Declare a variable for save and store ivref calibration_1
extern uint8_t _VCAL_1_;

// TCCR1C
#define DOC1B   5
#define DOC1A   4
#define DTEN1   3
#define DSX1    _SFR_MEM8(0x83)
#define DTR1    _SFR_MEM8(0x8C)

#define ADTMR   _SFR_MEM8(0x7D)

// PCICR
#define PCIE3   3
#define PCIE4   4

// PCIFR
#define PCIF3   3
#define PCIF4   4

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

// CLKPR
#define CLKOE   5
#define CLKOE1  6
#define CLKOE0  5

#define VDTCR   _SFR_MEM8(0x62)
#define VDTCE   7
#define SWRSTN  6
#define VSEL    1
#define VDTEN   0

#define RCKCAL  _SFR_MEM8(0x67)
#define RCCAL   _SFR_MEM8(0x66)

#define PRR1    _SFR_MEM8(0x65)
#define PRPCI   1
#define PREFL   2
#define PRWDT   5

#define LDOCR   _SFR_MEM8(0xCF)
#define LVL     0
#define PDE     3
#define LWEN    7

#define OP0TCNT _SFR_IO8(0x3A)
#define OP0CRB  _SFR_IO8(0x39)
#define TCSH    0
#define OPTEN   7
#define OP0CRA  _SFR_IO8(0x38)
#define CH0EN   0
#define CH1EN   1
#define CH0IM   2
#define CH1IM   3
#define ACFEN   5
#define ACCH    6
#define OPAEN   7

#define ECCR    _SFR_IO8(0x36)
#define EC      0
#define EEN     6
#define EWEN    7

#define DAL0    _SFR_IO8(0x32)
#define AFTCNT0 _SFR_IO8(0x31)

// TCCR0A
#define DOC0A   2
#define DOC0B   3
// TCCR0B
#define DTEN0   4
#define DSX0    _SFR_IO8(0x29)
#define DTR0    _SFR_IO8(0x2F)

#define OP1TCNT _SFR_IO8(0x14)
#define OP1CRB  _SFR_IO8(0x13)
#define OP1CRA  _SFR_IO8(0x12)
#define DAL1    _SFR_IO8(0x11)
#define AFTCNT1 _SFR_IO8(0x10)

#define AC1SR   _SFR_IO8(0x0F)

#define _VECTOR(N) __vector_ ## N
#undef SPM_Ready_vect
#undef SPM_Ready_vect_num
#define ANALOG_COMP_0_vect_num  23
#define ANALOG_COMP_0_vect      _VECTOR(23)
#define ANALOG_COMP_1_vect_num  25
#define ANALOG_COMP_1_vect      _VECTOR(25)
#define PCINT3_vect_num         27
#define PCINT3_vect             _VECTOR(27)
#define OPA0_COMP_vect_num      28
#define OPA0_COMP_vect          _VECTOR(28)
#define OPA1_COMP_vect_num      29
#define OPA1_COMP_vect          _VECTOR(29)

#undef _VECTORS_SIZE
#define _VECTORS_SIZE (30 * 4)

#endif
/**********************************************************************************
***                                     EOF                                     ***
**********************************************************************************/
