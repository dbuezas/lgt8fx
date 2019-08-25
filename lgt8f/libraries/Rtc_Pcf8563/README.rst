
This is my fixed/expanded version of the original Rtc_Pcf8563 library for Arduino.
All credits go to the original authors.

My version differs for

* a few more methods, mostly useful for debugging
* a single bug fix in RTCC_ALARM_AF


NAME
----
Pcf8563 Real Time Clock support routines

AUTHOR
------
Joe Robertson, jmr
orbitalair@bellsouth.net
http://orbitalair.wikispaces.com/Arduino

CREATION DATE
-------------
9/24/06,  init - built off of usart demo.  using mikroC

NOTES
-----

HISTORY
-------

* 10/14/06 ported to CCS compiler, jmr
* 2/21/09  changed all return values to hex val and not bcd, jmr
* 1/10/10  ported to arduino, jmr
* 2/14/10  added 3 world date formats, jmr
* 28/02/2012 A. Pasotti
   * fixed a bug in RTCC_ALARM_AF,
   * added a few (not really useful) methods


TODO
----

Add Euro date format
Add short time (hh:mm) format
Add 24h/12h format
Add timer support