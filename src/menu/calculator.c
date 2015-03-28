/* Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <math.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <avr-pt.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>

#include <debug.h>

#include <avrdata.h>

#include <linalg/vector.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"
#include "eval.h"

static char linebuffer[30];
static int stackpos;
static float stack[10]; /* keep a stack of 6 numbers */

static enum {NONE, STACKEMPTY, BUFFERFULL, BADEXP} error;

#define OPERATE(X) enter_line(); {float x = pop(); push(pop() X x);} PT_EXIT(&pt);
#define APPLY1(X) enter_line(); push(X(pop())); PT_EXIT(&pt);
#define APPLY2(X) enter_line(); push(X(pop(), pop())); PT_EXIT(&pt);

static uint8_t haddecimal, hade;

static uint8_t mode, size = 2;

#define ADDFUNC(name) { static const char s[] PROGMEM = #name; add_func(s); } \
                      PT_EXIT(&pt);

static float inv(float x) { return 1/x; }

/* evaluate an entire expression */
static float eval_line(char *buffer)
{
   struct atom queue[16];
   int queuepos;
   if(shunting_yard(buffer, queue, &queuepos) == -1) {
      error = BADEXP;
      return NAN;
   }
   return eval(queue, queuepos);
}

static void push(float x)
{
  if(stackpos == (sizeof stack) / (sizeof *stack)) {
    memmove(stack, stack+1, sizeof stack - sizeof *stack);
    stackpos--;
  }
  stack[stackpos++] = x;
}

static float pop(void)
{
  if(stackpos == 0)
    error = STACKEMPTY;
  else
    stackpos--;
  return stack[stackpos];
}

/* perform backspace, but pop each line off the stack */
static void backspace(void)
{
  if(linebuffer[0]) {
    int i=0;
    while(linebuffer[i]) i++;
    if(linebuffer[i-1] == '.')
      haddecimal = 0;
    else if(linebuffer[i-1] == 'e')
      hade = 0;
    linebuffer[i-1] = '\0';
  } else if(stackpos) {
    float x = pop();
    if(isnan(x) || isinf(x))
      linebuffer[0] = '\0';
    else {
      sprintf(linebuffer, "%g", x);
      haddecimal = !!strchr(linebuffer, '.');
      hade = !!strchr(linebuffer, 'e');
    }
  } 
}

static void add_char(char c)
{
  int i=0;
  while(linebuffer[i]) i++;
  if(i == sizeof linebuffer)
    error = BUFFERFULL;
  else {
    linebuffer[i] = c;
    linebuffer[i+1] = '\0';
  }
}

static void add_func(const char *c)
{
  char d;
  while((d = pgm_read_byte(c++)))
    add_char(d);
}

static void add_decimal(void)
{
  if(haddecimal)
    return;
  haddecimal = 1;
  add_char('.');
}

static void add_e(void)
{
  if(hade)
    return;
  hade = 1;
  add_char('e');
}

static void add_constant(float val)
{
  char b[12];
  int len = snprintf(b, sizeof b, "%g", val), i;
  for(i=0; i<len; i++)
    add_char(b[i]);
}

static void negate_line(void)
{
  if(linebuffer[0]=='-')
    memmove(linebuffer, linebuffer+1, strlen(linebuffer));
  else if(strlen(linebuffer) < sizeof linebuffer - 1) {
    memmove(linebuffer+1, linebuffer, strlen(linebuffer) + 1);
    linebuffer[0] = '-';
  }
}

static void enter_line(void)
{
  if(linebuffer[0]) {
    push(eval_line(linebuffer));
    haddecimal = 0;
    hade = 0;
    linebuffer[0] = '\0';
  }
}

static const struct gfx_rect selectionarea PROGMEM = {0, LCD_WIDTH-1, 12, LCD_HEIGHT-1};

