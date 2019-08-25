
#ifndef _IOLGT8F88D_H_
#define _IOLGT8F88D_H_

//ADCSRB
#define ICTL    4
//MCUSR
#define PDRF    6
#define SWDD	7		// swd interface disable

#define OC0AS		5

#define GUID3	(*((volatile unsigned char *)0xF6))
#define GUID2	(*((volatile unsigned char *)0xF5))
#define GUID1	(*((volatile unsigned char *)0xF4))
#define GUID0	(*((volatile unsigned char *)0xF3))

#define PMXCR	(*((volatile unsigned char *)0xEE))
#define PSSR	(*((volatile unsigned char *)0xE2))

#define DIDR2	(*((volatile unsigned char *)0xce))
#define DIDR3	(*((volatile unsigned char *)0xcf))
#define TKCSR	(*((volatile unsigned char *)0xcd))	

#define PINE	(*((volatile unsigned char *)0xa7))
#define DDRE	(*((volatile unsigned char *)0xa8))
#define PORTE	(*((volatile unsigned char *)0xa9))	
#define	PE6	6
#define	PE5	5
#define	PE4	4
#define	PE3	3
#define	PE2	2
#define	PE1	1
#define	PE0	0

#define OCPUE	(*((volatile unsigned char *)0x9f))	

#define IOCR	(*((volatile unsigned char *)0xf0))
#define	RSTIOEN 0
#define REFIOEN 1
#define XIEN	2
#define	DACEN0	3
#define	DACEN1	4
#define	STOSC0	5
#define	STOSC1	6
#define	IOCE	7

#define PMCR	(*((volatile unsigned char *)0xf2))
#define RCMEN	0
#define RCKEN	1
#define SCMEN	2
#define OSCKEN	3
#define WCES    4
#define CLKSS   5
#define EXTEN   5
#define CLKFS   6
#define PMCE	7

#define	VCAL	(*((volatile unsigned char *)0xC8))
#define	VCAL1	(*((volatile unsigned char *)0xCD))
#define VCAL2	(*((volatile unsigned char *)0xCE))

// TCCR1C
#define DOC1B	5
#define DOC1A	4
#define DTEN1	3
#define DSX1	(*((volatile unsigned char *)0x83))
#define DTR1	(*((volatile unsigned char *)0x8C))

#define ADTMR	(*((volatile unsigned char *)0x7D))

// PCICR
#define PCIE3	3
#define PCIE4	4

// PCIFR
#define	PCIF3	3
#define	PCIF4	4

#define PCMSK3	(*((volatile unsigned char *)0x73))
#define	PCINT31	7
#define	PCINT30	6
#define	PCINT29	5
#define	PCINT28	4
#define	PCINT27	3
#define	PCINT26	2
#define	PCINT25	1
#define	PCINT24	0

#define PCMSK4	(*((volatile unsigned char *)0x74))
#define	PCINT39	7
#define	PCINT38	6
#define	PCINT37	5
#define	PCINT36	4
#define	PCINT35	3
#define	PCINT34	2
#define	PCINT33	1
#define	PCINT32	0

// CLKPR
#define CLKOE	5
#define CLKOE1	6
#define CLKOE0	5

#define VDTCR	(*((volatile unsigned char *)0x62))
#define VDTCE	7
#define SWRSTN	6
#define VSEL	1
#define	VDTEN	0

#define RCKCAL	(*((volatile unsigned char *)0x67))
#define RCCAL	(*((volatile unsigned char *)0x66))

#define PRR1	(*((volatile unsigned char *)0x65))
#define PRPCI   1
#define PREFL   2
#define PRWDT   5

#define LDOCR	(*((volatile unsigned char *)0xCF))
#define	LVL	0
#define	PDE	3
#define	LWEN	7

#define	OP0TCNT	(*((volatile unsigned char *)0x5A))
#define	OP0CRB	(*((volatile unsigned char *)0x59))
#define	TCSH	0
#define	OPTEN	7
#define	OP0CRA	(*((volatile unsigned char *)0x58))
#define	CH0EN	0
#define	CH1EN	1
#define	CH0IM	2
#define	CH1IM	3
#define	ACFEN	5
#define	ACCH	6
#define	OPAEN	7

#define	ECCR	(*((volatile unsigned char *)0x56))
#define	EC	0
#define	EEN	6
#define	EWEN	7

#define	DAL0	(*((volatile unsigned char *)0x52))
#define	AFTCNT0	(*((volatile unsigned char *)0x51))

// TCCR0A
#define	DOC0A	2
#define DOC0B	3
// TCCR0B
#define DTEN0	4
#define	DSX0	(*((volatile unsigned char *)0x49))
#define	DTR0	(*((volatile unsigned char *)0x4f))

#define	OP1TCNT	(*((volatile unsigned char *)0x34))
#define OP1CRB	(*((volatile unsigned char *)0x33))
#define	OP1CRA	(*((volatile unsigned char *)0x32))
#define	DAL1	(*((volatile unsigned char *)0x31))
#define AFTCNT1	(*((volatile unsigned char *)0x30))

#define AC1SR	(*((volatile unsigned char *)0x2f))


#endif
/**********************************************************************************
***					         				EOF									***													  	
**********************************************************************************/ 
