#ifndef __OHMSENDER__
#define __OHMSENDER__

#include <Arduino.h>
#include "Median.h"

#define OHM_VCC 5.0
#define OHM_DIODE_FV 0.650  // Diode forward voltage, e.g. volatage drop
#define OHM_R1 1000.0
#define OHM_R2 0.0
#define OHM_SAMPLE_COUNT 15


class OhmSender {

public:
    OhmSender(uint8_t pin, float minOhms=0, float maxOhms=0);

    void begin();
    bool update();

    float getOhms();
    uint8_t getPercent();

protected:
    uint8_t pin;

    float ohms = 0;
    float samples[OHM_SAMPLE_COUNT];
    uint8_t sampleIndex = 0;

    float minOhms;
    float maxOhms;

    float readOhms() {
        float vOut = analogRead(pin) * OHM_VCC / 1023.0;
        float ratio = (vOut - OHM_DIODE_FV) / (OHM_VCC - OHM_DIODE_FV);
        float ohms = ((ratio * (OHM_R1 + OHM_R2)) - OHM_R2) / (1 - ratio);
        return ohms;
    }

    float sample(float ohms) {
        samples[sampleIndex++] = ohms;
        if (sampleIndex >= OHM_SAMPLE_COUNT) {
            sampleIndex = 0;
        }

        // Calculate median sample
        long stats[OHM_SAMPLE_COUNT];
        for (uint8_t i = 0; i < OHM_SAMPLE_COUNT; i++) {
            stats[i] = samples[i] * 10;
        }
        long milliohms = quick_select_median(stats, OHM_SAMPLE_COUNT);

        return milliohms / 10.0;

        //float averageOhms = 0;
        //for (uint8_t i = 0; i < OHM_SAMPLE_COUNT; i++) {
            //averageOhms += samples[i];
        //}

        //averageOhms /= OHM_SAMPLE_COUNT;
        //return averageOhms;
    }

    float calculatePercent() {
        return (this->ohms - this->minOhms) / (this->maxOhms - this->minOhms);
    }

private:
    long readVcc() {
        // Read 1.1V reference against AVcc

        // Set the ref to Vcc and the measurement to the internal 1.1V ref
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);
#else
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif  

        delay(2); // Wait for Vref to settle
        ADCSRA |= _BV(ADSC); // Start conversion
        while (bit_is_set(ADCSRA,ADSC)); // measuring

        uint8_t low  = ADCL; // Must read ADCL first - it then locks ADCH  
        uint8_t high = ADCH; // Unlocks both

        long result = (high<<8) | low;

        // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
        result = 1125300L / result;

        return result; // Vcc in millivolts
    }

    float readVoltage() {
        uint16_t value = analogRead(this->pin);
        float vcc = readVcc() / 1000.0;
        float voltage = value * vcc / 1024.0;
        return voltage;
    }

};

#endif