static int
operators_thread(int calc)
{
 static struct pt pt;
  PT_BEGIN(&pt);

restart:
   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Calculator Operators");

   /* draw selection */
   static const char items1[] PROGMEM = "1  + \n2  - \n3  * \n4  / \n5  % \n"
     "6  ^ \n7  ! \n8  inv \n9 neg\n0 decimal\nA e\nB Pi\nC E\nD Cancel";

   static const char items2[] PROGMEM = "1  + \n2  - \n3  * \n4  / \n5  % \n"
     "6  ^ \n7  ! \n8  ( \n9 ) \n0 decimal\nA e\nB Pi\nC E\nD Cancel";

   gfx_setfont(GFX_R08_FONT);
   draw_selection(&selectionarea, mode ? items1 : items2);
   for(;;) {
     if(mode && calc)
      switch(key_lastup()) {
      case KEY_1: OPERATE(+)
      case KEY_2: if(linebuffer[0]) {
	            OPERATE(-)
		  } else {
                    add_char('-');
		    PT_EXIT(&pt);
                  }
      case KEY_3: OPERATE(*)
      case KEY_4: OPERATE(/)
      case KEY_5: APPLY2(fmod)
      case KEY_6: APPLY2(pow)
      case KEY_7: APPLY1(factorial)
      case KEY_8: APPLY1(inv)
      case KEY_9: negate_line(); PT_EXIT(&pt);
      case KEY_0: add_decimal(); PT_EXIT(&pt);
      case KEY_A: add_e(); PT_EXIT(&pt);
      case KEY_B: add_constant(M_PI); PT_EXIT(&pt);
      case KEY_C: add_constant(exp(1)); PT_EXIT(&pt);
      case KEY_D: PT_EXIT(&pt);
      }
     else
      switch(key_lastup()) {
      case KEY_1: add_char('+'); PT_EXIT(&pt);
      case KEY_2: add_char('-'); PT_EXIT(&pt);
      case KEY_3: add_char('*'); PT_EXIT(&pt);
      case KEY_4: add_char('/'); PT_EXIT(&pt);
      case KEY_5: add_char('%'); PT_EXIT(&pt);
      case KEY_6: add_char('^'); PT_EXIT(&pt);
      case KEY_7: add_char('!'); PT_EXIT(&pt);
      case KEY_8: add_char('('); PT_EXIT(&pt);
      case KEY_9: add_char(')'); PT_EXIT(&pt);
      case KEY_0: add_char('.'); PT_EXIT(&pt);
      case KEY_A: add_char('e'); PT_EXIT(&pt);
      case KEY_B: add_constant(M_PI); PT_EXIT(&pt);
      case KEY_C: add_constant(exp(1)); PT_EXIT(&pt);
      case KEY_D: PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

static int
functions_thread(int calc)
{
 static struct pt pt;
  PT_BEGIN(&pt);

restart:
   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Calculator Functions");

   /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION_A(&selectionarea, "1 sqrt\n2 sin\n3 cos\n4 tan\n5 asin\n"
		    "6 acos\n7 atan\n8 exp\n9 log\n0 log10\n# dup\n* abs\n\n\n"
		    "A sinh\nB cosh\nC tanh\nD Cancel");

   for(;;) {
     if(mode && calc)
      switch(key_lastup()) {
      case KEY_1: APPLY1(sqrt) case KEY_2: APPLY1(sin)  case KEY_3: APPLY1(cos)
      case KEY_4: APPLY1(tan)  case KEY_5: APPLY1(asin) case KEY_6: APPLY1(acos)
      case KEY_7: APPLY1(atan) case KEY_8: APPLY1(exp)  case KEY_9: APPLY1(log)
      case KEY_0: APPLY1(log10)
      case KEY_STAR: APPLY1(fabs)
      case KEY_POUND:
      {
	 float x = pop();
	 push(x);
	 push(x);
      } PT_EXIT(&pt);
      case KEY_A: APPLY1(sinh) case KEY_B: APPLY1(cosh) case KEY_C: APPLY1(tanh)
      case KEY_D: PT_EXIT(&pt);
      }
     else
      switch(key_lastup()) {
      case KEY_1: ADDFUNC(sqrt) case KEY_2: ADDFUNC(sin)
      case KEY_3: ADDFUNC(cos)  case KEY_4: ADDFUNC(tan)
      case KEY_5: ADDFUNC(asin) case KEY_6: ADDFUNC(acos)
      case KEY_7: ADDFUNC(atan) case KEY_8: ADDFUNC(exp)
      case KEY_9: ADDFUNC(log)  case KEY_0: ADDFUNC(log10)
      case KEY_STAR: ADDFUNC(abs)
      case KEY_POUND:
	 if(calc) {
	    enter_line();
	    float x = pop();
	    push(x);
	    push(x);
	 } PT_EXIT(&pt);
      case KEY_A: ADDFUNC(sinh)
      case KEY_B: ADDFUNC(cosh)
      case KEY_C: ADDFUNC(tanh)
      case KEY_D: PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

static int
sensor_selection_thread(uint8_t *cancel, const char *name)
{
 static struct pt pt;
  PT_BEGIN(&pt);

restart:
   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Sensor Selection");

   gfx_setpos(LCD_WIDTH/2, 12);
   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);
   gfx_puts_P(name);

   /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION_A(&selectionarea,
		    "1 calibrated x\n2 calibrated y\n3 calibrated z\n"
		    "4 raw x\n5 raw y\n6 raw z\nD cancel");

   for(;;) {
      uint16_t key = key_lastup();
      switch(key) {
      case KEY_1:
      case KEY_2:
      case KEY_3:
	 add_char(pgm_read_byte(name));
	 add_char(key_todigit(key)-1+'x');
	 *cancel = 0;
	 PT_EXIT(&pt);
      case KEY_4:
      case KEY_5:
      case KEY_6:
	 add_char(pgm_read_byte(name));
	 add_char('r');
	 add_char(key_todigit(key)-4+'x');
	 *cancel = 0;
	 PT_EXIT(&pt);
      case KEY_D:
	 *cancel = 1;
	 PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

static int
sensors_thread(void)
{
 static struct pt pt;
  PT_BEGIN(&pt);

restart:
   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Sensor inputs");

   /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION_A(&selectionarea,
		    "1 accel\n2 mag\n"
		    "3 pitch\n4 roll\n5 yaw\n6 dip\n"
		    "7 temp\n8 time\n"
		    "C done\nD cancel");

   for(;;) {
      uint8_t cancel;
      switch(key_lastup()) {
      case KEY_1:
	 PT_WAIT_THREAD(&pt, sensor_selection_thread(&cancel, PSTR("accel")));
	 if(cancel)
	    goto restart;
	 PT_EXIT(&pt);
      case KEY_2:
	 PT_WAIT_THREAD(&pt, sensor_selection_thread(&cancel, PSTR("mag")));
	 if(cancel)
	    goto restart;
	 PT_EXIT(&pt);
      case KEY_3: ADDFUNC(pitch);
      case KEY_4: ADDFUNC(roll);
      case KEY_5: ADDFUNC(yaw);
      case KEY_6: ADDFUNC(dip);
      case KEY_7: ADDFUNC(temp);
      case KEY_8: ADDFUNC(time);
      case KEY_B: ADDFUNC(cosh)
      case KEY_C: ADDFUNC(tanh)
      case KEY_D: PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

static int
setup_thread(void)
{
 static struct pt pt;
  PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 12, "Calculator Setup");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);

   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 mode\n2 size\nA about\nD done");

   gfx_setpos(40, 12);
   if(mode)
     GFX_PUTS("Reverse Polish\n");
   else
     GFX_PUTS("Standard\n");

   gfx_setfont(gfx_fonts+size);
   gfx_setpos(50, 29);
   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_MIDDLE);
   GFX_PUTS("123");

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
	 mode = !mode;
	 goto restart;
      case KEY_2:
	 if(++size == 4)
	    size = 0;
	 goto restart;
      case KEY_A:
	 PT_WAIT_THREAD(&pt, reader_thread(about_calculator));
	 goto restart;
      case KEY_D:
	 PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

int
graph_entry_custom_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, -2, 11, "Custom Graph Entry");

   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);

   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 12, 23, 1);

   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_B08_FONT);
   gfx_setpos(LCD_WIDTH/2, 11);
   gfx_puts(linebuffer);

   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 23, LCD_HEIGHT-1,
		  "0-9 number\n* backspace\n# .\n\n"
		  "A operators\nB functions\nC sensors\nD done");

   for(;;) {
	 uint16_t key = key_lastup();
	 if(key >= KEY_1 && key <= KEY_0) {
	    add_char('0' + key_todigit(key));
	    goto restart;
	 }
	 switch(key) {
	 case KEY_STAR:
	 {
	    int len = strlen(linebuffer);
	    if(len)
	       linebuffer[len-1] = '\0';
	 } goto restart;
	 case KEY_POUND:
	    add_decimal(); goto restart;
	 case KEY_A:
	    PT_WAIT_THREAD(&pt, operators_thread(0)); goto restart;
	 case KEY_B:
	    PT_WAIT_THREAD(&pt, functions_thread(0)); goto restart;
	 case KEY_C:
	    PT_WAIT_THREAD(&pt, sensors_thread()); goto restart;
	 case KEY_D:
	    PT_EXIT(&pt);
	 }
      
      MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

char *calculator_linebuffer(void)
{
   return linebuffer;
}

int
calculator_thread(void)
{
  static struct pt pt;

  PT_BEGIN(&pt);
  linebuffer[0] = '\0';
restart:
  switch(error) {
  case NONE: break;
  case STACKEMPTY:
    PT_WAIT_THREAD(&pt, reader_thread(warning_calculator_stack_empty));
    stackpos = 0;
    break;
  case BUFFERFULL:
    PT_WAIT_THREAD(&pt, reader_thread(warning_calculator_buffer_full));
    break;
  case BADEXP:
    PT_WAIT_THREAD(&pt, reader_thread(warning_calculator_bad_exp));
    pop();
    break;
  }

  error = NONE;

   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);

   DRAW_SELECTION(0, LCD_WIDTH/2, 2, LCD_HEIGHT-1,
      "* operators\n# functions\nA setup\nB <==\nC enter\nD menu");

   gfx_line(53, 0, 53, LCD_HEIGHT-1);

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 55, LCD_WIDTH-21, 0, 8, "Calculator");

   /* draw stack */
   gfx_setpos(LCD_WIDTH-1, 10);
   gfx_setalignment(GFX_ALIGN_RIGHT | GFX_ALIGN_TOP);
   gfx_setfont(gfx_fonts+size);

   int n;
   switch(size) {
   case 0: n = 7; break;
   case 1: n = 6; break;
   case 2: n = 4; break;
   case 3: n = 3; gfx_setpos(LCD_WIDTH-1, 8); break;
   }
     
   int i;
   for(i = stackpos-n; i<stackpos; i++) {
     if(i < 0)
       GFX_PRINTF("\n");
     else
       GFX_PRINTF("%g\n", stack[i]);

     int y;
     gfx_getpos(0, &y);
     if(y > 55)
       break;
   }

   /* draw linebuffer */
   gfx_line(53, 54, LCD_WIDTH-1, 54);
   gfx_setpos(LCD_WIDTH-1, 55);

   gfx_setfont(GFX_5x8_FONT);
   gfx_puts(linebuffer);

   for(;;) {
      uint16_t key = key_lastup();
      if(key >= KEY_1 && key <= KEY_0) {
	 add_char('0' + key_todigit(key));
	 goto restart;
      }

      switch(key) {
      case KEY_STAR: PT_WAIT_THREAD(&pt, operators_thread(1)); goto restart;
      case KEY_POUND: PT_WAIT_THREAD(&pt, functions_thread(1)); goto restart;
      case KEY_A: PT_WAIT_THREAD(&pt, setup_thread()); goto restart;
      case KEY_B: backspace(); goto restart;
      case KEY_C: enter_line(); goto restart;
      case KEY_D: PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
