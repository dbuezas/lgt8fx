#ifndef __FASTIO_H__
#define __FASTIO_H__

#define _FIO_DDR_0	DDRD
#define _FIO_DDR_1	DDRD
#define _FIO_DDR_2	DDRD
#define _FIO_DDR_3	DDRD
#define _FIO_DDR_4	DDRD
#define _FIO_DDR_5	DDRD
#define _FIO_DDR_6	DDRD
#define _FIO_DDR_7	DDRD
#define _FIO_DDR_8	DDRB
#define _FIO_DDR_9	DDRB
#define _FIO_DDR_10	DDRB 
#define _FIO_DDR_11	DDRB 
#define _FIO_DDR_12	DDRB 
#define _FIO_DDR_13	DDRB 
#define _FIO_DDR_14	DDRC 
#define _FIO_DDR_15	DDRC 
#define _FIO_DDR_16	DDRC 
#define _FIO_DDR_17	DDRC 
#define _FIO_DDR_18	DDRC 
#define _FIO_DDR_19	DDRC
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__) 
#define _FIO_DDR_20	DDRE 
#define _FIO_DDR_21	DDRE
#if defined(__LGT8FX8P48__) 
#define _FIO_DDR_22	DDRB 
#define _FIO_DDR_23	DDRC 
#define _FIO_DDR_24	DDRF 
#define _FIO_DDR_25	DDRE 
#define _FIO_DDR_26	DDRE 
#define _FIO_DDR_27	DDRB 
#define _FIO_DDR_28	DDRC 
#define _FIO_DDR_29	DDRE 
#define _FIO_DDR_30	DDRE 
#define _FIO_DDR_31	DDRE 
#define _FIO_DDR_32	DDRF 
#define _FIO_DDR_33	DDRF 
#define _FIO_DDR_34	DDRF 
#define _FIO_DDR_35	DDRF 
#define _FIO_DDR_36	DDRF 
#define _FIO_DDR_37	DDRF 
#define _FIO_DDR_38	DDRF 
#define _FIO_DDR_39	DDRF 
#else
#define _FIO_DDR_22 DDRE
#define _FIO_DDR_23 DDRE
#define _FIO_DDR_24 DDRE
#define _FIO_DDR_25 DDRE
#define _FIO_DDR_26 DDRE
#endif
#endif

#define _FIO_DDR_D0	DDRD
#define _FIO_DDR_D1	DDRD
#define _FIO_DDR_D2	DDRD
#define _FIO_DDR_D3	DDRD
#define _FIO_DDR_D4	DDRD
#define _FIO_DDR_D5	DDRD
#define _FIO_DDR_D6	DDRD
#define _FIO_DDR_D7	DDRD
#define _FIO_DDR_D8	DDRB
#define _FIO_DDR_D9	DDRB
#define _FIO_DDR_D10	DDRB 
#define _FIO_DDR_D11	DDRB 
#define _FIO_DDR_D12	DDRB 
#define _FIO_DDR_D13	DDRB 
#define _FIO_DDR_D14	DDRC 
#define _FIO_DDR_D15	DDRC 
#define _FIO_DDR_D16	DDRC 
#define _FIO_DDR_D17	DDRC 
#define _FIO_DDR_D18	DDRC 
#define _FIO_DDR_D19	DDRC
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__) 
#define _FIO_DDR_D20	DDRE 
#define _FIO_DDR_D21	DDRE
#if defined(__LGT8FX8P48__) 
#define _FIO_DDR_D22	DDRB 
#define _FIO_DDR_D23	DDRC 
#define _FIO_DDR_D24	DDRF 
#define _FIO_DDR_D25	DDRE 
#define _FIO_DDR_D26	DDRE 
#define _FIO_DDR_D27	DDRB 
#define _FIO_DDR_D28	DDRC 
#define _FIO_DDR_D29	DDRE 
#define _FIO_DDR_D30	DDRE 
#define _FIO_DDR_D31	DDRE 
#define _FIO_DDR_D32	DDRF 
#define _FIO_DDR_D33	DDRF 
#define _FIO_DDR_D34	DDRF 
#define _FIO_DDR_D35	DDRF 
#define _FIO_DDR_D36	DDRF 
#define _FIO_DDR_D37	DDRF 
#define _FIO_DDR_D38	DDRF 
#define _FIO_DDR_D39	DDRF 
#else
#define _FIO_DDR_D22 DDRE
#define _FIO_DDR_D23 DDRE
#define _FIO_DDR_D24 DDRE
#define _FIO_DDR_D25 DDRE
#define _FIO_DDR_D26 DDRE
#endif
#endif

