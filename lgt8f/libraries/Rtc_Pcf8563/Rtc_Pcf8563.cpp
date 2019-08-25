/*****
 *  NAME
 *    Pcf8563 Real Time Clock support routines
 *  AUTHOR
 *    Joe Robertson, jmr
 *    orbitalair@bellsouth.net
 *    http://orbitalair.wikispaces.com/Arduino
 *  CREATION DATE
 *    9/24/06,  init - built off of usart demo.  using mikroC
 *  NOTES
 *  HISTORY
 *    10/14/06 ported to CCS compiler, jmr
 *    2/21/09  changed all return values to hex val and not bcd, jmr
 *    1/10/10  ported to arduino, jmr
 *    2/14/10  added 3 world date formats, jmr
 *    28/02/2012 A. Pasotti
 *             fixed a bug in RTCC_ALARM_AF,
 *             added a few (not really useful) methods
 *    22/10/2014 Fix whitespace, tabs, and newlines, cevich
 *    22/10/2014 add voltLow get/set, cevich
 *    22/10/2014 add century get, cevich
 *    22/10/2014 Fix get/set date/time race condition, cevich
 *    22/10/2014 Header/Code rearranging, alarm/timer flag masking
 *               extern Wire, cevich
 *    26/11/2014 Add zeroClock(), initialize to lowest possible
 *               values, cevich
 *    22/10/2014 add timer support, cevich
 *
 *  TODO
 *    x Add Euro date format
 *    Add short time (hh:mm) format
 *    Add 24h/12h format
 ******
 *  Robodoc embedded documentation.
 *  http://www.xs4all.nl/~rfsber/Robo/robodoc.html
 */

#include <Arduino.h>
#include "Rtc_Pcf8563.h"

Rtc_Pcf8563::Rtc_Pcf8563(void)
{
    Wire.begin();
    Rtcc_Addr = RTCC_R>>1;
}

/* Private internal functions, but useful to look at if you need a similar func. */
byte Rtc_Pcf8563::decToBcd(byte val)
{
    return ( (val/10*16) + (val%10) );
}

byte Rtc_Pcf8563::bcdToDec(byte val)
{
    return ( (val/16*10) + (val%16) );
}

void Rtc_Pcf8563::zeroClock()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);        // start address

    Wire.write((byte)0x0);     //control/status1
    Wire.write((byte)0x0);     //control/status2
    Wire.write((byte)0x00);    //set seconds to 0 & VL to 0
    Wire.write((byte)0x00);    //set minutes to 0
    Wire.write((byte)0x00);    //set hour to 0
    Wire.write((byte)0x01);    //set day to 1
    Wire.write((byte)0x00);    //set weekday to 0
    Wire.write((byte)0x81);    //set month to 1, century to 1900
    Wire.write((byte)0x00);    //set year to 0
    Wire.write((byte)0x80);    //minute alarm value reset to 00
    Wire.write((byte)0x80);    //hour alarm value reset to 00
    Wire.write((byte)0x80);    //day alarm value reset to 00
    Wire.write((byte)0x80);    //weekday alarm value reset to 00
    Wire.write((byte)SQW_32KHZ); //set SQW to default, see: setSquareWave
    Wire.write((byte)0x0);     //timer off
    Wire.endTransmission();
}

void Rtc_Pcf8563::clearStatus()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);
    Wire.write((byte)0x0);                 //control/status1
    Wire.write((byte)0x0);                 //control/status2
    Wire.endTransmission();
}

/*
* Read status byte
*/
byte Rtc_Pcf8563::readStatus2()
{
    getDateTime();
    return getStatus2();
}

void Rtc_Pcf8563::clearVoltLow(void)
{
    getDateTime();
    // Only clearing is possible on device (I tried)
    setDateTime(getDay(), getWeekday(), getMonth(),
                getCentury(), getYear(), getHour(),
                getMinute(), getSecond());
}

