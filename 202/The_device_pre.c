#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"

// Global variables
bool game_over = false; //sets game over to false so game runs 
int delay = 10;
int timeout = INT_MAX;
double start_time; // sets start time as a double for timed functions 
bool paused = true; //makes sure game is paused at the start
double device_dir = M_PI / 2; //set as global because it will change.... 
int new_dry = 0; // to display the speed/direction (for testing)
double battery_life; 
int Ts;
int Tm;
bool bounced = false;
bool charging = false; 
bool eating = true; 
int load = 0; 
int got_to_base;
int left_base; 
double bl; 

//defining Charging Station dimensions
#define CS_WIDTH 10
#define CS_HEIGHT 5 

double cx, cy; 

//Definining Vacuum State 
#define DEVICE_WIDTH 10
#define DEVICE_HEIGHT 9
#define DEVICE_SPEED 0.2

#define confine(x,y,z) (x<y?y:x>z?z:x)

double rx, ry, drx, dry;

//double rx, ry are robots coordinates 

//Defining SLIME STATE
#define MAX_SLIME 7
#define SLIME_WIDTH 5
#define SLIME_HEIGHT 5

int num_slime = 0;

double sx[MAX_SLIME], sy[MAX_SLIME];

//Defining TRASH STATE 
#define MAX_TRASH 6
#define TRASH_WIDTH 11
#define TRASH_HEIGHT 6

int num_trash = 0;

double tx[MAX_TRASH], ty[MAX_TRASH];

/// DEFINING DUST STATE
#define MAX_DUST 1000
#define DUST_WIDTH 1
#define DUST_HEIGHT 1 

int num_dust = 0;

double dx[MAX_DUST], dy[MAX_DUST];

///////SET UP BOUNDING BOX COLLISION////////////////

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

//////DRAWING BORDER////////////////////
void
draw_border ()
{
   int w, h;
  get_screen_size (&w, &h);
  //Draw border - horizontal 

  int chh = '-';
  draw_line (0, 0, w - 1, 0, chh);
  draw_line (0, h - 3, w - 1, h - 3, chh);
  draw_line (0, h - 1, w - 1, h - 1, chh);
  draw_line (0, 2, w - 1, 2, chh);  //table row 1
  draw_line (0, 4, w - 1, 4, chh);  //table row 2

  //Draw border - Vertical 
  int chv = '|';
  draw_line (0, 0, 0, h - 1, chv);
  draw_line (w - 1, 0, w - 1, h - 1, chv);
  draw_line (round (w / 3), 0, round (w / 3), 4, chv);  //table column 1
  draw_line (round (w * 2 / 3), 0, round (w * 2 / 3), 4, chv);  //table column 2 
  
  //intersection points (excluding the table) 
  const int chi = '+';
  draw_char (0, 0, chi);
  draw_char (0, h - 3, chi);
  draw_char (0, h - 1, chi);
  draw_char (w - 1, 0, chi);
  draw_char (w - 1, h - 3, chi);
  draw_char (w - 1, h - 1, chi);
  draw_char (round (w / 3), 0, chi);
  draw_char (round (2 * w / 3), 0, chi);

  /////////////OTHER INFO////////////////////////////////
  draw_string (1, 1, " SID: n10256989"); //prints student id 
  draw_string (round (w * 2 / 3) + 1, 3, "dust,slime,trash"); // prints the dust slime and trash headers
  draw_string (w/2-25, 1, "degrees"); // prints the degrees header
  
  
  
}

