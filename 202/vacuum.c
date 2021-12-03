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

//Definining Vacuum State 
#define DEVICE_WIDTH 10
#define DEVICE_HEIGHT 9
#define DEVICE_SPEED 0.2
//charging station
#define CS_WIDTH 10
#define CS_HEIGHT 5 

#define confine(x,y,z) (x<y?y:x>z?z:x)
//sets all the global variables (nessecery??)
bool charging; 
int new_dry; 
double rx, ry, drx, dry;
double device_dir;
double cx, cy; 
bool collided (
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1);



// sets up the vacuums starting position and starts it going straight down
void setup_device ()
{
  int w, h;
  get_screen_size (&w, &h);
  device_dir = M_PI/2; 

  rx = w / 2;
  ry = h / 2;
}
// gets vacuum moving with desired vacuums speed.
void start_device ()
{
  const double step = DEVICE_SPEED;

  drx = step * cos (device_dir);
  dry = step * sin (device_dir);
}
// draws the vacuum using the ch3 which is a @ symbol 
void draw_device (){
  const int ch3 = '@';

  int left = round (rx) - DEVICE_WIDTH/2;
  int right = left + DEVICE_WIDTH - 1;

  int top = round(ry) - DEVICE_HEIGHT/2;
  int bottom = top + DEVICE_HEIGHT - 1;

  draw_line(left, top + 3, left, bottom - 3, ch3);
  draw_line (left + 1, top + 2, left + 1, bottom - 2, ch3);
  draw_line (left + 2, top + 1, left + 2, bottom - 1, ch3);
  draw_line (left + 3, top, left + 3, bottom, ch3);
  draw_line (left + 4, top, left + 4, bottom, ch3); 
  draw_line (left + 5, top, left + 5, bottom, ch3);
  draw_line (left + 6, top, left + 6, bottom, ch3);  
  draw_line (left + 7, top + 1, left + 7, bottom - 1, ch3);
  draw_line (right - 1, top + 2, right - 1, bottom - 2, ch3);
  draw_line (right, top + 3, right, bottom - 3, ch3);
}
//updates the vacuums position with the random bounces, also tells game when the robot bounces
void update_device (){

  int new_x = round (rx + drx);
  int new_y = round (ry + dry);

  bool bounced = false; 
  
  if (new_x == 5 || new_x + DEVICE_WIDTH - 5 == screen_width ()){
          timer_pause(1); 
          device_dir = rand() * 2 * M_PI / RAND_MAX;
          drx = DEVICE_SPEED * cos (device_dir); 
          drx = DEVICE_SPEED * cos (device_dir);    
          // horizontal bounce
          bounced = true;
        }

  if ( new_y == 8 || new_y + DEVICE_HEIGHT - 2  == screen_height ()){
          timer_pause(1); 
          device_dir= rand() * 2 * M_PI / RAND_MAX;
          drx = DEVICE_SPEED * cos (device_dir); 
          dry = DEVICE_SPEED* sin (device_dir);
          // vertical bounce
          bounced = true; 
        } 

  if (!bounced){      // chooses new direction if vacuum bounces 

    rx += drx;
    ry += dry;
  }
	new_dry = 90*dry/0.2; 
}

void do_collision_device() {
	int w, h;
	get_screen_size(&w, &h);
	/////////////DEVICE -- CHARGING STATION COLLISION///////////////////////////////
		if (collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, cx, cy, CS_WIDTH, CS_HEIGHT) && !charging){
			device_dir = rand() * 2 * M_PI / RAND_MAX;
			timer_pause(1);  //pause for one time slice 
			drx = -DEVICE_SPEED * cos(device_dir);
			dry = -DEVICE_SPEED * sin(device_dir);
			return;
		} //this should be an angle between 30/60 degrees left/right
	} 


// manual controls for vacuum 
bool device_ctrl(int ch) {
    return (ch == 'j' || ch == 'l' || ch == 'k' || ch == 'i');

}

void move_device (int ch){
  if (ch == 'j' && rx - DEVICE_WIDTH / 2 > 2) //moves vacuum left
    {
      rx--;
    }
  else if (ch == 'l' && rx + DEVICE_WIDTH / 2 < screen_width () - 2) //moves vacuum right
    {
      rx++;
    }
  else if (ch == 'k' && ry + DEVICE_HEIGHT / 2 < screen_height () - 5) //moves vacuum down
    {
      ry++;
    }
  else if (ch == 'i' && ry - DEVICE_HEIGHT / 2 > 6) //moves vacuum up
    {
      ry--;
    }
}



void do_device_cheat(){
	int w, h;
	get_screen_size(&w, &h);

	ry = get_int("Choose a new y position:");   // changes y position of vacuum to whatever user would like 
	if (ry < 6 - DEVICE_HEIGHT/2 || ry > h - 3) {
		ry = h / 2; 
	}
	rx = get_int("Choose a new x position:"); // changes x position of vacuum to whatever user would like 
	if (rx < 1 + DEVICE_WIDTH || rx > w - DEVICE_WIDTH) {
		rx = w / 2; 
	}
	device_dir = get_int("Choose a new direction: "); // changes direction of vacuum to whatever user would like 
}




void draw_dir(){
	int w, h;
  get_screen_size (&w, &h);
	
	draw_int(round(w / 3) + 1, 1, new_dry); // prints vacuums direction
	
}
