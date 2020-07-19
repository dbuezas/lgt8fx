/*
To make vscode use this documentation, add as the last forcedInclude file in
your c_cpp_properties.json, for example:
"forcedInclude": [
        "/Applications/Arduino.app/Contents/Java/hardware/tools/avr/avr/include/avr/iomx8.h",
        "~/code/lgt8fx/lgt8f/variants/lgt8fx8p/pins_arduino.h",
        "~/code/lgt8fx/lgt8f/cores/lgt8f/lgtx8p.h",
        "~/code/lgt8fx/lgt8f/cores/lgt8f/Arduino.h",
        "~/code/lgt8fx/lgt8f/cores/lgt8f/lgtx8p_documentation.h"
],
DO NOT INCLUDE THIS FILE IN YOUR CODE

The information in this file was taken from "LGT8FX8P_databook_V1.05 By
https://github.com/watterott"
https://github.com/watterott/LGT8F328P-Testing/raw/master/LGT8FX8P_databook_v1.0.5-English.pdf

Not all registers are documented here, PRs welcomed!
*/

/*
//----------------------------------------------------------------
// ADMUX--ADC mutl-channel selection control register
//----------------------------------------------------------------
Address: 0x7C
7:6     |   REFS[1:0]
In combination with REFS2 in ADCSRD register to select ADC
reference voltage source. By setting REFS control bit to select
reference voltage, if changing REFS setting during conversion,
such change only effect after conversion is complete.

REFS2, REFS[1:0]    |	Reference voltage selection
0_00	AREF
0_01	AVCC
0_10	On-chip 2.048V reference voltage source
0_11	On-chip 1.024V reference voltage source
1_00	On-chip 4.096V reference voltage source
//----------------------------------------------------------------
5   |   ADLAR   |	Conversion result left adjustment enable control bit.
When setting ADLAR to one, conversion result in ADC data register is left
adjustment When setting ADLAR to zero, conversion result in ADC data register is
right adjustment
//----------------------------------------------------------------
4:0	CHMUX[ 4:0]	ADC input source selection control bit
CHMUX[4:0]	Singe-end input source	Comment
0_0000	PC0	external input port
0_0001	PC1
0_0010	PC2
0_0011	PC3
0_0100	PC4
0_0101	PC5
0_0110	PE1
0_0111	PE3
0_1001	PC7
0_1010	PF0
0_1011	PE6
0_1100	PE7
0_1110	4/5VDO	Internal voltage division circuit
0_1000	1/5VDO
0_1101	IVREF	Internal reference
0_1111	AGND	Analog ground
0_xxxx	DACO	Internal DAC output
*/
#define ADMUX

/*
//----------------------------------------------------------------
// DAC (Digital to Analog Controller) Register Definition
//----------------------------------------------------------------
Address: 0XA1
|   3   |   DACEN   |   DAC enable control bit
1= enable DAC modular
0= disable DAC modular
//----------------------------------------------------------------
|   2   |   DAOE	|   DAC output to external port enable control
1= DAC output to external port PD4 is enabled
0= DAC output to external port is disabled
//----------------------------------------------------------------
|   1   |   DAVS1   |   DAC reference voltage source selection bit 1
|   0   |   DAVS0	|   DAC reference voltage source selection bit 0
00: voltage source is system working power VCC
01: voltage source is external input AVREF
10: voltage source is internal reference voltage
11: shut down DAC reference source and DAC at the same time
*/
#define DACON

/*
//----------------------------------------------------------------
// ADCSRC--ADC control status register C
//----------------------------------------------------------------
Address: 0x7D						Default value:0x00
7		OFEN	1=Enable detuning compensate 0= Disable detuning
compensate
//----------------------------------------------------------------
6	-	Unimplemented
//----------------------------------------------------------------
5	SPN	ADC conversion input polarity control, only used in
detuning calibration. In normal mode it must be cleared.
//----------------------------------------------------------------
4	AMEN	Channel auto monitoring enable: 1: enable channel auto
monitoring 0: disable channel auto monitoring
//----------------------------------------------------------------
3	-	Unimplemented
//----------------------------------------------------------------
2	SPD	"0= ADC low speed conversion mode
 1= ADC high speed conversion mode, only used for analog input with
 low resistance"
//----------------------------------------------------------------
1	DIFS	"0= ADC conversion from ADC multiplexer
 1= ADC conversion from internal differential amplifier"
//----------------------------------------------------------------
0	ADTM	Testing mode, internal reference voltage is output from
AVREF port
*/
#define ADCSRC