void clock() {

	int T=get_current_time();
	int w, h;
    get_screen_size(&w, &h);
//counts the time from the start of the game
	if ( T-start_time<60){
	draw_int (w-w+2,h-h+3,(T-start_time)/60); // divides into minutes 
	draw_string (w-w+3,h-h+3,":");
	draw_int (w-w+4,h-h+3,T-start_time);
	draw_string (w-w+7,h-h+3,"time mm:ss");
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
////////BATTERY LIFE///////////////////////
void update_battery_life() {
	battery_life -= 1.5 / 100;
} 


// sets what to do when vacuum reaches the base
void at_base() {
	int w, h;
	get_screen_size(&w, &h);
	charging = true;
	draw_string(w/2-93, h/2+29, "DOCKED"); // prints docked in the input bar
	if (charging) {
		battery_life += 3.5*(10.0 / 300.0);
        load=0;		

		if (battery_life >= 100 && load <=45) {
			charging = false; //leave base via collision 
			eating = true; 
		}
	}
} 




 //returns the vacuum back to base while not picking up trash 
 void return_to_base() {
	eating = !eating; //stop eating rubbish when going to base

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


// prints out the battery life and also prints the game over menu when battery reaches 0%
void draw_battery(){
	int w, h;
	get_screen_size(&w, &h);
	
	if(battery_life <= 25){	
		  return_to_base(); 
      }
     
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
	
	
	//stops Vacuum from moving 
		 dry = 0; 
		 drx = 0; 

      }
  const char *blife = {
	  "Battery Life:"
  };
  draw_formatted((w * 2 / 3) + 1, 1, blife);
  draw_char((w * 2 / 3) + 17, 1, '%');
  draw_int(round(w * 2 / 3) + 14, 1, battery_life);
  }

///////////////CHARGING STATION//////////////////
void setup_chargingstation() {
	int w, h;
	get_screen_size(&w, &h);
	cx = w / 2;
	cy = 7;
}
// draws the charging station as specified 
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

///////////ROBOT VACUUM ////////////////////

//sets all the parameters
void
setup_device ()
{
  int w, h;
  get_screen_size (&w, &h);
  device_dir = M_PI/2; 

  rx = w / 2;
  ry = h / 2;
}
// gets vacuum moving
void
start_device ()
{
  const double step = DEVICE_SPEED;

  drx = step * cos (device_dir);
  dry = step * sin (device_dir);
}
// draws the vacuum
void
draw_device (){
  const int A = '@';

  int left = round (rx) - DEVICE_WIDTH/2;
  int right = left + DEVICE_WIDTH - 1;

  int top = round(ry) - DEVICE_HEIGHT/2;
  int bottom = top + DEVICE_HEIGHT - 1;

  draw_line(left, top + 3, left, bottom - 3, A);
  draw_line (left + 1, top + 2, left + 1, bottom - 2, A);
  draw_line (left + 2, top + 1, left + 2, bottom - 1, A);
  draw_line (left + 3, top, left + 3, bottom, A);
  draw_line (left + 4, top, left + 4, bottom, A); 
  draw_line (left + 5, top, left + 5, bottom, A);
  draw_line (left + 6, top, left + 6, bottom, A);  
  draw_line (left + 7, top + 1, left + 7, bottom - 1, A);
  draw_line (right - 1, top + 2, right - 1, bottom - 2, A);
  draw_line (right, top + 3, right, bottom - 3, A);
}
//updates the vacuums position
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
  if (ch == 'j' && rx - DEVICE_WIDTH / 2 > 2)
    {
      rx--;
    }
  else if (ch == 'l' && rx + DEVICE_WIDTH / 2 < screen_width () - 2)
    {
      rx++;
    }
  else if (ch == 'k' && ry + DEVICE_HEIGHT / 2 < screen_height () - 5)
    {
      ry++;
    }
  else if (ch == 'i' && ry - DEVICE_HEIGHT / 2 > 6)
    {
      ry--;
    }
}

//////////////RUBBISH//////////////////////
void draw_trash(int i) {
	int left = round(tx[i]) - TRASH_WIDTH / 2;
	int right = left + TRASH_WIDTH - 1;
	int bottom = round(ty[i]) - TRASH_HEIGHT / 2 + TRASH_HEIGHT - 1;

	draw_line(left, bottom, right, bottom, '&');
	draw_line(left + 1, bottom - 1, right - 1, bottom - 1,'&');
	draw_line(left + 2, bottom - 2, right - 2, bottom - 2, '&');
	draw_line(left + 3, bottom - 3, right - 3, bottom - 3, '&');
	draw_line(left + 4, bottom - 4, right - 4, bottom - 4, '&');
	draw_line(left + 5, bottom - 5, left + 5, bottom - 5, '&');
}

void draw_trashs() {
	for (int i = 0; i < num_trash; i++) {
		draw_trash(i);
	}
}


void setup_trash(int i, double x1, double y1) {
	tx[i] = x1;
	ty[i] = y1;
}

//asks for ammount of trash and draws it 
void setup_trashs() {
	int trash_max = get_int("How many trash piles would you like?");
	trash_max = trash_max < 1 || trash_max > MAX_TRASH ? 1 : trash_max;
	double x1, y1;
	int w, h;
	get_screen_size(&w, &h);
	
	while (num_trash < trash_max) {
		x1 = TRASH_WIDTH / 2 + rand() % (w - 1 - TRASH_WIDTH);
		y1 = TRASH_HEIGHT / 2 + rand() % (h - 1 - TRASH_HEIGHT);

		x1 = confine(x1, (1 + TRASH_WIDTH / 2), (w - 1 - TRASH_WIDTH / 2));
		y1 = confine(y1, (5 + TRASH_HEIGHT / 2), (h - 4 - TRASH_HEIGHT / 2));
		
		bool trash_collided = false;
		for (int c_trash = 0; c_trash < num_trash - 1; c_trash++) {
			if (collided(x1, y1, TRASH_WIDTH, TRASH_HEIGHT, tx[c_trash], ty[c_trash], TRASH_WIDTH, TRASH_HEIGHT)) {
				trash_collided = true;
				break;
			}

		}

		if (!trash_collided && !collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, x1, y1, TRASH_WIDTH, TRASH_HEIGHT) &&
			!collided(cx, cy, CS_WIDTH, CS_HEIGHT, x1, y1, TRASH_WIDTH, TRASH_HEIGHT)) {
			setup_trash(num_trash, x1, y1);
			num_trash++;
		}
	}
}

