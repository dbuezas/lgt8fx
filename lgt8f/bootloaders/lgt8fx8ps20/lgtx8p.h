/*
**
** Copyright (c) 2013, 	LogicGreen techologies
** All rights reserved.
**
** project  : LGT8F328PBSPPrj
** filename : lgt8f328p_spec.h
** version  : 1.0
** date     : Feb 01, 2017

VERSION HISTORY:
----------------
Version 	: 1.0
Date 		: Feb 01, 2017
Revised by 	: LogicGreen Software Group
Description : Original version.
**
*/

#ifndef _LGT8F328P_SPEC_H_
#define _LGT8F328P_SPEC_H_

//----------------------------------------------------------------
// DSU (Digital Signal Co-processor) Register Definition
//----------------------------------------------------------------
#define DSCR	(*((volatile unsigned char *)0x20))	
#define	DSUEN	7
#define	MM	6
#define	DSD1	5
#define	DSD0	4
#define	DSN	2
#define	DSZ	1
#define	DSC	0

#define DSIR	(*((volatile unsigned char *)0x21))	
#define DSSD	(*((volatile unsigned char *)0x22))	

#define DSDX	(*((volatile unsigned char *)0x30))	
#define DSDY	(*((volatile unsigned char *)0x31))	

#define DSAL	(*((volatile unsigned char *)0x58))	
#define DSAH	(*((volatile unsigned char *)0x59))	

//----------------------------------------------------------------
// GPIO Group C Register Definition
//----------------------------------------------------------------
#define	PC7	7

//----------------------------------------------------------------
// GPIO Group E Register Definition
//----------------------------------------------------------------
#define IPINE	(*((volatile unsigned char *)0x2C))
#define IDDRE	(*((volatile unsigned char *)0x2D))
#define IPORTE	(*((volatile unsigned char *)0x2E))	

#define EPINE	(*((volatile unsigned char *)0xA7))
#define EDDRE	(*((volatile unsigned char *)0xA8))
#define EPORTE	(*((volatile unsigned char *)0xA9))	

//#define USE_EIO_EREG

#ifdef USE_EIO_EREG
#define PINE	EPINE
#define DDRE	EDDRE
#define PORTE	EPORTE
#else
#define PINE	IPINE
#define DDRE	IDDRE
#define PORTE	IPORTE
#endif

#define OCPUE	(*((volatile unsigned char *)0xE1))	

#define	PE7	7
#define	PE6	6
#define	PE5	5
#define	PE4	4
#define	PE3	3
#define	PE2	2
#define	PE1	1
#define	PE0	0

//----------------------------------------------------------------
// GPIO Group F Register Definition
//----------------------------------------------------------------
#define PINF	(*((volatile unsigned char *)0x32))
#define DDRF	(*((volatile unsigned char *)0x33))
#define PORTF	(*((volatile unsigned char *)0x34))	

#define	PF7	7
#define	PF6	6
#define	PF5	5
#define	PF4	4
#define	PF3	3
#define	PF2	2
#define	PF1	1
#define	PF0	0

//----------------------------------------------------------------
// SPI (Serial Periphral Interface) Register Definition
//----------------------------------------------------------------
#define SPFR	(*((volatile unsigned char *)0x39))	
#define	RDFULL	7
#define	RDEMPT	6
#define	RDPTR1	5
#define	RDPTR0	4
#define	WRFULL	3
#define	WREMPT	2
#define	WRPTR1	1
#define	WRPTR0	0

// Bits of SPSR
#define DUAL	2

//----------------------------------------------------------------
// AC0 (Analog Comparator 0) Register Definition
//----------------------------------------------------------------
#define C0SR	(*((volatile unsigned char *)0x50))	
#define	C0D	7
#define	C0BG	6
#define	C0O	5
#define	C0I	4
#define	C0IE	3
#define	C0IC	2
#define	C0IS1	1
#define	C0IS0	0

#define C0XR	(*((volatile unsigned char *)0x51))	
#define	C0OE	6
#define	C0HSYE	5
#define	C0PS0	4
#define	C0WKE	3
#define	C0FEN	2
#define	C0FS1	1
#define	C0FS0	0

