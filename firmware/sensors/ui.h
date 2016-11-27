#ifndef __UI_H
#define __UI_H

#include "sensors.h"

void ui_setup(options_t& options);
void ui_loop(const readings_t readings);

#endif