// makes sure trash doesn't collide with border vacuum or other trash
void check_collision_trash() {
	for (int i = 0; i < num_trash; i++) {
		if (collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, tx[i], ty[i], TRASH_WIDTH, TRASH_HEIGHT)) {
			num_trash--;
			ty[i] = ty[num_trash];
			tx[i] = tx[num_trash];
			load = load + 10;
			return;
		}
	}
}

////SLIME/////

void draw_slime(int i) {
	int left = round(sx[i]) - SLIME_WIDTH / 2;
	int right = left + SLIME_WIDTH - 1;

	int top = round(sy[i]) - SLIME_HEIGHT / 2;
	int bottom = top + SLIME_HEIGHT - 1;

	draw_line(left, top + 2, left, bottom - 2, '~');
	draw_line(left + 1, top + 1, left + 1, bottom - 1, '~');
	draw_line(left + 2, top, left + 2, bottom, '~');
	draw_line(left + 3, top + 1, left + 3, bottom - 1, '~');
	draw_line(right, top + 2, right, bottom - 2, '~');
}

void draw_slimes() {
	for (int i = 0; i < num_slime; i++) {
		draw_slime(i);
	}
}


void setup_slime(int i, double x, double y) {
	sx[i] = x;
	sy[i] = y;
}
// asks for number of slimes then prints them 
void setup_slimes() {
	int slime_max = get_int("How many slime piles would you like?");
	slime_max = slime_max < 1 || slime_max > MAX_SLIME ? 1 : slime_max;
	double x, y;

	int w, h;
	get_screen_size(&w, &h);

	while (num_slime < slime_max) {
		//		sx[i] = 1 + SLIME_WIDTH / 2 + rand() % (w - 1 - SLIME_WIDTH);
		x = SLIME_WIDTH / 2 + rand() % (w - 1 - SLIME_WIDTH);
		y = SLIME_HEIGHT / 2 + rand() % (h - 1 - SLIME_HEIGHT);

		x = confine(x, (1 + SLIME_WIDTH / 2), (w - 1 - SLIME_WIDTH / 2));
		y = confine(y, (5 + SLIME_HEIGHT / 2), (h - 4 - SLIME_HEIGHT / 2));

		bool slime_collided = false;

		for (int c_slime = 0; c_slime < num_slime - 1; c_slime++) {
			if ((collided(x, y, SLIME_WIDTH, SLIME_HEIGHT, sx[c_slime], sy[c_slime], SLIME_WIDTH, SLIME_HEIGHT)) ||
				(collided(x, y, SLIME_WIDTH, SLIME_HEIGHT, tx[c_slime], ty[c_slime], TRASH_WIDTH, TRASH_HEIGHT))) {
				slime_collided = true;
				break;
			}
		}
		if (!slime_collided && !collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, x, y, SLIME_WIDTH, SLIME_HEIGHT) &&
			!collided(cx, cy, CS_WIDTH, CS_HEIGHT, x, y, SLIME_WIDTH, SLIME_HEIGHT)) {
			setup_slime(num_slime, x, y);
			num_slime++;
		}
	}

}
//checks slimes don't collide 
void check_collision_slime() {
	for (int i = 0; i < num_slime; i++) {
		if (collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, sx[i], sy[i], SLIME_WIDTH, SLIME_HEIGHT) && !charging && eating) {
			num_slime--;
			sy[i] = sy[num_slime];
			sx[i] = sx[num_slime];
			load = load + 5;
		}
	}
}

