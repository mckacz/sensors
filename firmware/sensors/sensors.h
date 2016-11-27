#ifndef __SENSORS_H
#define __SENSORS_H

#include <Arduino.h>

#define PIN_POWER 3
#define PIN_RADIO_CE  7
#define PIN_RADIO_CSN 8

#define ENABLE_BMP180  1
#define ENABLE_TSL2561 2
#define ENABLE_HTU21   4

struct options_t {
    uint8_t  id;             //sensor id (0 - 255)
    uint8_t  channel;        //channel (1 - 125)
    uint8_t  sen_addr[3];    //sensor address
    uint8_t  hub_addr[3];    //hub address
    uint8_t  interval;       //reading interval in seconds
    uint8_t  flags;          //sensors enabling flags
    float    vbat_correct;   //VBAT correction
    float    vreg_correct;   //VREG correction
    uint16_t altitude;       //altitude in meteres (for BMP180)
};

struct readings_t {
    uint8_t  id;             //sensor id (from options)
    uint16_t seq;            //sequence number
    uint8_t  flags;          //flags (from options)
    uint8_t  status;         //reading status
    int16_t  bar_temp;       //[BMP180] temperature (°C * 100)
    double   bar_pres_abs;   //[BMP180] absolute presure (hPa)
    double   bar_pres_rel;   //[BMP180] relative presure (hPa)
    double   light_lux;      //[TSL2561] illumination (lux)
    int16_t  htu_temp;       //[HTU21] temperature (°C * 100)
    uint16_t htu_hum;        //[HTU21] humidity (% * 100)
    uint16_t vbat;           //battery voltage (V * 100)
    uint16_t vreg;           //regulator voltage (V * 100)
};

#endif