#define C0TR	(*((volatile unsigned char *)0x52))	

//----------------------------------------------------------------
// AC1 (Analog Comparator 1) Register Definition
//----------------------------------------------------------------
#define C1SR	(*((volatile unsigned char *)0x2F))	
#define	C1D	7
#define	C1BG	6
#define	C1O	5
#define	C1I	4
#define	C1IE	3
#define	C1IC	2
#define	C1IS1	1
#define	C1IS0	0

#define C1XR	(*((volatile unsigned char *)0x3A))	
#define	C1OE	6
#define	C1HSYE	5
#define	C1PS0	4
#define	C1WKE	3
#define	C1FEN	2
#define	C1FS1	1
#define	C1FS0	0

#define C1TR	(*((volatile unsigned char *)0x5B))	

//----------------------------------------------------------------
// EEP (E2PROM Controller) Register Definition
//----------------------------------------------------------------
#define E2PDL	(*((volatile unsigned char *)0x40))	
#define E2PDH	(*((volatile unsigned char *)0x5A))	

#define E2PD0	(*((volatile unsigned char *)0x40))	
#define E2PD1	(*((volatile unsigned char *)0x5A))	
#define E2PD2	(*((volatile unsigned char *)0x57))	
#define E2PD3	(*((volatile unsigned char *)0x5C))	

#define	ECCR	(*((volatile unsigned char *)0x56))
#define ECS	0
#define	CP0	2
#define	CP1	3
#define	SWM	4
#define ERN	5
#define	EEN	6
#define	EWEN	7

//----------------------------------------------------------------
// TC0 (Timer Counter 0) Register Definition
//----------------------------------------------------------------
#define TCCR0C	(*((volatile unsigned char *)0x49))
#define DSX07	7
#define DSX06	6
#define DSX05	5
#define DSX04	4
#define DSX01	1
#define DSX00	0

#define DTR0	(*((volatile unsigned char *)0x4F))

// Bits of TCCR0B
#define OC0AS	5
#define DTEN0 	4

// Bits of TIFR0
#define OC0A	7
#define OC0B	6

//----------------------------------------------------------------
// TC1 (Timer Counter 1) Register Definition
//----------------------------------------------------------------
#define TCCR1D	(*((volatile unsigned char *)0x83))
#define DSX17	7
#define DSX16	6
#define DSX15	5
#define DSX14	4
#define DSX11	1
#define DSX10	0

#define DTR1L	(*((volatile unsigned char *)0x8C))
#define DTR1H	(*((volatile unsigned char *)0x8D))

#define PSSR	(*((volatile unsigned char *)0xE2))
#define PSS1	7
#define PSR1	0

#define TCKSR	(*((volatile unsigned char *)0xEC))
#define TCKCSR	(*((volatile unsigned char *)0xEC))
#define F2XEN	6
#define TC2XF1	5
#define TC2XF0	4
#define TC2XS1	1
#define TC2XS0	0

// Bits of TCCR1C
#define DOC1B	5
#define DOC1A	4
#define DTEN1	3

// Bits of TIFR1
#define OC1A	7
#define OC1B	6

//----------------------------------------------------------------
// TC2 (Timer Counter 2) Register Definition
//----------------------------------------------------------------

// Bits of ASSR
#define INTCK	7

// Bits of TIFR2
#define OC2A	7
#define OC2B	6

//----------------------------------------------------------------
// TC3 (Timer Counter 3) Register Definition
//----------------------------------------------------------------
#define TIFR3	(*((volatile unsigned char *)0x38))	
#define OC3A	7
#define OC3B	6
#define ICF3 	5
#define OCF3C 	3
#define OCF3B 	2
#define OCF3A 	1
#define TOV3 	0

#define TIMSK3	(*((volatile unsigned char *)0x71))
#define ICIE3 	5
#define OCIE3C 	3
#define OCIE3B 	2
#define OCIE3A 	1
#define TOIE3 	0

#define TCCR3A	(*((volatile unsigned char *)0x90))
#define COM3A1 	7
#define COM3A0 	6
#define COM3B1 	5
#define COM3B0 	4
#define COM3C1 	3
#define COM3C0 	2
#define WGM31 	1
#define WGM30 	0

