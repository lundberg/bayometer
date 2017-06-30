#ifndef __GAUGE__
#define __GAUGE__

#include <Arduino.h>
#include <FastLED.h>


class Gauge {

public:
    Gauge(struct CRGB *leds, uint8_t fromLED, uint8_t toLED,
            uint16_t minValue, uint16_t maxValue, long color);

    void setValue(uint16_t value);

private:
    struct CRGB *leds;
    uint8_t direction;
    float percentPerLED;
    uint8_t percent = 0;

    uint8_t fromLED;
    uint8_t toLED;
    uint16_t minValue;
    uint16_t maxValue;
    long color;

};

#endif
