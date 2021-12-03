#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h" 



void draw_border ()
{
   int w, h;
  get_screen_size (&w, &h);
  //Draw border - horizontal 

  int chh = '-';
  draw_line (0, 0, w - 1, 0, chh);
  draw_line (0, h - 3, w - 1, h - 3, chh);
  draw_line (0, h - 1, w - 1, h - 1, chh);
  
  draw_line (0, 2, w - 1, 2, chh);  //draws table row 1
  draw_line (0, 4, w - 1, 4, chh);  //draws table row 2

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
//print captions 
  draw_string (1, 1, " SID: n10256989"); //prints student id 
  draw_string (round (w * 2 / 3) + 1, 3, "dust,slime,trash         100, 3, 1"); // prints the dust slime and trash headers with generic numbers as placeholders 
  draw_string (w/2-25, 1, "degrees"); // prints the degrees header
  
  
  
}