#define TCCR3B	(*((volatile unsigned char *)0x91))
#define ICNC3 	7
#define ICES3 	6
#define WGM33 	4
#define WGM32 	3
#define CS32 	2
#define CS31 	1
#define CS30 	0

#define TCCR3C	(*((volatile unsigned char *)0x92))
#define FOC3A 	7
#define FOC3B 	6
#define DOC31 	5
#define DOC30 	4
#define DTEN3 	3
#define DOC32 	1
#define FOC3C 	0

#define TCCR3D	(*((volatile unsigned char *)0x93))
#define DSX37	7
#define DSX36	6
#define DSX35	5
#define DSX34	4
#define DSX31	1
#define DSX30	0

#define TCNT3L	(*((volatile unsigned char *)0x94))
#define TCNT3H	(*((volatile unsigned char *)0x95))

#define ICR3L	(*((volatile unsigned char *)0x96))
#define ICR3H	(*((volatile unsigned char *)0x97))

#define OCR3AL	(*((volatile unsigned char *)0x98))
#define OCR3AH	(*((volatile unsigned char *)0x99))

#define OCR3BL	(*((volatile unsigned char *)0x9A))
#define OCR3BH	(*((volatile unsigned char *)0x9B))

#define DTR3A	(*((volatile unsigned char *)0x9C))
#define DTR3B	(*((volatile unsigned char *)0x9D))

#define OCR3CL	(*((volatile unsigned char *)0x9E))
#define OCR3CH	(*((volatile unsigned char *)0x9F))

// Bits of PSSR
#define PSS3	6
#define PSR3	1

//----------------------------------------------------------------
// PCI (PIN Change Interrupt) Register Definition
//----------------------------------------------------------------
#define PCMSK3	(*((volatile unsigned char *)0x73))
#define PCMSK4	(*((volatile unsigned char *)0x74))

// Bits of PCICR
#define PCIE4	4
#define PCIE3	3

// Bits of PCIFR
#define PCIF4	4
#define PCIF3	3

//----------------------------------------------------------------
// ADC (Analog to Digital Controller) Register Definition
//----------------------------------------------------------------

// bit of ADCSRB
#define ICTL	4

#define ADCSRC	(*((volatile unsigned char *)0x7D))
#define OFEN	7
#define OFSF	6
#define SPN	5
#define AMEN	4
#define SPD	2
#define DIFS	1
#define ADTM	0

#define OFR0	(*((volatile unsigned char *)0xA3))
#define OFR1	(*((volatile unsigned char *)0xA4))

#define ADT0L	(*((volatile unsigned char *)0xA5))
#define ADT0H	(*((volatile unsigned char *)0xA6))

#define ADT1L	(*((volatile unsigned char *)0xAA))
#define ADT1H	(*((volatile unsigned char *)0xAB))

#define ADMSC	(*((volatile unsigned char *)0xAC))
#define AMOF	7
#define AMFC3	3
#define AMFC2	2
#define AMFC1	1
#define AMFC0	0

#define ADCSRD	(*((volatile unsigned char *)0xAD))
#define BGEN	7
#define REFS2	6
#define IVSEL1	5
#define IVSEL0	4
#define VDS2	2
#define VDS1	1
#define VDS0	0

#define DAPCR	(*((volatile unsigned char *)0xDC))
#define DAPEN	7
#define GA1	6
#define GA0	5
#define DNS2	4
#define DNS1	3
#define DNS0	2
#define DPS1	1
#define DPS0	0

#define DAPTR	(*((volatile unsigned char *)0xDD))

#define DAPTC	(*((volatile unsigned char *)0xDE))
#define DAPTE	7

// Bits of ADCSRB
#define ACME01	7
#define ACME00	6
#define ACME11	5
#define ACME10	4
#define ACTS	3

// Bits of DIDR0
#define ADC7D	7
#define ADC6D	6
#define PE3D	7
#define PE1D	6
#define PC5D	5
#define PC4D	4
#define PC3D	3
#define PC2D	2
#define PC1D	1
#define PC0D	0

