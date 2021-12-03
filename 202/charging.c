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
// global variables.......... again
double rx, ry, drx, dry;
double cx, cy; 
 bool charging; 
 bool eating; 
 int load; 
double battery_life; 

#define DEVICE_WIDTH 10
#define DEVICE_HEIGHT 9
#define DEVICE_SPEED 0.2

//defining Charging Station dimensions
#define CS_WIDTH 10
#define CS_HEIGHT 5 

// draws the charging station using # symbols and using the width and height defined above 
void draw_chargingstation ()
{
  int w, h;
  get_screen_size (&w, &h);


  int left = round (cx) - CS_WIDTH / 2;
  int right = left + CS_WIDTH - 1;

  int top = round(cy) - CS_HEIGHT/2;
  int bottom = top + CS_HEIGHT - 1;

  const int cs = '#';
  draw_line (left, top, right, top, cs);
  draw_line (left, top, left, bottom, cs);
  draw_line (right, top, right, bottom, cs);
  draw_line (left, bottom, right, bottom, cs);
}

// prints the load hud
void draw_menu(){
	int w, h;
  get_screen_size (&w, &h);
	draw_string(round(w / 3) + 3, 3, "g  weight "); // prints weight currently in vacuum 
	draw_int(round(w / 3) + 1, 3, load);
	
	
	
}

// sets up the charging stations position
void setup_chargingstation() {
	int w, h;
	get_screen_size(&w, &h);
	cx = w / 2;
	cy = 7;
}


// gives the vacuum directions for when its at base 

void at_base() {
	int w, h;
	get_screen_size(&w, &h);
	charging = true;
	draw_string(w/2-93, h/2+29, "DOCKED"); // prints docked in the input bar
	if (charging) {
		battery_life += 3.5*(10.0 / 300.0); //charges the battery %
        load=0;		// dumps the load...

		if (battery_life >= 100 && load <=45) { 
			charging = false; //leave base via collision when battery is charged and load is dumped 
			eating = true; 
		}
	}
} 


//returns the vacuum back to base while not picking up trash 
 void return_to_base() {
	eating = false; //stop eating rubbish when going to base

	int w, h;
	get_screen_size(&w, &h);
	double t1 = w / 2 - rx;
	double t2 = 10 - (ry - DEVICE_HEIGHT / 2);
	double d = sqrt(t1*t1 + t2 * t2);
	drx = t1 * DEVICE_SPEED / d;
	dry = t2 * DEVICE_SPEED / d;
	double new_y_1 = round(ry + dry); 
	double new_x_1 = round(rx + drx); 

	if (round(new_y_1) - DEVICE_HEIGHT / 2 <= 11 && round(new_x_1) == w / 2) { //orignal posiiton 
		at_base();	
	}
}







////////BATTERY LIFE///////////////////////
void update_battery_life() {
	battery_life -= 1.5 / 100;
} 










// prints out the battery life and also prints the game over menu when battery reaches 0%
void battery_function(){
	int w, h;
	get_screen_size(&w, &h);
	// returns vacuum to base if battery % goes under 25%
	if(battery_life <= 25){	
		  return_to_base(); 
      }
     // tells us when battery % hits 0
      if(battery_life <= 0){
		  battery_life = 0; 
		  //prints box
	draw_line (w/2-20,h/2+3,w/2+20,h/2+3,'-');
    draw_line (w/2-20,h/2-3,w/2+20,h/2-3,'-');
	draw_line (w/2+20,h/2+3,w/2+20,h/2-3,'|');
	draw_line (w/2-20,h/2-3,w/2-20,h/2+3,'|');		
	//prints options 
	draw_string(w/2-18,h/2-2, "Simulation over");
	draw_string (w/2-18,h/2-1, "press Q to quit ");
	draw_string (w/2-18, h/2, "or");
	draw_string (w/2-18, h/2+1, "press R to restart");
	
	
	//stops Vacuum from moving when at 0%
		 dry = 0; 
		 drx = 0; 

      }
  const char *blife = {
	  "Battery Life:"
  };
  // prints the battery life caption and current %
  draw_formatted((w * 2 / 3) + 1, 1, blife);
  draw_char((w * 2 / 3) + 17, 1, '%');
  draw_int(round(w * 2 / 3) + 14, 1, battery_life);
  }
  
  
  
  
  
  // returns vacuum to base while not eating when load gets over 45g 
  void load_carrying_setup() {
	int w, h;
	get_screen_size(&w, &h);

	if (load > 45) {
		return_to_base();
		eating= false;
	
		
	}
	
}

// gets a new load for the vacuum selected by the user 
void do_load_cheat() {
	load = get_int("What ~positive~ weight is the vacuum holding?"); 
}


// gets a new battery % for the vacuum selected by the user 
void do_battery_cheat() {
	battery_life = get_int("Chose your battery's charge level: "); 
}