/*
* Atomicly read all device registers in one operation
*/
void Rtc_Pcf8563::getDateTime(void)
{
    /* Start at beginning, read entire memory in one go */
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT1_ADDR);
    Wire.endTransmission();

    /* As per data sheet, have to read everything all in one operation */
    uint8_t readBuffer[16] = {0};
    Wire.requestFrom(Rtcc_Addr, 16);
    for (uint8_t i=0; i < 16; i++)
        readBuffer[i] = Wire.read();

    // status bytes
    status1 = readBuffer[0];
    status2 = readBuffer[1];

    // time bytes
    //0x7f = 0b01111111
    volt_low = readBuffer[2] & RTCC_VLSEC_MASK;  //VL_Seconds
    sec = bcdToDec(readBuffer[2] & ~RTCC_VLSEC_MASK);
    minute = bcdToDec(readBuffer[3] & 0x7f);
    //0x3f = 0b00111111
    hour = bcdToDec(readBuffer[4] & 0x3f);

    // date bytes
    //0x3f = 0b00111111
    day = bcdToDec(readBuffer[5] & 0x3f);
    //0x07 = 0b00000111
    weekday = bcdToDec(readBuffer[6] & 0x07);
    //get raw month data byte and set month and century with it.
    month = readBuffer[7];
    if (month & RTCC_CENTURY_MASK)
        century = true;
    else
        century = false;
    //0x1f = 0b00011111
    month = month & 0x1f;
    month = bcdToDec(month);
    year = bcdToDec(readBuffer[8]);

    // alarm bytes
    alarm_minute = readBuffer[9];
    if(B10000000 & alarm_minute)
        alarm_minute = RTCC_NO_ALARM;
    else
        alarm_minute = bcdToDec(alarm_minute & B01111111);
    alarm_hour = readBuffer[10];
    if(B10000000 & alarm_hour)
        alarm_hour = RTCC_NO_ALARM;
    else
        alarm_hour = bcdToDec(alarm_hour & B00111111);
    alarm_day = readBuffer[11];
    if(B10000000 & alarm_day)
        alarm_day = RTCC_NO_ALARM;
    else
        alarm_day = bcdToDec(alarm_day  & B00111111);
    alarm_weekday = readBuffer[12];
    if(B10000000 & alarm_weekday)
        alarm_weekday = RTCC_NO_ALARM;
    else
        alarm_weekday = bcdToDec(alarm_weekday  & B00000111);

    // CLKOUT_control 0x03 = 0b00000011
    squareWave = readBuffer[13] & 0x03;

    // timer bytes
    timer_control = readBuffer[14] & 0x03;
    timer_value = readBuffer[15];  // current value != set value when running
}


void Rtc_Pcf8563::setDateTime(byte day, byte weekday, byte month,
                              bool century, byte year, byte hour,
                              byte minute, byte sec)
{
    /* year val is 00 to 99, xx
        with the highest bit of month = century
        0=20xx
        1=19xx
        */
    month = decToBcd(month);
    if (century)
        month |= RTCC_CENTURY_MASK;
    else
        month &= ~RTCC_CENTURY_MASK;

    /* As per data sheet, have to set everything all in one operation */
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write(RTCC_SEC_ADDR);       // send addr low byte, req'd
    Wire.write(decToBcd(sec) &~RTCC_VLSEC_MASK); //set sec, clear VL bit
    Wire.write(decToBcd(minute));    //set minutes
    Wire.write(decToBcd(hour));        //set hour
    Wire.write(decToBcd(day));            //set day
    Wire.write(decToBcd(weekday));    //set weekday
    Wire.write(month);                 //set month, century to 1
    Wire.write(decToBcd(year));        //set year to 99
    Wire.endTransmission();
    // Keep values in-sync with device
    getDateTime();
}

/**
* Get alarm, set values to RTCC_NO_ALARM (99) if alarm flag is not set
*/
void Rtc_Pcf8563::getAlarm()
{
    getDateTime();
}

