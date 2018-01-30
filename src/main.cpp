#include <Tachometer.h>
#include <SevenSegmentTM1637.h>
#include <FastLED.h>
#include <Rotary.h>
#include "Gauge.h"
#include "FuelTank.h"

#define PIN_DISPLAY_CLK_1 A5  // Nano: 4
#define PIN_DISPLAY_DIO_1 A4  // Nano: 5
#define PIN_DISPLAY_CLK_2 A3  // Nano: 6
#define PIN_DISPLAY_DIO_2 A4  // Nano: 5

#define PIN_RPM 2
#define PIN_GAUGE 8
#define PIN_FUEL A0

#define PIN_ROTARY_DT 6   // Nano: 10
#define PIN_ROTARY_CLK 7  // Nano: 11

#define NEOPIXEL_LED_COUNT 24

#define IDLE_RPM 1400.0
#define MAX_RPM 4000.0

SevenSegmentTM1637 displays[2] = {
    {PIN_DISPLAY_CLK_2, PIN_DISPLAY_DIO_2},
    {PIN_DISPLAY_CLK_1, PIN_DISPLAY_DIO_1}
};

Tachometer tachometer(PIN_RPM, 2);  // Boxer-four = 2 sparks per rotation

CRGB leds[NEOPIXEL_LED_COUNT];
Gauge rpmGauge(leds, 12, 21, IDLE_RPM, MAX_RPM, 0xFF8000);
//Gauge speedGauge(leds, 10, 1, 0.0, 100.0, 0xFF8000);

FuelTank tank(PIN_FUEL, 1.8, 103.8);

Rotary rotary(PIN_ROTARY_CLK, PIN_ROTARY_DT);


void spark() {
    tachometer.trigger();
}


void setupDisplay() {
    displays[0].begin();
    displays[0].setBacklight(100);
    displays[0].print("EALY");
    displays[1].begin();
    displays[1].setBacklight(100);
    displays[1].print(" BAY");
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

    tachometer.begin();
    //tachometer.begin(&spark);

    tank.begin();

    pinMode(13, INPUT);  // RPM vs Hz

    Serial.println("------------------------------");
}


char displayBuffer[5];
char rpminfo = 1;
char tankinfo = 1;

void loop() {
    if (tachometer.update()) {
        int rpm;
        //if (digitalRead(13) == HIGH) {
        if (rpminfo == 1) {
            rpm = tachometer.getHZ();
        } else {
            rpm = tachometer.getRPM(true);
        }
        if (rpm >= 10000) {
            rpm = 9999;
        }
        sprintf(displayBuffer, "%4u", rpm);
        displays[0].print(displayBuffer);

        rpmGauge.setValue(rpm);
    }

    if (tank.update()) {
        //if (digitalRead(13) == HIGH) {
        if (tankinfo == 1) {
            int tval = tank.getLitres() * 10;
            sprintf(displayBuffer, "%3uL", tval);
        } else {
            int tval = tank.getPercent();
            sprintf(displayBuffer, "%3uP", tval);
        }
        displays[1].print(displayBuffer);
    }

    unsigned char direction = rotary.process();
    if (direction) {
        if (direction == DIR_CW) {
            rpminfo *= -1;
        } else {
            tankinfo *= -1;
        }
    }
    //delay(100);
}