/*
//----------------------------------------------------------------
DAPCR--Differential operation amplifier control register
Address: 0xDC					Default value:0x00
//----------------------------------------------------------------
7		DAPEN
1= enable differential amplifier
1= disable differential amplifier
//----------------------------------------------------------------

6:5		GA[1:0]	Differential amplifier gain control
 00 = x1
 01 = x8
 10 = x16
 11 = x32"
//----------------------------------------------------------------
4:2		DNS[2:0]	"Differential amplifier reverse input source
selection bit 000 = ADC2/APN0 001 = ADC3/APN1 010 = ADC8/APN2 011 = ADC9/APN3
 100 = PE0/APN4
 101 = ADC multiplexer
 110 = AGND 111 = disable differential amplifier reverse input"
//----------------------------------------------------------------
1:0	DPS[1:0]	"Differential amplifier forward input source
selection bit 00 = ADC multiplexer
 01 = ADC0/APP0
 10 = ADC1/APP1
 11 = AGND"
 */
#define DAPCR

/*
//----------------------------------------------------------------
        ADCSRB--ADC control and status register B
//----------------------------------------------------------------
Address: 0x7B						Default value:0x00
7		ACME01	"Comparator 0 negative input selection
6		ACME00
 00: negative select external input ACIN0
 01: negative select ADC multiplexer output
 1x: nagative select operation amplifer 0 output"
 //----------------------------------------------------------------
5		ACME11	"Comparator 1 negative input selection
4		ACME10
 00: negative select external input ACIN2
 01: negative select ADC multiplexer output
 1x: nagative select operation amplifer 1 output"
 //----------------------------------------------------------------
3		ACTS	"AC trigger source channel selection
 0- AC0 output used as ADC auto conversion trigger source 1- AC1
 output used as ADC auto conversion trigger source"
 //----------------------------------------------------------------
2:0		ADTS[2:0 ]	"ADC auto trigger source selection control bit
 When setting ADATE is one, auto triggering is enabled, trigger source is
control by ADTS. When setting ADATE is zero, auto ADTS setting is useless.
Rising edge of selected trigger singal interrupt flag will start one conversion.
When trigger source is switched from interrupt clear to interupt enable, trigger
singal will generate a rising edge, if ADEN is set in this case, ADC will also
start a conversion. When in continurous conversion mode (ADTS=0),
Auto-triggering is disabled."

ADTS[2:0]			Trigger source
0			Continurous conversion mode
1			Comparator 0/1
2			External interrupt 0
3			Timer/Counter 0 compare match
4			Timer/Counter 0 overflow
5			Timer/Counter 1 compare match B
6			Timer/Counter 1 overflow
7			Timer/Counter 1 input capture event
*/
#define ADCSRB

/*
ADCSRD--ADC control register D
//----------------------------------------------------------------
6		BGEN	Internal reference globe control enable, 1=enable
//----------------------------------------------------------------
5		REFS2	In combination with REFS in ADMUX register, used
to select reference voltage of ADC conversion, referring to
definition of REFS in ADMUX.
//----------------------------------------------------------------
5:4		IVSEL	"When ADC reference voltage is set as VCC or AVREF,
IVSEL is used to control internal reference of output voltage:
00 = 1.024V
01 = 2.048V
1x = 4.096V"
//----------------------------------------------------------------
3		-	reserved
//----------------------------------------------------------------
2:0		VDS[2:0]	"Voltage division circuit input source selection
000/111 =shut down voltage division circuit modular
001 = ADC0
010 = ADC1
011 = ADC4
100 = ADC5
101 = external reference input (AVREF)
110 = system power"
*/
#define ADCSRD

