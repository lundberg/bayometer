#include <Tachometer.h>
#include <SevenSegmentTM1637.h>
#include <FastLED.h>
#include "Gauge.h"

#define PIN_DISPLAY_CLK 4
#define PIN_DISPLAY_DIO 5
#define PIN_RPM 2
#define PIN_GAUGE 6

#define NEOPIXEL_LED_COUNT 24

#define IDLE_RPM 1400.0
#define MAX_RPM 4000.0

SevenSegmentTM1637 display(PIN_DISPLAY_CLK, PIN_DISPLAY_DIO);
Tachometer tachometer(PIN_RPM, 2);  // Boxer-four = 2 sparks per rotation

CRGB leds[NEOPIXEL_LED_COUNT];
Gauge rpmGauge(leds, 12, 21, IDLE_RPM, MAX_RPM, 0xFF8000);
//Gauge speedGauge(leds, 10, 1, 0.0, 100.0, 0xFF8000);


void spark() {
    tachometer.trigger();
}


void setupDisplay() {
    display.begin();
    display.setBacklight(100);
    display.print("BAY");
    delay(1000);
}


void setupNeoPixels() {
    FastLED.addLeds<NEOPIXEL, PIN_GAUGE>(leds, NEOPIXEL_LED_COUNT);
    FastLED.setBrightness(16);
    FastLED.clear(true);
}


void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    Serial.println("\r\n\r\nBooting...");

    setupDisplay();
    setupNeoPixels();

    tachometer.begin(&spark);

    pinMode(13, INPUT);  // RPM vs Hz

    Serial.println("------------------------------");
}


char displayBuffer[5];

void loop() {
    if (tachometer.update()) {
        int rpm;
        if (digitalRead(13) == HIGH) {
            rpm = tachometer.getHZ();
        } else {
            rpm = tachometer.getRPM(true);
        }
        if (rpm >= 10000) {
            rpm = 9999;
        }
        sprintf(displayBuffer, "%4u", rpm);
        display.print(displayBuffer);

        rpmGauge.setValue(rpm);
    }
}
