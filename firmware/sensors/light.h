#ifndef __LIGHT_H
#define __LIGHT_H

#include "sensors.h"

#define LIGHT_TIME 2   // light sensor integration time 402ms

#define LIGHT_ERR_READ  8
#define LIGHT_ERR_RANGE 9

extern void light_begin();
extern void light_read(readings_t &readings);

#endif