/*
* Returns true if AIE is on
*
*/
bool Rtc_Pcf8563::alarmEnabled()
{
    return getStatus2() & RTCC_ALARM_AIE;
}

/*
* Returns true if AF is on
*
*/
bool Rtc_Pcf8563::alarmActive()
{
    return getStatus2() & RTCC_ALARM_AF;
}

/* enable alarm interrupt
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void Rtc_Pcf8563::enableAlarm()
{
    getDateTime();  // operate on current values
    //set status2 AF val to zero
    status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_TIMER_TF;
    //enable the interrupt
    status2 |= RTCC_ALARM_AIE;

    //enable the interrupt
    Wire.beginTransmission(Rtcc_Addr);  // Issue I2C start signal
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();
}

/* set the alarm values
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void Rtc_Pcf8563::setAlarm(byte min, byte hour, byte day, byte weekday)
{
    getDateTime();  // operate on current values
    if (min <99) {
        min = constrain(min, 0, 59);
        min = decToBcd(min);
        min &= ~RTCC_ALARM;
    } else {
        min = 0x0; min |= RTCC_ALARM;
    }

    if (hour <99) {
        hour = constrain(hour, 0, 23);
        hour = decToBcd(hour);
        hour &= ~RTCC_ALARM;
    } else {
        hour = 0x0; hour |= RTCC_ALARM;
    }

    if (day <99) {
        day = constrain(day, 1, 31);
        day = decToBcd(day); day &= ~RTCC_ALARM;
    } else {
        day = 0x0; day |= RTCC_ALARM;
    }

    if (weekday <99) {
        weekday = constrain(weekday, 0, 6);
        weekday = decToBcd(weekday);
        weekday &= ~RTCC_ALARM;
    } else {
        weekday = 0x0; weekday |= RTCC_ALARM;
    }

    alarm_hour = hour;
    alarm_minute = min;
    alarm_weekday = weekday;
    alarm_day = day;

    // First set alarm values, then enable
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)RTCC_ALRM_MIN_ADDR);
    Wire.write((byte)alarm_minute);
    Wire.write((byte)alarm_hour);
    Wire.write((byte)alarm_day);
    Wire.write((byte)alarm_weekday);
    Wire.endTransmission();

    Rtc_Pcf8563::enableAlarm();
}

void Rtc_Pcf8563::clearAlarm()
{
    //set status2 AF val to zero to reset alarm
    status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_TIMER_TF;
    //turn off the interrupt
    status2 &= ~RTCC_ALARM_AIE;

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();
}

/**
* Reset the alarm leaving interrupt unchanged
*/
void Rtc_Pcf8563::resetAlarm()
{
    //set status2 AF val to zero to reset alarm
    status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_TIMER_TF;

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();
}

// true if timer interrupt and control is enabled
bool Rtc_Pcf8563::timerEnabled()
{
    if (getStatus2() & RTCC_TIMER_TIE)
        if (timer_control & RTCC_TIMER_TE)
            return true;
    return false;
}


// true if timer is active
bool Rtc_Pcf8563::timerActive()
{
    return getStatus2() & RTCC_TIMER_TF;
}


// enable timer and interrupt
void Rtc_Pcf8563::enableTimer(void)
{
    getDateTime();
    //set TE to 1
    timer_control |= RTCC_TIMER_TE;
    //set status2 TF val to zero
    status2 &= ~RTCC_TIMER_TF;
    //set AF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_ALARM_AF;
    //enable the interrupt
    status2 |= RTCC_TIMER_TIE;

    // Enable interrupt first, then enable timer
    Wire.beginTransmission(Rtcc_Addr);  // Issue I2C start signal
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_TIMER1_ADDR);
    Wire.write((byte)timer_control);  // Timer starts ticking now!
    Wire.endTransmission();
}


