#ifndef __FUELTANK__
#define __FUELTANK__

#include "OhmSender.h"


class FuelTank : public OhmSender {

public:
    FuelTank(uint8_t pin, float minOhms=0, float maxOhms=90,
             uint8_t capacity=60) : OhmSender(pin, minOhms, maxOhms) {
       this->capacity = capacity; 
    }

    float getLitres() {
        float l = this->calculatePercent() * this->capacity;
        l = min(max(l, 0), this->capacity);
        l = round(l * 10.0) / 10.0;
        return l;
    }

private:
    uint8_t capacity;

};

#endif
