/*
  PMU.h - PMU library
  Copyright (c) 2015 LogicGreen All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef PMU_h
#define PMU_h

#include <Arduino.h>

/* 
 * power mode defintion:
 * PM_IDLE0: disable core_clock only
 * PM_POWERDWN: disable most clocks and switch main clock to rc32k
 * PM_POFFS0: power off all core functions (digial and analog)
 * 		wake up by external interrupt or periodly
 * PM_POFFS1: the lowest power mode which close all modules and clocks 
 * 		wake up by external interrupt only
 */

#define	PM_ADC_ON 	0xFFFE
#define	PM_USART0_ON 	0xFFFD
#define	PM_SPI_ON	0xFFFB
#define	PM_TIM1_ON	0xFFF7
#define	PM_BOD_ON	0xFFEF
#define	PM_TIM0_ON	0xFFDF
#define	PM_TIM2_ON	0xFFBF
#define	PM_TWI_ON	0xFF7F

#define	PM_PCI_ON	0xFDFF
#define	PM_E2P_ON	0xFBFF
#define	PM_WDT_ON	0xDFFF

enum period_t {
	SLEEP_64MS = 0,
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

enum pmu_t {
	PM_IDLE,
	PM_POWERDOWN = 2,
	PM_POFFS1 = 3,
	PM_POFFS0 = 6
};

class PowerControl
{
private:
	uint8_t pmcr_reg;
	uint8_t clkpr_reg;
	uint8_t wdtcr_reg;
	uint8_t ldocr_reg;
#if defined(__LGT8FX8E__)
	uint8_t adcsra_reg;
	uint8_t ddrd_reg;
	uint8_t ddrb_reg;
	uint8_t portd_reg;
	uint8_t portb_reg;
	uint8_t didr0_reg;
#endif

	void resume(pmu_t mode, period_t period);
	void clock_switch(pmu_t mode);
	void do_sleep(pmu_t mode);
	void enable_wdt(period_t period);
	void enable_rc32k(bool wcs = 0);

public:
	PowerControl();
	void sleep(pmu_t mode, period_t period = SLEEP_FOREVER);
	void setIOCompatible() { DDRE |= 0x10; PORTE &= 0xef; };
};

extern PowerControl PMU;

#endif
