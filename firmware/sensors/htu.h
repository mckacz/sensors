#ifndef __HUM_H
#define __HUM_H

#include "sensors.h"

#define HUM_READ_ERR 128

extern void htu_begin();
extern void htu_read(readings_t &readings);

#endif
