/********************************************************
 * This example shows how the watchdog interrupt works. *
 * About every 4 seconds there will be a wdt interrupt. *
 * If you uncomment the line containing wdt_reset() the *
 * sketch will not detect wdt interrupt signal.         *
 ********************************************************
 * When writing an Interrupt Service Routine (ISR):     *
 * Keep short,don't use delay(), don't do serial prints *
 * Make variables shared with the main code volatile    *
 * Read this: https://gammon.com.au/interrupts          *
 ********************************************************/
#include <WDT.h>

volatile boolean isrflag;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.begin(19200);
    Serial.println(F("Skech started."));
    isrflag = false;
    wdt_ienable(WTO_4S);
}

void loop() {
    int n = 0;
    do {
        Serial.print(F("    Elapsed time: "));
        Serial.print(n);
        Serial.println(" s");
        delay(999);
        if (digitalRead(LED_BUILTIN)) 
            digitalWrite(LED_BUILTIN, LOW );
        else
            digitalWrite(LED_BUILTIN, HIGH);
        if (isrflag) {
            Serial.println(F("--There was a wdt interrupt.--"));
            isrflag = false;
            wdt_ienable(WTO_4S);
        }
      //wdt_reset();
    } while( n++ < 1000 );
}

ISR (WDT_vect)
{
    isrflag = true;
    wdt_reset();
}
