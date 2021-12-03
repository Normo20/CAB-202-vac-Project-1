#ifndef trash_H
#define trash_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "charging.h"
#include "vacuum.h"
#include "misc.h"



void draw_trash(int i);
void draw_trashs();
void setup_trash(int i, double x1, double y1);
void setup_trashs() ;
void check_collision_trash() ;
void draw_slime(int i); 
void draw_slimes() ;
void setup_slime(int i, double x, double y);
void setup_slimes();
void check_collision_slime();
void draw_dust(int i) ;
void draw_dusts();
void setup_dust(int i, double x2, double y2);
void setup_dusts();
void check_collision_dust();
void do_slime_cheat() ;
void do_dust_cheat();
void do_trash_cheat();








#endif