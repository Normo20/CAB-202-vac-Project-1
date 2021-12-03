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


// Global variables  

#define DEVICE_WIDTH 10
#define DEVICE_HEIGHT 9
#define DEVICE_SPEED 0.2

#define confine(x,y,z) (x<y?y:x>z?z:x)

//defining Charging Station dimensions
#define CS_WIDTH 10
#define CS_HEIGHT 5 
double cx, cy; 
double rx, ry, drx, dry;
//double rx, ry are robots coordinates 

 bool charging; 
  bool eating;
 int load ; 



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

bool collided (
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1);







//////////////RUBBISH//////////////////////

//draws the trash 0 0
//                 o
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
 // draws multiple trash(s)
void draw_trashs() {
	for (int i = 0; i < num_trash; i++) {
		draw_trash(i);
	}
}

// sets up the trash 
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

// makes sure trash doesn't collide with border, vacuum or other trash
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
//draws slime 
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
// draws multiple slime(s)
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




//places new slime(s) down where user desires 
void do_slime_cheat() {
	int slime_id = get_int("Slime Id?"); 
	int x = get_int("x?");
	int y = get_int("y?");

	if (slime_id >= 0 && slime_id < num_slime) {
		sx[slime_id] = x;
		sy[slime_id] = y;
	}
}

//places new dust(s) down where user desires 
void do_dust_cheat() {
	int dust_id = get_int("Dust Id?"); 
	int x = get_int("x?");
	int y = get_int("y?");

	if (dust_id >= 0 && dust_id < num_dust) {
		dx[dust_id] = x;
		dy[dust_id] = y;
	}
}

//places new trash(s) down where user desires 
void do_trash_cheat() {
	int trash_id = get_int("Trash Id?"); 
	int x = get_int("x?");
	int y = get_int("y?");

	if (trash_id >= 0 && trash_id < num_trash) {
		tx[trash_id] = x;
		ty[trash_id] = y;
	}
}