// set count-down value and frequency
void Rtc_Pcf8563::setTimer(byte value, byte frequency, bool is_pulsed)
{
    getDateTime();
    if (is_pulsed)
        status2 |= is_pulsed << 4;
    else
        status2 &= ~(is_pulsed << 4);
    timer_value = value;
    // TE set to 1 in enableTimer(), leave 0 for now
    timer_control |= (frequency & RTCC_TIMER_TD10); // use only last 2 bits

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_TIMER1_ADDR);
    Wire.write((byte)timer_control);
    Wire.write((byte)timer_value);
    Wire.endTransmission();

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();

    enableTimer();
}


// clear timer flag and interrupt
void Rtc_Pcf8563::clearTimer(void)
{
    getDateTime();
    //set status2 TF val to zero
    status2 &= ~RTCC_TIMER_TF;
    //set AF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_ALARM_AF;
    //turn off the interrupt
    status2 &= ~RTCC_TIMER_TIE;
    //turn off the timer
    timer_control = 0;

    // Stop timer first
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_TIMER1_ADDR);
    Wire.write((byte)timer_control);
    Wire.endTransmission();

    // clear flag and interrupt
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();
}


// clear timer flag but leave interrupt unchanged */
void Rtc_Pcf8563::resetTimer(void)
{
    getDateTime();
    //set status2 TF val to zero to reset timer
    status2 &= ~RTCC_TIMER_TF;
    //set AF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_ALARM_AF;

    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT2_ADDR);
    Wire.write((byte)status2);
    Wire.endTransmission();
}

/**
* Set the square wave pin output
*/
void Rtc_Pcf8563::setSquareWave(byte frequency)
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)RTCC_SQW_ADDR);
    Wire.write((byte)frequency);
    Wire.endTransmission();
}

void Rtc_Pcf8563::clearSquareWave()
{
    Rtc_Pcf8563::setSquareWave(SQW_DISABLE);
}

const char *Rtc_Pcf8563::formatTime(byte style)
{
    getTime();
    switch (style) {
        case RTCC_TIME_HM:
            strOut[0] = '0' + (hour / 10);
            strOut[1] = '0' + (hour % 10);
            strOut[2] = ':';
            strOut[3] = '0' + (minute / 10);
            strOut[4] = '0' + (minute % 10);
            strOut[5] = '\0';
            break;
        case RTCC_TIME_HMS:
        default:
            strOut[0] = '0' + (hour / 10);
            strOut[1] = '0' + (hour % 10);
            strOut[2] = ':';
            strOut[3] = '0' + (minute / 10);
            strOut[4] = '0' + (minute % 10);
            strOut[5] = ':';
            strOut[6] = '0' + (sec / 10);
            strOut[7] = '0' + (sec % 10);
            strOut[8] = '\0';
            break;
    }
    return strOut;
}


const char *Rtc_Pcf8563::formatDate(byte style)
{
    getDate();

        switch (style) {

        case RTCC_DATE_ASIA:
            //do the asian style, yyyy-mm-dd
            if (century ){
                strDate[0] = '1';
                strDate[1] = '9';
            }
            else {
                strDate[0] = '2';
                strDate[1] = '0';
            }
            strDate[2] = '0' + (year / 10 );
            strDate[3] = '0' + (year % 10);
            strDate[4] = '-';
            strDate[5] = '0' + (month / 10);
            strDate[6] = '0' + (month % 10);
            strDate[7] = '-';
            strDate[8] = '0' + (day / 10);
            strDate[9] = '0' + (day % 10);
            strDate[10] = '\0';
            break;
        case RTCC_DATE_US:
        //the pitiful US style, mm/dd/yyyy
            strDate[0] = '0' + (month / 10);
            strDate[1] = '0' + (month % 10);
            strDate[2] = '/';
            strDate[3] = '0' + (day / 10);
            strDate[4] = '0' + (day % 10);
            strDate[5] = '/';
            if (century){
                strDate[6] = '1';
                strDate[7] = '9';
            }
            else {
                strDate[6] = '2';
                strDate[7] = '0';
            }
            strDate[8] = '0' + (year / 10 );
            strDate[9] = '0' + (year % 10);
            strDate[10] = '\0';
            break;
        case RTCC_DATE_WORLD:
        default:
            //do the world style, dd-mm-yyyy
            strDate[0] = '0' + (day / 10);
            strDate[1] = '0' + (day % 10);
            strDate[2] = '-';
            strDate[3] = '0' + (month / 10);
            strDate[4] = '0' + (month % 10);
            strDate[5] = '-';

            if (century){
                strDate[6] = '1';
                strDate[7] = '9';
            }
            else {
                strDate[6] = '2';
                strDate[7] = '0';
            }
            strDate[8] = '0' + (year / 10 );
            strDate[9] = '0' + (year % 10);
            strDate[10] = '\0';
            break;

    }
    return strDate;
}

