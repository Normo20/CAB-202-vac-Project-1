#ifndef charging_H
#define charging_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "trash.h"
#include "vacuum.h"
#include "misc.h"

void draw_chargingstation ();
 void draw_menu();
void setup_chargingstation();
void at_base();
 void return_to_base();
 void update_battery_life();
 void battery_function();
 void load_carrying_setup();
 void do_load_cheat();
void do_battery_cheat();







#endif