//DUST/////
void draw_dust(int i) {
	int left = round(dx[i]);
	int top = round(dy[i]);

	draw_char(left, top, '.');
}

void draw_dusts() {
	for (int i = 0; i < num_dust; i++) {
		draw_dust(i);
	}
}


void setup_dust(int i, double x2, double y2) {
	dx[i] = x2;
	dy[i] = y2;
}
//again asks how many dust you want 
void setup_dusts() {
	int dust_max = get_int("     How mush dust would you like?     ");
	dust_max = dust_max < 1 || dust_max > MAX_DUST ? 1 : dust_max;
	double x2, y2;

	int w, h;
	get_screen_size(&w, &h);

	while (num_dust < dust_max) {
		//		sx[i] = 1 + SLIME_WIDTH / 2 + rand() % (w - 1 - SLIME_WIDTH);
		x2 = DUST_WIDTH / 2 + rand() % (w - 1 - DUST_WIDTH);
		y2 = DUST_HEIGHT / 2 + rand() % (h - 1 - DUST_HEIGHT);

		x2 = confine(x2, (1 + DUST_WIDTH / 2), (w - 1 - DUST_WIDTH / 2));
		y2 = confine(y2, (5 + DUST_HEIGHT / 2), (h - 4 - DUST_HEIGHT / 2));

		bool dust_collided = false;
		
		for (int c_dust = 0; c_dust < num_dust - 1; c_dust++) {
			if ((collided(x2, y2, DUST_WIDTH, DUST_HEIGHT, dx[c_dust], dy[c_dust], DUST_WIDTH, DUST_HEIGHT)) ||
				(collided(x2, y2, DUST_WIDTH, DUST_HEIGHT, sx[c_dust], sy[c_dust], SLIME_WIDTH, SLIME_HEIGHT))||
				(collided(x2, y2, DUST_WIDTH, DUST_HEIGHT, tx[c_dust], ty[c_dust], TRASH_WIDTH, TRASH_HEIGHT))) {
				dust_collided = true;
				break;
			}
		}
		if (!dust_collided && !collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, x2, y2, DUST_WIDTH, DUST_HEIGHT) &&
			!collided(cx, cy, CS_WIDTH, CS_HEIGHT, x2, y2, DUST_WIDTH, DUST_HEIGHT)) {
			setup_dust(num_dust, x2, y2);
			num_dust++;
		}

	}

}

//checks for dust colliding 