#define _FIO_PORT_0	PORTD
#define _FIO_PORT_1	PORTD
#define _FIO_PORT_2	PORTD
#define _FIO_PORT_3	PORTD
#define _FIO_PORT_4	PORTD
#define _FIO_PORT_5	PORTD
#define _FIO_PORT_6	PORTD
#define _FIO_PORT_7	PORTD
#define _FIO_PORT_8	PORTB
#define _FIO_PORT_9	PORTB
#define _FIO_PORT_10	PORTB 
#define _FIO_PORT_11	PORTB 
#define _FIO_PORT_12	PORTB 
#define _FIO_PORT_13	PORTB 
#define _FIO_PORT_14	PORTC 
#define _FIO_PORT_15	PORTC 
#define _FIO_PORT_16	PORTC 
#define _FIO_PORT_17	PORTC 
#define _FIO_PORT_18	PORTC 
#define _FIO_PORT_19	PORTC
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__) 
#define _FIO_PORT_20	PORTE 
#define _FIO_PORT_21	PORTE
#if defined(__LGT8FX8P48__) 
#define _FIO_PORT_22	PORTB 
#define _FIO_PORT_23	PORTC 
#define _FIO_PORT_24	PORTF 
#define _FIO_PORT_25	PORTE 
#define _FIO_PORT_26	PORTE 
#define _FIO_PORT_27	PORTB 
#define _FIO_PORT_28	PORTC 
#define _FIO_PORT_29	PORTE 
#define _FIO_PORT_30	PORTE 
#define _FIO_PORT_31	PORTE 
#define _FIO_PORT_32	PORTF 
#define _FIO_PORT_33	PORTF 
#define _FIO_PORT_34	PORTF 
#define _FIO_PORT_35	PORTF 
#define _FIO_PORT_36	PORTF 
#define _FIO_PORT_37	PORTF 
#define _FIO_PORT_38	PORTF 
#define _FIO_PORT_39	PORTF 
#else
#define _FIO_PORT_22 PORTE
#define _FIO_PORT_23 PORTE
#define _FIO_PORT_24 PORTE
#define _FIO_PORT_25 PORTE
#define _FIO_PORT_26 PORTE
#endif
#endif

#define _FIO_PORT_D0	PORTD
#define _FIO_PORT_D1	PORTD
#define _FIO_PORT_D2	PORTD
#define _FIO_PORT_D3	PORTD
#define _FIO_PORT_D4	PORTD
#define _FIO_PORT_D5	PORTD
#define _FIO_PORT_D6	PORTD
#define _FIO_PORT_D7	PORTD
#define _FIO_PORT_D8	PORTB
#define _FIO_PORT_D9	PORTB
#define _FIO_PORT_D10	PORTB 
#define _FIO_PORT_D11	PORTB 
#define _FIO_PORT_D12	PORTB 
#define _FIO_PORT_D13	PORTB 
#define _FIO_PORT_D14	PORTC 
#define _FIO_PORT_D15	PORTC 
#define _FIO_PORT_D16	PORTC 
#define _FIO_PORT_D17	PORTC 
#define _FIO_PORT_D18	PORTC 
#define _FIO_PORT_D19	PORTC
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__) 
#define _FIO_PORT_D20	PORTE 
#define _FIO_PORT_D21	PORTE
#if defined(__LGT8FX8P48__) 
#define _FIO_PORT_D22	PORTB 
#define _FIO_PORT_D23	PORTC 
#define _FIO_PORT_D24	PORTF 
#define _FIO_PORT_D25	PORTE 
#define _FIO_PORT_D26	PORTE 
#define _FIO_PORT_D27	PORTB 
#define _FIO_PORT_D28	PORTC 
#define _FIO_PORT_D29	PORTE 
#define _FIO_PORT_D30	PORTE 
#define _FIO_PORT_D31	PORTE 
#define _FIO_PORT_D32	PORTF 
#define _FIO_PORT_D33	PORTF 
#define _FIO_PORT_D34	PORTF 
#define _FIO_PORT_D35	PORTF 
#define _FIO_PORT_D36	PORTF 
#define _FIO_PORT_D37	PORTF 
#define _FIO_PORT_D38	PORTF 
#define _FIO_PORT_D39	PORTF 
#else
#define _FIO_PORT_D22 PORTE
#define _FIO_PORT_D23 PORTE
#define _FIO_PORT_D24 PORTE
#define _FIO_PORT_D25 PORTE
#define _FIO_PORT_D26 PORTE
#endif
#endif

