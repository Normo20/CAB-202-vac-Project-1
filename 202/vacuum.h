#ifndef vacuum_H
#define vacuum_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "trash.h"
#include "charging.h"
#include "misc.h"


#define DEVICE_WIDTH 10
#define DEVICE_HEIGHT 9
#define DEVICE_SPEED 0.2

#define confine(x,y,z) (x<y?y:x>z?z:x)


void setup_device ();
void start_device ();
void draw_device ();
void update_device ();
void do_collision_device();
bool device_ctrl(int ch) ;
void move_device (int ch);
void do_device_cheat();
void draw_dir();



 
#endif