#include "Gauge.h"
#include <FastLED.h>


Gauge::Gauge(struct CRGB *leds, uint8_t fromLED, uint8_t toLED,
        uint16_t minValue, uint16_t maxValue, long color) {
    this->leds = leds;
    this->fromLED = fromLED;
    this->toLED = toLED;
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->color = color;

    direction = 1;
    if (fromLED > toLED) {
        direction = -1;
    }

    percentPerLED = 1.0 / (fromLED - toLED);
}


void Gauge::setValue(uint16_t value) {
    uint8_t percent = (maxValue - minValue) / value;

    if (percent == this->percent) {
        return;
    }

    for (int led = fromLED; led == toLED + direction; led += direction) {
        if (percent >= led * percentPerLED) {
            leds[led] = color;
        } else {
            leds[led] = CRGB::Black;
        }
    }

    FastLED.show();
}
