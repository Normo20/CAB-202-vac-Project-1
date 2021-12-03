#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
double start_time; // sets start time as ch3 double for timed functions 
int timeout = INT_MAX;
void timer() {

	int T=get_current_time();
	int w, h;
    get_screen_size(&w, &h);
//counts the time from the start of the game
	if ( T-start_time<60){ 
	draw_int (w-w+2,h-h+3,(T-start_time)/60); // divides into minutes 
	draw_string (w-w+3,h-h+3,":");
	draw_int (w-w+4,h-h+3,T-start_time); // get_current_time - start time gets us the seconds since the simulation started.
	draw_string (w-w+7,h-h+3,"time mm:ss"); // prints the caption of mm:ss
	//prints caption
	}
	if (T-start_time>60 && T-start_time<120){ // loops the seconds counter at 60
		draw_int (w-w+2,h-h+3,(T-start_time)/60);
	draw_string (w-w+3,h-h+3,":");
	draw_int (w-w+4,h-h+3,T-start_time-60);
	draw_string (w-w+7,h-h+3,"time mm:ss");
	}
	if (T-start_time>120 && T-start_time<180){  // loops the seconds counter at 120
		draw_int (w-w+2,h-h+3,(T-start_time)/60);
	draw_string (w-w+3,h-h+3,":");
	draw_int (w-w+4,h-h+3,T-start_time-120);
	draw_string (w-w+7,h-h+3,"time mm:ss");
}
	if (T-start_time>180){
		draw_int (w-w+2,h-h+3,(T-start_time)/60); // loops the seconds counter at 180
	draw_string (w-w+3,h-h+3,":");
	draw_int (w-w+4,h-h+3,T-start_time-180);
	draw_string (w-w+7,h-h+3,"time mm:ss");
}

}

// sets a timeout counter if the user wishes 
void do_timeout() {
	timeout = get_int("How many seconds shall the program run?"); 

}
//times the game out after a certain period of time.
bool timed_out() {
    return get_current_time() - start_time >= timeout;
}