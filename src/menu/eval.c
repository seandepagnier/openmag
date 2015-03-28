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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <avrdata.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>
#include <calibration/temperature.h>

#include <data/data.h>
#include <debug.h>

#include "eval.h"
#include "internal.h"

static int find_func(char **name)
{
   static const char funcs[][6] PROGMEM = {"sqrt",
					   "sinh", "cosh", "tanh",
					   "sin", "cos", "tan",
					   "asin", "acos", "atan",
					   "exp", "log10", "log", "abs"};
  int i;
  for(i=0; i<(sizeof funcs) / (sizeof *funcs); i++) {
    int len = strlen_P(funcs[i]);
    if(!strncmp_P(*name, funcs[i], len)) {
      *name += len;
      return i;
    }
  }

  return -1;
}

static int find_value(char **name)
{
  static const char values[][6] PROGMEM =
     {"ax", "ay", "az", "arx", "ary", "arz",
      "mx", "my", "mz", "mrx", "mry", "mrz",
      "pitch", "roll", "yaw", "dip", "temp", "time", "ane"};

  int i;
  for(i=0; i<(sizeof values) / (sizeof *values); i++) {
    int len = strlen_P(values[i]);
    if(!strncmp_P(*name, values[i], len)) {
      *name += len;
      return i;
    }
  }

  return -1;
}

static float apply_func(int func, float x)
{
  switch(func) {
  case 0: return sqrt(x);
  case 1: return sinh(x); case 2: return cosh(x); case 3: return tanh(x); 
  case 4:  return sin(x);  case 5:  return cos(x); case 6: return tan(x);
  case 7:  return asin(x); case 8:  return acos(x); case 9:  return atan(x);
  case 10: return exp(x);  case 11: return log10(x); case 12:  return log(x);
  case 13: return fabs(x);
  }
  return NAN;
}

static float apply_operator(char op, float x, float y)
{
  switch(op) {
  case '+': return x+y;  case '-': return x-y;
  case '*': return x*y;  case '/': return x/y;
  case '^': return pow(x, y);  case '%': return fmod(x, y);
  }
  return NAN;
}

static float apply_value(enum value value)
{
   float p[3];
   switch(value) {
#define EACH_SENSOR(NAME) \
   case NAME##X: case NAME##Y: case NAME##Z: \
      sensor_get_calibrated(&NAME, p); \
      return p[value-NAME##X]; \
   case NAME##RAWX: case NAME##RAWY: case NAME##RAWZ: \
      return NAME.raw[value-NAME##RAWX];
   EACH_SENSOR(accel)
   EACH_SENSOR(mag)
   case PITCH:
#ifdef __AVR__
      calc_box_pitch_roll();
#else
      {
	 static float t;
	 pitch = 180/M_PI*sin(gettime());
      }
#endif
      return pitch;
   case ROLL: calc_box_pitch_roll(); return roll;
   case YAW: calc_box_yaw_dip(); return yaw;
   case DIP: calc_box_yaw_dip(); return dip;
   case TEMP: return temp_get(0);
   case TIME: return gettime();
   case ANEMOMETER: return calibrated_anemometer();
   }
   return NAN;
}

static int oporder(char op)
{
  switch(op) {
  case '(': case ')': return 0;
  case '+': case '-': return 1;
  case '*': case '/': case '%': return 2;
  case '^': case '!': return 3;
  default: return 4;
  }
}

/* perform shunting_yard algorithm to convert the line to postfix */
int shunting_yard(char *buffer, struct atom opqueue[16], int *pos)
{
   struct atom opstack[16];

  int opstackpos = 0, opqueuepos = 0;

  char *p = buffer;
  while(*p) {
    if(p>buffer && /* implied multiplication */
       (((p[0] == '(' || isalpha(p[0])) && isdigit(p[-1]) && !(oporder(p[-1])&3)) ||
	(p[-1] == ')' && !isalpha(p[0]) && p[0] != '!' && !(oporder(p[0])&3)))
       && (p > buffer+5 || strncmp_P(PSTR("log10"), p-5, 5))) {
      *--p = '*';
      continue;
    }

    /* read in as numeric */
    if((*p >= '0' && *p <= '9') || *p == '.'
       || (p[0] == '-' && /* implied subtraction */
	   (p == buffer || p[-1] == '(' || isalpha(p[-1])))) {
      opqueue[opqueuepos].ops = CONSTANT;
      opqueue[opqueuepos++].val = strtod(p, &p);
      continue;
    }

    /* function or value token, push on stack */
    if(*p >= 'a' && *p <= 'z') {
      int q = 0;
      if((opstack[opstackpos].func = find_func(&p)) >= 0)
	 opstack[opstackpos].ops = FUNCTION;
      else if((opstack[opstackpos].value = find_value(&p)) >= 0)
	 opstack[opstackpos].ops = VALUE;
      else
	 return -1;
  
      opstackpos++;
      continue;
    }

    if(*p != '(')
      while(opstackpos && oporder(opstack[opstackpos-1].op) >= oporder(*p)) {
	if(opstack[opstackpos-1].op == '(' && *p == ')') {
	  opstackpos--;
	  if(opstack[opstackpos-1].ops == 1)
	    opqueue[opqueuepos++] = opstack[--opstackpos];
	  goto parenok;
	}
	opqueue[opqueuepos++] = opstack[--opstackpos];
      }

    if(*p == ')')
       return -1;

    opstack[opstackpos].op = *p;
    opstack[opstackpos++].ops = OPERATOR;
  parenok:

    p++;
  }

  /* push stack onto queue */
  while(opstackpos)
    opqueue[opqueuepos++] = opstack[--opstackpos];

  *pos = opqueuepos;
  return 0;
}

/* evaluate a queue of values and return the result */
float eval(struct atom queue[16], int queuepos)
{
   struct atom stack[16];
   int stackpos = 0;

  /* now evaluate from RPN */
  int i;
  for(i=0; i < queuepos; i++)
    switch(queue[i].ops) {
    case CONSTANT: stack[stackpos++].val = queue[i].val; break;
    case FUNCTION: stack[stackpos-1].val
	= apply_func(queue[i].func, stack[stackpos-1].val); break;
    case OPERATOR:
      if(queue[i].op == '!')
	stack[stackpos-1].val = factorial(stack[stackpos-1].val);
      else {
	stackpos--;
	stack[stackpos-1].val =
	  apply_operator(queue[i].op,
			 stack[stackpos-1].val,
			 stack[stackpos].val);
      } break;
    case VALUE:
       stack[stackpos++].val = apply_value(queue[i].value);
    }

  if(stackpos != 1)
     return NAN;

  return stack[0].val;
}

float factorial(float x)
{
  x++; /* below is approximation of gamma */
  return pow(x/exp(1) * sqrt(x*sinh(1/x) + 1/(810*pow(x, 6))), x) * sqrt(2*M_PI/x);
}