// Bits of DIDR1
#define PE7D	7
#define PE6D	6
#define PE0D	5
#define C0PD	4
#define PF0D	3
#define PC7D	2
#define PD7D	1
#define PD6D	0

#define DIDR2	(*((volatile unsigned char *)0x76))
#define PB5D	6

//----------------------------------------------------------------
// DAC (Digital to Analog Controller) Register Definition
//----------------------------------------------------------------
#define DACON	(*((volatile unsigned char *)0xA0))
#define DACEN	3
#define DAOE	2
#define DAVS1	1
#define DAVS0	0

#define DALR	(*((volatile unsigned char *)0xA1))

//----------------------------------------------------------------
// Config () Register Definition
//----------------------------------------------------------------
// Bits of MCUCR
#define FWKEN	7
#define FPDEN	6
#define EXRFD	5
#define IRLD	3
#define IFAIL	2
#define MWCE	0

// Bits of MCUSR
#define SWDD	7
#define OCDRF	4

// Bits of CLKPR
#define WCE	7
#define CLKOE1	6 
#define CLKOE0	5
#define CLKOE	5 

#define VDTCR	(*((volatile unsigned char *)0x62))
//#define WCE	7
#define SWR	6
#define VDTS2	4
#define VDTS1	3
#define VDTS0	2
#define VDREN	1
#define VDTEN	0

#define PRR1	(*((volatile unsigned char *)0x65))
#define	PRWDT	5
#define	PRTC3	3
#define	PREFL	2
#define	PRPCI	1

#define RCMCAL	(*((volatile unsigned char *)0x66))
#define RCKCAL	(*((volatile unsigned char *)0x67))

#define IVBASE	(*((volatile unsigned char *)0x75))

#define IOCWK	(*((volatile unsigned char *)0xAE))
#define IOCD7
#define IOCD6
#define IOCD5
#define IOCD4
#define IOCD3
#define IOCD2
#define IOCD1
#define IOCD0

#define DPS2R	(*((volatile unsigned char *)0xAF))
#define DPS2E	3
#define LPRCE	2
#define TOS1	1
#define TOS0	0

#define VCAL 	(*((volatile unsigned char *)0xC8))
#define VCAL3	(*((volatile unsigned char *)0xCC))
#define VCAL1	(*((volatile unsigned char *)0xCD))
#define VCAL2	(*((volatile unsigned char *)0xCE))

#define LDOCR	(*((volatile unsigned char *)0xCF))	
//#define WCE	7
#define PDEN	3
#define VSEL2	2
#define VSEL1	1
#define VSEL0	0

#define HDR	(*((volatile unsigned char *)0xE0))	
#define HDR5	5
#define HDR4	4
#define HDR3	3
#define HDR2	2
#define HDR1	1
#define HDR0	0

#define PMX1	(*((volatile unsigned char *)0xED))
#define C3AC	2
#define C2BF7	1
#define C2AF6	0

#define PMX0	(*((volatile unsigned char *)0xEE))
//#define WCE	7
#define C1BF4	6
#define C1AF5	5
#define C0BF3	4
#define C0AC0	3
#define SSB1	2
#define TXD6	1
#define RXD5	0

#define PMX2	(*((volatile unsigned char *)0xF0))
#define IOCR	(*((volatile unsigned char *)0xF0))
//#define WCE	7
#define IOCE	7
#define STSC1	6
#define STSC0	5
#define XIEN	2
#define E6EN	1
#define REFIOEN	1
#define C6EN	0
#define RSTIOEN	0

#define PMCR	(*((volatile unsigned char *)0xF2))
#define PMCE	7
#define CLKFS	6
#define CLKSS	5
#define WCLKS	4
#define WCES	4
#define OSCKEN	3
#define OSCMEN	2
#define RCKEN	1
#define RCMEN	0

#define GUID0	(*((volatile unsigned char *)0xF3))
#define GUID1	(*((volatile unsigned char *)0xF4))
#define GUID2	(*((volatile unsigned char *)0xF5))
#define GUID3	(*((volatile unsigned char *)0xF6))

#endif
/**********************************************************************************
***	     	  			EOF					***									  	
**********************************************************************************/ 
