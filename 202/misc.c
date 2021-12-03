#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "charging.h"
#include "vacuum.h"
#include "trash.h"
#include "timers.h"
#include "draw_border.h"

// global variables 
int timeout;
bool game_over; 
int delay;
double start_time; 
bool paused;
double battery_life; 
bool charging; 
bool eating; 
 int load; 
 int new_dry; 
 double rx, ry, drx, dry;
 double device_dir;

// draws everything in the game/ simulation
void draw_all()
{
	int w, h;
	get_screen_size(&w, &h);
	clear_screen(); // clears screen 
	draw_border(); // draws border 
	timer(); // draws the timer 
	draw_device(); // draws the vacuum

	draw_chargingstation(); 
	if (charging) at_base();
	battery_function(); // if its at base it charges the battery and dumps the load

// I really don't need to keep doing this do I?
	draw_trashs(); 
	draw_slimes();
	draw_dusts(); 
	draw_dir();
	draw_menu();
	show_screen();

}



////SETUP/////////////
//sets stuff in the simulation up 
void setup(void)
{
	start_time = get_current_time();
	setup_device();
	battery_life = 100; // sets the starting battery life to 100%
	setup_chargingstation();  
	setup_trashs();
	setup_slimes();
	setup_dusts();
	draw_all();
}

















// changes the delay in milliseconds to whatever the user desires 
void do_delay_cheat() {
  delay = get_int("New delay (milliseconds)?"); 
  int delay = 100; 

    if (delay < 0) delay = 10;
}









// prints the help screen with handy tips 
void do_help() {
     int w, h;
	 get_screen_size(&w, &h); 
	 clear_screen(); 
	 draw_formatted_lines(0,0,
		  "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
                        "!!!    Vacuum game    !!!\n"
                        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "\n"
        "i, j, k, l: Move vacuum left, down, right, up \n"
		"d, s, t,: lets you put any type of rubbish on the floor where you want! \n"
        "r: restart\n"
        "o: set ch3 time out for ch3 specified number of milliseconds chosen by you \n"
        "v: vacuum cheat - lets you override the position and direction of the vacuum\n"
        "p: pause or resume zombie movement\n"
		"w: lets you choose any positive value as the vacuums new weight in grams \n"
        "m: change the millisecond loop delay\n"
        "q: quit\n"
		"y: lets you choose how much battery in % you would like right now \n"
        "?: display this help screen & also exits it \n\n\n"
        "Press any key to continue playing the game :) \n"
	 ); 
	 show_screen(); 
    
    while (get_char() > 0) {}
    while (!timed_out() && wait_char() <= 0) {}
}
////////////LOOP FOR MAIN AND CONTROLS////////////////////
// the loop loops the functions 
void loop()
{
	int ch = get_char();
int w, h;
    get_screen_size(&w, &h);
	// prints the game over screen if game is quit
	if (ch == 'q') {
		clear_screen();
		 draw_formatted_lines(w/2-20, h/2-10,
		                "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
                        "!!!  Game over... goodbye:) !!!\n"
                        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "\n");
		
		show_screen();
		timer_pause(1500);
		
		
		game_over = true;
		return;
	}
	
	// resets the game 
	else if (ch == 'r') {
		clear_screen();
		reset();
	}
	// timeout called
	else if (ch == 'o') {
		do_timeout();
	}
	// does the device cheat 
	else if (ch == 'v') {
		do_device_cheat();
	}
	// does the delay cheat 
	else if (ch == 'm') {
		do_delay_cheat();
	}
	
	// brings up the help screen 
	else if (ch == '?') {
		do_help();
	}
	
	// executes the slime cheat
	else if (ch == 's') {
		do_slime_cheat();
	}
	// executes the trash cheat
	else if (ch == 't') {
		do_trash_cheat();
	}
	// executes the dust cheat
	else if (ch == 'd') {
		do_dust_cheat();
	}
	//executes the load cheat
	else if (ch == 'w') {
		do_load_cheat();
	}
	//executes the battery cheat 
	else if (ch == 'y') {
		do_battery_cheat(); 
	}
	//executes the back to base command.... sorta -_-
	else if (ch == 'b') {
			return_to_base();
			update_battery_life();
			battery_function();
			load_carrying_setup();
			at_base();
			
	}
//pauses game
	else if (ch == 'p') {
		paused = !paused;

		if (!paused) {
			start_device();
		}
	}

	if (paused) {} //does nothing 
	else {
		if (!charging) {
			update_device();
			move_device(ch);
			update_battery_life();
			battery_function();
			load_carrying_setup();
			do_collision_device();
			
		}
		else {
			at_base(); 
		}

	}
	check_collision_trash();
	check_collision_slime();
	check_collision_dust();  
	draw_all();
}


//resets the function by calling on the setup again 
void reset() {
	setup();
}