#define _FIO_PIN_0	PIND
#define _FIO_PIN_1	PIND
#define _FIO_PIN_2	PIND
#define _FIO_PIN_3	PIND
#define _FIO_PIN_4	PIND
#define _FIO_PIN_5	PIND
#define _FIO_PIN_6	PIND
#define _FIO_PIN_7	PIND
#define _FIO_PIN_8	PINB
#define _FIO_PIN_9	PINB
#define _FIO_PIN_10	PINB 
#define _FIO_PIN_11	PINB 
#define _FIO_PIN_12	PINB 
#define _FIO_PIN_13	PINB 
#define _FIO_PIN_14	PINC 
#define _FIO_PIN_15	PINC 
#define _FIO_PIN_16	PINC 
#define _FIO_PIN_17	PINC 
#define _FIO_PIN_18	PINC 
#define _FIO_PIN_19	PINC
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__) 
#define _FIO_PIN_20	PINE 
#define _FIO_PIN_21	PINE
#if defined(__LGT8FX8P48__) 
#define _FIO_PIN_22	PINB 
#define _FIO_PIN_23	PINC 
#define _FIO_PIN_24	PINF 
#define _FIO_PIN_25	PINE 
#define _FIO_PIN_26	PINE 
#define _FIO_PIN_27	PINB 
#define _FIO_PIN_28	PINC 
#define _FIO_PIN_29	PINE 
#define _FIO_PIN_30	PINE 
#define _FIO_PIN_31	PINE 
#define _FIO_PIN_32	PINF 
#define _FIO_PIN_33	PINF 
#define _FIO_PIN_34	PINF 
#define _FIO_PIN_35	PINF 
#define _FIO_PIN_36	PINF 
#define _FIO_PIN_37	PINF 
#define _FIO_PIN_38	PINF 
#define _FIO_PIN_39	PINF 
#else
#define _FIO_PIN_22 PINE
#define _FIO_PIN_23 PINE
#define _FIO_PIN_24 PINE
#define _FIO_PIN_25 PINE
#define _FIO_PIN_26 PINE
#endif
#endif

#define _FIO_PIN_D0	PIND
#define _FIO_PIN_D1	PIND
#define _FIO_PIN_D2	PIND
#define _FIO_PIN_D3	PIND
#define _FIO_PIN_D4	PIND
#define _FIO_PIN_D5	PIND
#define _FIO_PIN_D6	PIND
#define _FIO_PIN_D7	PIND
#define _FIO_PIN_D8	PINB
#define _FIO_PIN_D9	PINB
#define _FIO_PIN_D10	PINB 
#define _FIO_PIN_D11	PINB 
#define _FIO_PIN_D12	PINB 
#define _FIO_PIN_D13	PINB 
#define _FIO_PIN_D14	PINC 
#define _FIO_PIN_D15	PINC 
#define _FIO_PIN_D16	PINC 
#define _FIO_PIN_D17	PINC 
#define _FIO_PIN_D18	PINC 
#define _FIO_PIN_D19	PINC
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__) 
#define _FIO_PIN_D20	PINE 
#define _FIO_PIN_D21	PINE
#if defined(__LGT8FX8P48__) 
#define _FIO_PIN_D22	PINB 
#define _FIO_PIN_D23	PINC 
#define _FIO_PIN_D24	PINF 
#define _FIO_PIN_D25	PINE 
#define _FIO_PIN_D26	PINE 
#define _FIO_PIN_D27	PINB 
#define _FIO_PIN_D28	PINC 
#define _FIO_PIN_D29	PINE 
#define _FIO_PIN_D30	PINE 
#define _FIO_PIN_D31	PINE 
#define _FIO_PIN_D32	PINF 
#define _FIO_PIN_D33	PINF 
#define _FIO_PIN_D34	PINF 
#define _FIO_PIN_D35	PINF 
#define _FIO_PIN_D36	PINF 
#define _FIO_PIN_D37	PINF 
#define _FIO_PIN_D38	PINF 
#define _FIO_PIN_D39	PINF 
#else
#define _FIO_PIN_D22 PINE
#define _FIO_PIN_D23 PINE
#define _FIO_PIN_D24 PINE
#define _FIO_PIN_D25 PINE
#define _FIO_PIN_D26 PINE
#endif
#endif