void Rtc_Pcf8563::initClock()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);        // start address

    Wire.write((byte)0x0);     //control/status1
    Wire.write((byte)0x0);     //control/status2
    Wire.write((byte)0x81);     //set seconds & VL
    Wire.write((byte)0x01);    //set minutes
    Wire.write((byte)0x01);    //set hour
    Wire.write((byte)0x01);    //set day
    Wire.write((byte)0x01);    //set weekday
    Wire.write((byte)0x01);     //set month, century to 1
    Wire.write((byte)0x01);    //set year to 99
    Wire.write((byte)0x80);    //minute alarm value reset to 00
    Wire.write((byte)0x80);    //hour alarm value reset to 00
    Wire.write((byte)0x80);    //day alarm value reset to 00
    Wire.write((byte)0x80);    //weekday alarm value reset to 00
    Wire.write((byte)0x0);     //set SQW, see: setSquareWave
    Wire.write((byte)0x0);     //timer off
    Wire.endTransmission();
}

void Rtc_Pcf8563::setTime(byte hour, byte minute, byte sec)
{
    getDateTime();
    setDateTime(getDay(), getWeekday(), getMonth(),
                getCentury(), getYear(), hour, minute, sec);
}

void Rtc_Pcf8563::setDate(byte day, byte weekday, byte month, bool century, byte year)
{
    getDateTime();
    setDateTime(day, weekday, month, century, year,
                getHour(), getMinute(), getSecond());
}

void Rtc_Pcf8563::getDate()
{
    getDateTime();
}

void Rtc_Pcf8563::getTime()
{
    getDateTime();
}

bool Rtc_Pcf8563::getVoltLow(void)
{
    return volt_low;
}

byte Rtc_Pcf8563::getSecond() {
    return sec;
}

byte Rtc_Pcf8563::getMinute() {
    return minute;
}

byte Rtc_Pcf8563::getHour() {
    return hour;
}

byte Rtc_Pcf8563::getAlarmMinute() {
    return alarm_minute;
}

byte Rtc_Pcf8563::getAlarmHour() {
    return alarm_hour;
}

byte Rtc_Pcf8563::getAlarmDay() {
    return alarm_day;
}

byte Rtc_Pcf8563::getAlarmWeekday() {
    return alarm_weekday;
}

byte Rtc_Pcf8563::getTimerControl() {
    return timer_control;
}

byte Rtc_Pcf8563::getTimerValue() {
    // Impossible to freeze this value, it could
    // be changing during read.  Multiple reads
    // required to check for consistency.
    uint8_t last_value;
    do {
        last_value = timer_value;
        getDateTime();
    } while (timer_value != last_value);
    return timer_value;
}

byte Rtc_Pcf8563::getDay() {
    return day;
}

byte Rtc_Pcf8563::getMonth() {
    return month;
}

byte Rtc_Pcf8563::getYear() {
    return year;
}

bool Rtc_Pcf8563::getCentury() {
    return century;
}

byte Rtc_Pcf8563::getWeekday() {
    return weekday;
}

byte Rtc_Pcf8563::getStatus1() {
    return status1;
}

byte Rtc_Pcf8563::getStatus2() {
    return status2;
}
