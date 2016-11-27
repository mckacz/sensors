#ifndef __HUM_H
#define __HUM_H

#include "sensors.h"

#define STATUS_ERR_HUM 16

extern void htu_begin();
extern void htu_read(readings_t &readings);

#endif
