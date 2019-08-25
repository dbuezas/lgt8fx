/* Demonstration of Rtc_Pcf8563 Clock on LCD. 
 *
 * I used a RBBB with Arduino IDE, the pins are mapped a 
 * bit differently.  Change for your hw.
 * SCK - A5, SDA - A4, INT - D3/INT1
 *
 * This sketch connects a lcd to display the clock output.
 * 
 * setup:  see Pcf8563 data sheet.
 *         1x 10Kohm pullup on Pin3 INT
 *         No pullups on Pin5 or Pin6 (I2C internals used)
 *         1x 0.1pf on power
 *         1x 32khz chrystal
 *         1x h44780 LCD
 *
 * Joe Robertson, jmr
 * orbitalair@bellsouth.net
 */

#include <Wire.h>
#include <Rtc_Pcf8563.h>
/* add the lcd support */ 
#include <LiquidCrystal.h>

//init the real time clock
Rtc_Pcf8563 rtc;

/* initialize the library objects */
/* LiquidCrystal lcd(rs, en, d4, d5, d6, d7); */
LiquidCrystal lcd(4 ,9 ,5 ,6 ,7 ,8);

void setup()
{
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  
  //clear out all the registers
  rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century, year
  rtc.setDate(14, 6, 3, 0, 10);
  //hr, min, sec
  rtc.setTime(1, 15, 40);
}

void loop()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  //lcd.print(rtc.formatTime(RTCC_TIME_HM));
  lcd.print(rtc.formatTime());
  lcd.setCursor(0, 1);
  //lcd.print(rtc.formatDate(RTCC_DATE_ASIA));
  lcd.print(rtc.formatDate());
  
  delay(1000);
    
}