/*
//----------------------------------------------------------------
TCCR1A –TC1 Control Register A
//----------------------------------------------------------------
Address: 0x80					Default value: 0x00
Bit	7	6	5	4	3	2	1	0
        COM1A1	COM1A0	COM1B1	COM1B0	-	-	WGM11	WGM10
R/W	R/W	R/W	R/W	R/W	-	-	R/W	R/W
Bit	Name	Comment
//----------------------------------------------------------------
7	COM1A1	"Compare match output A mode control high
 COM1A1 and COM1A0 combine to consist of COM1A[1:0], used to control 0C1A output
compare waveform. If 1 or 2 of the COM0A bits are set, output compare waveform
takes up 0C1A pin, however whose data direction regester must be set to high. In
different mode, C0M1A control output compare waveform differently, for details
referring to list on compare output control."
//----------------------------------------------------------------
6	COM1A0	"Compare match output A mode control low
 COM1A1 and COM1A0 combine to consist of COM1A[1:0], used to control 0C1A output
compare waveform. If 1 or 2 of the COM0A bits are set, output compare waveform
takes up 0C1A pin, however whose data direction regester must be set to high. In
different mode, C0M1A control compare waveform differently, for details
referring to list on compare output control."
//----------------------------------------------------------------
5	COM1B1	"Compare match output B mode control high
 COM1B1 and COM1B0 combine to consist of COM1B[1:0], used to control 0C1B output
compare waveform. If 1 or 2 of the COM0B bits are set, compare waveform takes up
0C1B pin, however whose data direction regester must be set to high. In
different mode, C0M1B control compare waveform differently, for details
referring to list on compare output control."
//----------------------------------------------------------------
4	COM1B0	"Compare match output B mode control low
 COM1B1 and COM1B0 combine to consist of COM1B[1:0], used to control 0C1B output
compare waveform. If 1 or 2 of the COM0B bits are set, compare waveform takes up
0C1B pin, however whose data direction regester must be set to high. In
different mode, C0M1B control compare waveform differently, for details
referring to list on compare output control."
//----------------------------------------------------------------
3:2	-	reserved
//----------------------------------------------------------------
1	WGM11	"Waveform generation mode control sub-low
 WGM11, WGM13, WGM12 and WGM10 generate waveform generation control WGM1[3:0]
together, counting method and waveform generation method of control counter is
referring to list on waveform generation mode."
//----------------------------------------------------------------
0	WGM10	"Waveform generation mode control lowest
 WGM10, WGM13, WGM12 and WGM11 generate waveform generation control WGM1[3:0]
together, counting method and waveform generation method of control counter is
referring to list on waveform generation mode."

//----------------------------------------------------------------
Below list shows how compare output mode controls output compare waveform in
non-PWM mode (normal mode and CTC mode)
//----------------------------------------------------------------
        COM1x[1:0]			Comment

0 OC1x disconnect, general IO port operation
1 Toggle OC1x signal in compare match
2 Clear OC1x signal in compare match
3 Set OC1x signal in compare match

//----------------------------------------------------------------
Below list shows how compare output mode controls output compare
waveform in fast PWM mode
//----------------------------------------------------------------
        COM1x[1:0]			Comment
0 OC1x disconnect, general IO port operation
1 If WGM1 is 15, OC1A signal is toggled in
compare match and OC1B is disconnected If WGM1 is other value,
OC1x disconnected, general IO port operation
2 Clear OC1x signal in compare match, while
set OC1x in maximum value match
3 Set OC1x signal in compare match, while
clear OC1x in maximum value match

//----------------------------------------------------------------
Below list shows how compare output mode controls output compare
waveform in phase correction mode
//----------------------------------------------------------------
        COM1x[1:0]			Comment
0 OC1x disconnect, general IO port operation
1 If WGM1 is 9 or 11, OC1A signal is toggled
in compare match and OC1B is disconnected If WGM1 is other value,
OC1x disconnected, general IO port operation
2 OC1x is cleared in up-counting compare match,
and set OC1x in down-counting compare match
3 OC1x is set in up-counting compare match,
and clear OC1x in down-counting compare match
*/
#define TCCR1A