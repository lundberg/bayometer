#include "OhmSender.h"


OhmSender::OhmSender(uint8_t pin, float minOhms, float maxOhms) {
    this->pin = pin;
    this->minOhms = minOhms;
    this->maxOhms = maxOhms;
}


void OhmSender::begin() {
    pinMode(this->pin, INPUT);

    float ohms = this->readOhms();
    for (uint8_t i = 0; i < OHM_SAMPLE_COUNT; i++) {
        this->samples[i] = ohms;
    }
}


bool OhmSender::update() {
    bool updated = false;

    float ohms = this->readOhms();
    ohms = this->sample(ohms);

    if (this->ohms != ohms) {
        this->ohms = ohms;
        updated = true;
    }

    return updated;
}


float OhmSender::getOhms() {
    return round(this->ohms * 100) / 100;
}


uint8_t OhmSender::getPercent() {
    float percent = this->calculatePercent() * 100.0;
    percent = min(max(percent, 0), 100);
    return round(percent);
}
