/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <fcntl.h>
#include <unistd.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <string.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include <calibration/calibrationserver.h>

#include <linalg/vector.h>
#include <linalg/quaternion.h>
#include <linalg/rotate.h>

#include <gfx/lcd.h>

#include <menu/menu.h>
#include <menu/keys.h>

#include <avrdrivers/avrdrivers.h>
SET_SOFTWARE_NAME("menuhost");

#include "emulation.h"

float monitor_ratio;

float adcdiv = 1;

int freeze;

static void display(void)
{
   if(freeze)
      return;

   extern int menu_poll(void);
   menu_poll();

   /* convert colorspace */
   char fb24[LCD_WIDTH*LCD_HEIGHT*3];
   int x, y;
   for(x = 0; x<LCD_WIDTH; x++)
      for(y = 0; y<LCD_HEIGHT; y++) {
          int fbval = gfx_getpixel(x, y);
          if(reversevideo)
              fbval = !fbval;
          int i;
          for(i=0; i<3; i++)
             fb24[(y*LCD_WIDTH+x)*3+i] = fbval ? contrast : backcolor[i];
      }

   /* stick the framebuffer in a texture */
   glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, LCD_WIDTH, LCD_HEIGHT,
                0, GL_RGB, GL_UNSIGNED_BYTE, fb24);

   /* render it as a fat quad */
   glBegin(GL_QUADS);
   float left, right, top, bottom;

   if(monitor_ratio < (float)LCD_WIDTH/LCD_HEIGHT) {
      float bar = (LCD_WIDTH / monitor_ratio - LCD_HEIGHT) / 2
         / (LCD_WIDTH / monitor_ratio);
      left = 0, right = 1;
      top = bar, bottom = 1 - bar;
   } else {
      float bar = (LCD_HEIGHT * monitor_ratio - LCD_WIDTH) / 2
         / (LCD_HEIGHT * monitor_ratio);
      left = bar, right = 1 - bar;
      top = 0, bottom = 1;
   }

   glTexCoord2f(        0,          0); glVertex2d(left, bottom);
   glTexCoord2f(LCD_WIDTH,          0); glVertex2d(right, bottom);
   glTexCoord2f(LCD_WIDTH, LCD_HEIGHT); glVertex2d(right, top);
   glTexCoord2f(        0, LCD_HEIGHT); glVertex2d(left, top);
   glEnd();

   glutSwapBuffers();
}

/* new window size or exposure */
static void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
   gluOrtho2D(0, 1, 0, 1);

   monitor_ratio = w/LCD_RATIO/h;
}

static void key(unsigned char k, int down)
{
   switch (k) {
   case '1': key_set(KEY_1, down); break;
   case '2': key_set(KEY_2, down); break;
   case '3': key_set(KEY_3, down); break;
   case '4': key_set(KEY_4, down); break;
   case '5': key_set(KEY_5, down); break;
   case '6': key_set(KEY_6, down); break;
   case '7': key_set(KEY_7, down); break;
   case '8': key_set(KEY_8, down); break;
   case '9': key_set(KEY_9, down); break;
   case '0': key_set(KEY_0, down); break;
   case '*': key_set(KEY_STAR, down); break;
   case '#': key_set(KEY_POUND, down); break;
   case 'a': case 'A': key_set(KEY_A, down); break;
   case 'b': case 'B': key_set(KEY_B, down); break;
   case 'c': case 'C': key_set(KEY_C, down); break;
   case 'd': case 'D': key_set(KEY_D, down); break;
   case 'f': case 'F': if(down) freeze = !freeze; break;
   case 27: eeprom_save(); exit(0); break;
   }
}

static void keydown(unsigned char k, int x, int y)
{
   x = y = 0;
   key(k, 1);
}

static void keyup(unsigned char k, int x, int y)
{
   x = y = 0;
   key(k, 0);
}

static FILE *dev;

static void idle(void)
{
   calibration_server_poll();
}

static void ratefunc(int msecs)
{
   glutPostRedisplay();
   glutTimerFunc(msecs, ratefunc, msecs);
}

int main( int argc, char *argv[] )
{
   surveymeasurements_clear();
   eeprom_load();

   usemenu = 1;

   glutInit(&argc, argv);
   glutInitWindowSize(LCD_WIDTH*LCD_RATIO*5, LCD_HEIGHT*5);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DOUBLE);
   glutCreateWindow("menuhost");

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);

   glutIgnoreKeyRepeat(1);
   glutKeyboardFunc(keydown);
   glutKeyboardUpFunc(keyup);

   /* setup the calibration server */
   calibration_server_setup(argc, argv);

   /* turn green backlight off by default in emulation */
   extern uint8_t greenbacklight;
   greenbacklight = 0;

   /* apply settings */
   settings_apply();

   glutIdleFunc(idle);

   glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glEnable(GL_TEXTURE_RECTANGLE_ARB);

   glutTimerFunc(0, ratefunc, 1000/12);
   glClearColor (0.4, 0.0, 0.4, 0.0);

   glutMainLoop();
   return 0;
}
