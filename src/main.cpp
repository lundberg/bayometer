#include <Tachometer.h>
#include <SevenSegmentTM1637.h>

#define PIN_CLK 4
#define PIN_DIO 5
#define PIN_RPM 2

SevenSegmentTM1637 display(PIN_CLK, PIN_DIO);
Tachometer tachometer(PIN_RPM, 4);


void spark() {
    tachometer.trigger();
}


void setupDisplay() {
    display.begin();
    display.setBacklight(100);
    display.print("BAY");
    delay(1000);
}


void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    Serial.println("\r\n\r\nBooting...");

    setupDisplay();
    tachometer.begin(&spark);

    Serial.println("------------------------------");
}


char displayBuffer[5];

void loop() {
    if (tachometer.update()) {
        int rpm = tachometer.getRPM(true);
        if (rpm >= 10000) {
            rpm = 9999;
        }
        sprintf(displayBuffer, "%4u", rpm);
        display.print(displayBuffer);
    }
}
