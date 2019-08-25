/*
*******************************  C HEADER FILE  **********************************
** project  : LGT8F88ABSPPrj
** filename : iolgt8f88a.h 
** version  : 1.0 	
** date     : April 01, 2013
** 			  
**********************************************************************************
** All rights reserved.       
**********************************************************************************
VERSION HISTORY:
----------------
Version 	: 1.0
Date 		: April 01, 2013
Revised by 	: LogicGreen Software Group
Description : Original version.
*/

#ifndef _IOLGT8F88A_H_
#define _IOLGT8F88A_H_

//ADCSRB
#define ICTL    4
//MCUSR
#define PDRF    6
#define SWDD	7		// swd interface disable

#define RCCAL	(*((volatile unsigned char *)0x66))
#define PSSR	(*((volatile unsigned char *)0xE2))

#define DIDR2	(*((volatile unsigned char *)0xce))
#define DIDR3	(*((volatile unsigned char *)0xcf))
#define TKCSR	(*((volatile unsigned char *)0xcd))	

#define PINE	(*((volatile unsigned char *)0xa7))
#define DDRE	(*((volatile unsigned char *)0xa8))
#define PORTE	(*((volatile unsigned char *)0xa9))	

#define OCPUE	(*((volatile unsigned char *)0x9f))	

#define IOCR	(*((volatile unsigned char *)0xf0))
#define DSCR	(*((volatile unsigned char *)0xf1))
#define PMCR	(*((volatile unsigned char *)0xf2))
#define RCMEN	0
#define RCKEN	1
#define SCMEN	2
#define SCKEN	3
#define WCES    4
#define EXTEN   5
#define LFEN    6
#define PMCE	7

#define PCIE3	3
#define PCMSK3	(*((volatile unsigned char *)0x73))


//#define CLKPR	(*(volatile unsigned char *)0x61)
//#define CLKPCE	7
#define CLKOE	5
#define CLKOE1	6
#define CLKOE0	5
//#define CLKPS3	3
//#define CLKPS2	2
//#define CLKPS1	1
//#define CLKPS0	0
//#define CLKPS	0
#define PRR1	(*(volatile unsigned char *)0x65)
#define PRPCI   1
#define PREFL   2
#define PRWDT   5

#endif