void check_collision_dust() {
	for (int i = 0; i < num_dust; i++) {
		if (collided(rx, ry, DEVICE_WIDTH, DEVICE_HEIGHT, dx[i], dy[i], DUST_WIDTH, DUST_HEIGHT) && !charging && eating) {
			num_dust--;
			dy[i] = dy[num_dust];
			dx[i] = dx[num_dust];
			load = load + 1;
		}
	}
}




//////////DRAW_ALL///////////////
// draws everything onto the screen 
void
draw_all()
{
	int w, h;
	get_screen_size(&w, &h);
	clear_screen();
	draw_border();
	clock();
	draw_device();

	draw_chargingstation();
	if (charging) at_base();
	draw_battery();

	draw_trashs(); 
	draw_slimes();
	draw_dusts(); 
	draw_string(round(w / 3) + 3, 3, "g  weight "); // prints weight currently in vacuum 
	draw_int(round(w / 3) + 1, 3, load);
	draw_int(round(w / 3) + 1, 1, new_dry); // prints vacuums direction
	show_screen();

}
// is supposed to return vacuum to base when load hits 45g
void load_carrying_setup() {
	int w, h;
	get_screen_size(&w, &h);

	if (load > 45) {
		return_to_base();
	
		
	}
	
}


////SETUP/////////////

void setup(void)
{
	start_time = get_current_time();
	setup_device();
	battery_life = 100;
	setup_chargingstation();  
	setup_trashs();
	setup_slimes();
	setup_dusts();
	draw_all();
}

void reset() {
	setup();
}

///////////////FUNCTIONS FOR CONTROLS//////////////////

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

// changes the delay in milliseconds 
void do_delay_cheat() {
  delay = get_int("New delay (milliseconds)?"); 
  int delay = 100; 

    if (delay < 0) delay = 10;
}

// sets a timeout counter if the user wishes 
void do_timeout() {
	timeout = get_int("How many seconds shall the program run?"); 

}

bool timed_out() {
    return get_current_time() - start_time >= timeout;
}
//places new slimes down at random 
void do_slime_cheat() {
	int slime_id = get_int("Slime Id?"); 
	int x = get_int("x?");
	int y = get_int("y?");

	if (slime_id >= 0 && slime_id < num_slime) {
		sx[slime_id] = x;
		sy[slime_id] = y;
	}
}

//places new dust down at random 
void do_dust_cheat() {
	int dust_id = get_int("Dust Id?"); 
	int x = get_int("x?");
	int y = get_int("y?");

	if (dust_id >= 0 && dust_id < num_dust) {
		dx[dust_id] = x;
		dy[dust_id] = y;
	}
}

//places new trash down at random 
void do_trash_cheat() {
	int trash_id = get_int("Trash Id?"); 
	int x = get_int("x?");
	int y = get_int("y?");

	if (trash_id >= 0 && trash_id < num_trash) {
		tx[trash_id] = x;
		ty[trash_id] = y;
	}
}
// gets a new weight for the vacuum
void do_load_cheat() {
	load = get_int("What ~positive~ weight is the vacuum holding?"); 
}
// gets a new battery % for the vacuum
void do_battery_cheat() {
	battery_life = get_int("Chose your battery's charge level: "); 
}

// prints the help screen 
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
        "o: set a time out for a specified number of milliseconds chosen by you \n"
        "v: vacuum cheat - lets you override the position and direction of the vacuum\n"
        "p: pause or resume zombie movement\n"
		"w: lets you choose any positive value as the vacuums new weight in grams \n"
        "m: change the millisecond loop delay\n"
        "q: quit\n"
		"y: lets you choose how much battery in % you would like right now \n"
        "?: display this help screen & also exits it \n\n\n"
        "Press a key to continue playing the game :) \n"
	 ); 
	 show_screen(); 
    
    while (get_char() > 0) {}
    while (!timed_out() && wait_char() <= 0) {}
}
////////////LOOP FOR MAIN AND CONTROLS////////////////////
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
			draw_battery();
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

	if (paused) {} //do nothing 
	else {
		if (!charging) {
			update_device();
			move_device(ch);
			update_battery_life();
			draw_battery();
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