/****************************************************
 * This example shows how the watchdog reset works. *
 * It restarts every 4 seconds. If you uncomment    *
 * the line containing wdt_reset() the sketch will  *
 * run continuously.                                *
 ****************************************************/
#include <WDT.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.begin(19200);
    Serial.println(F("Skech started."));
    wdt_enable(WTO_4S);
}

void loop() {
    int n = 0;
    do {
        Serial.print(F("Elapsed time: "));
        Serial.print(n);
        Serial.println(" s");
        delay(499);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(499);
        digitalWrite(LED_BUILTIN, LOW);
      //wdt_reset();
    } while( n++ < 1000 );
}
