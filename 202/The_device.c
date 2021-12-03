// cab202 assesment 1 The_Device by James Norman n10256989

//captioning this like you're an idiot for top marks sorry in advance.

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "helpers.h" //helpers file....duh
#include "draw_border.h" // contains the function that draws the border and the hub rows.
#include "timers.h" // contains everything relating to the timer. 
#include "vacuum.h" // contains all of the functions relating to the movement of the vacuum.
#include "charging.h" // contains everything relating to the charging action and station.
#include "trash.h" // contains all of the functions relating to trash. 
#include "misc.h" // contains the draw all, loop, reset and command functions.



// Global variables
bool game_over = false; //sets game over to false so game runs 
int delay = 10;
double start_time; // sets start time as ch3 double for timed functions 
bool paused = true; //makes sure game is paused at the start
double battery_life; 
bool charging = false; //makes sure it's not always doing charging things 
bool eating = true; //makes sure device starts off eating trash 
 int load = 0;  //sets load to 0
 int new_dry = 0; //sets the y direction to 0
 double rx, ry, drx, dry;
 double device_dir = M_PI / 2;
 
 // function for colliding 
 bool collided(
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1)
{
    int left0 = round(x0) - w0 / 2, right0 = left0 + w0 - 1;
    int left1 = round(x1) - w1 / 2, right1 = left1 + w1 - 1;

    int top0 = round(y0) - h0 / 2, bottom0 = top0 + h0 - 1;
    int top1 = round(y1) - h1 / 2, bottom1 = top1 + h1 - 1;

    if (right1 < left0 || right0 < left1) return false;
    if (bottom1 < top0 || bottom0 < top1) return false;

    return true;
} 


////////////////MAIN////////////////
// runs the game 
//bored of reading my descriptions yet?
int main (void)
{
  const int DELAY = 10;

  srand(get_current_time ()); 

  setup_screen ();
  setup ();

  while (!game_over)
    {
      loop ();
      timer_pause (DELAY);
    }

  return 0;
}
/////////////////END OF GAME/////////////////////