#include <Arduino.h>
#include <SparkFunTSL2561.h>
#include "light.h"

SFE_TSL2561 light;
uint16_t light_delay;

void light_begin() {
    light.begin();
}

void light_read(readings_t &readings, boolean gain) {
    light.setTiming(gain, LIGHT_TIME, light_delay);
    light.setPowerUp();

    delay(light_delay + 250);

    uint16_t data0, data1;

    if (light.getData(data0, data1)) {
        double lux;
        boolean good;

        good = light.getLux(gain, light_delay, data0, data1, lux);
        readings.light_lux = lux;

        if (!good) {
            readings.status |= LIGHT_ERR_RANGE;
        }
    }
    else {
        readings.status |= LIGHT_ERR_READ;
    }
}

void light_read(readings_t &readings) {
    light_read(readings, 1);

    if(readings.status & LIGHT_ERR_RANGE) {
        readings.status = readings.status  - LIGHT_ERR_RANGE;

        light_read(readings, 0);
    }
}