#define _FIO_BIT_0  0
#define _FIO_BIT_1  1
#define _FIO_BIT_2  2
#define _FIO_BIT_3  3
#define _FIO_BIT_4  4
#define _FIO_BIT_5  5
#define _FIO_BIT_6  6
#define _FIO_BIT_7  7
#define _FIO_BIT_8  0
#define _FIO_BIT_9  1
#define _FIO_BIT_10  2
#define _FIO_BIT_11  3
#define _FIO_BIT_12  4
#define _FIO_BIT_13  5
#define _FIO_BIT_14  0
#define _FIO_BIT_15  1
#define _FIO_BIT_16  2
#define _FIO_BIT_17  3
#define _FIO_BIT_18  4
#define _FIO_BIT_19  5
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
#define _FIO_BIT_20  1
#define _FIO_BIT_21  3
#if defined(__LGT8FX8P48__)
#define _FIO_BIT_22  6
#define _FIO_BIT_23  7
#define _FIO_BIT_24  0
#define _FIO_BIT_25  6
#define _FIO_BIT_26  7
#define _FIO_BIT_27  7
#define _FIO_BIT_28  6
#define _FIO_BIT_29  0
#define _FIO_BIT_30  2
#define _FIO_BIT_31  4
#define _FIO_BIT_32  5
#define _FIO_BIT_33  1
#define _FIO_BIT_34  2
#define _FIO_BIT_35  3
#define _FIO_BIT_36  4
#define _FIO_BIT_37  5
#define _FIO_BIT_38  6
#define _FIO_BIT_39  7
#else
#define _FIO_BIT_22  0
#define _FIO_BIT_23  2
#define _FIO_BIT_24  4
#define _FIO_BIT_25  5
#define _FIO_BIT_26  6
#endif
#endif

#define _FIO_BIT_D0  0
#define _FIO_BIT_D1  1
#define _FIO_BIT_D2  2
#define _FIO_BIT_D3  3
#define _FIO_BIT_D4  4
#define _FIO_BIT_D5  5
#define _FIO_BIT_D6  6
#define _FIO_BIT_D7  7
#define _FIO_BIT_D8  0
#define _FIO_BIT_D9  1
#define _FIO_BIT_D10  2
#define _FIO_BIT_D11  3
#define _FIO_BIT_D12  4
#define _FIO_BIT_D13  5
#define _FIO_BIT_D14  0
#define _FIO_BIT_D15  1
#define _FIO_BIT_D16  2
#define _FIO_BIT_D17  3
#define _FIO_BIT_D18  4
#define _FIO_BIT_D19  5
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
#define _FIO_BIT_D20  1
#define _FIO_BIT_D21  3
#if defined(__LGT8FX8P48__)
#define _FIO_BIT_D22  6
#define _FIO_BIT_D23  7
#define _FIO_BIT_D24  0
#define _FIO_BIT_D25  6
#define _FIO_BIT_D26  7
#define _FIO_BIT_D27  7
#define _FIO_BIT_D28  6
#define _FIO_BIT_D29  0
#define _FIO_BIT_D30  2
#define _FIO_BIT_D31  4
#define _FIO_BIT_D32  5
#define _FIO_BIT_D33  1
#define _FIO_BIT_D34  2
#define _FIO_BIT_D35  3
#define _FIO_BIT_D36  4
#define _FIO_BIT_D37  5
#define _FIO_BIT_D38  6
#define _FIO_BIT_D39  7
#else
#define _FIO_BIT_D22  0
#define _FIO_BIT_D23  2
#define _FIO_BIT_D24  4
#define _FIO_BIT_D25  5
#define _FIO_BIT_D26  6
#endif
#endif


#define fastioMode(pin, dir) do {\
	if((dir & INPUT_PULLUP) == INPUT_PULLUP) _FIO_PORT_##pin |= (1 << _FIO_BIT_##pin);\
    if(dir == OUTPUT) _FIO_DDR_##pin |= (1 << _FIO_BIT_##pin);\
    else _FIO_DDR_##pin &= ~(1 << _FIO_BIT_##pin); \
} while(0)

#define fastioWrite(pin, val) do {\
	_FIO_PORT_##pin = (_FIO_PORT_##pin & ~(1 << _FIO_BIT_##pin)) | (val << _FIO_BIT_##pin);\
} while(0)

#define fastioRead(pin) ((_FIO_PIN_##pin >> _FIO_BIT_##pin) & 0x1)

#define fastioToggle(pin) do {\
	_FIO_PIN_##pin = (1 << _FIO_BIT_##pin);\
} while(0)

#endif
