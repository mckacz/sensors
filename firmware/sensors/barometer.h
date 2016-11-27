#ifndef __BAROMETER_H
#define __BAROMETER_H

#include "sensors.h"

#define BAROMETER_ERR_INIT        1
#define BAROMETER_ERR_TEMP_START  2
#define BAROMETER_ERR_TEMP_READ   3
#define BAROMETER_ERR_PRESS_START 4
#define BAROMETER_ERR_PRESS_READ  5

extern void barometer_begin();
extern void barometer_read(readings_t &readings, options_t &options);

#endif
