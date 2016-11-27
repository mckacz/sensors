#include <Arduino.h>
#include <SparkFunHTU21D.h>
#include "htu.h"

HTU21D sensor;

void htu_begin() {
    sensor.begin();
}

void htu_read(readings_t &readings) {
    readings.htu_hum = sensor.readHumidity() * 100.0;
    readings.htu_temp = sensor.readTemperature() * 100.0;

    if(readings.htu_hum > 30000 || readings.htu_temp > 30000) {
        readings.status |= STATUS_ERR_HUM;
    }
}
