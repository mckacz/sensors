#include <Arduino.h>
#include <SFE_BMP180.h>
#include "barometer.h"

SFE_BMP180 barometer;
boolean bmp_ready;

void barometer_begin() {
    bmp_ready = barometer.begin();
}

void barometer_read(readings_t &readings, options_t &options) {
    uint8_t status;
    double T, P;

    if(!bmp_ready) {
        readings.status |= BAROMETER_ERR_INIT;
        return;
    }

    status = barometer.startTemperature();

    if (status == 0) {
        readings.status |= BAROMETER_ERR_TEMP_START;
    } else {
        delay(status);
        status = barometer.getTemperature(T);

        if (status == 0) {
            readings.status |= BAROMETER_ERR_TEMP_READ;
        } else {
            readings.bar_temp = (uint16_t)(T * 100.0);
            status = barometer.startPressure(3);

            if (status == 0) {
                readings.status |= BAROMETER_ERR_PRESS_START;
            } else {
                delay(status);
                status = barometer.getPressure(P, T);

                if (status == 0) {
                    readings.status |= BAROMETER_ERR_PRESS_READ;
                } else {
                    readings.bar_pres_abs = P;
                    readings.bar_pres_rel = barometer.sealevel(P, options.altitude);
                }
            }
        }
    }
}
