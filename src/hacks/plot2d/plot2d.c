/* Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
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

#include "GL/glut.h"

#include <errno.h>
#include <string.h>

/* goal is to graph implicit functions, allows realtime modification of
   coefficients */

int width, height;

#define SCALE 2

double minx = -1.266*SCALE, maxx = 1.266*SCALE;
double miny = -SCALE, maxy = SCALE;

double C[10], dC[10];
int K[10];

int cursors;
int axes = 1;

/* cursors */
double xl, xr, xa;

const double colors[10][3] = {{1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 1},
                              {0, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, .5, 0},
                              {.5, 1, 0}, {.5, .5, .5}};

#define FONT        GLUT_BITMAP_9_BY_15
static void drawputs(const char *str)
{
   while(*str)
      glutBitmapCharacter(FONT, *str++);      
}

void drawprintf(const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   char buf[256];
   vsnprintf(buf, sizeof buf, fmt, ap);
   drawputs(buf);
   va_end(ap);
}

void synccolor(void)
{
   double pos[4], vp[4];
   glGetDoublev(GL_VIEWPORT, vp);
   glGetDoublev(GL_CURRENT_RASTER_POSITION, pos);
   glRasterPos2d(pos[0]/vp[2], pos[1]/vp[3]);
}

static void func(double x, double y, unsigned char data[3])
{
   double diff = C[0]*x*x + C[1]*x*y + C[2]*y*y + C[3]*x + C[4]*y + C[5];
   double diff2 = diff*diff;
   double diff6 = diff2*diff2*diff2;

   double sat = (1 - diff2);
   if(sat < 0)
      sat = 0;
   if(sat > 1)
      sat = 1;

   if(diff > 0)
      data[0] = data[1] = data[2] = sat*255;
   else {
      data[0] = data[1] = sat*255;
      data[2] = (1 - sat)*255;
   }
}

static void drawstuff( void )
{
#if 0
   glPointSize(2);
   double a;
   double x1 = C[3], y1 = 0;
   double x2 = C[4]*sin(C[5]/50), y2 = C[4]*cos(C[5]/50);

   glLineWidth(2);
   glBegin(GL_LINES);

   for(a = 0; a < 2 * M_PI; a+= .2) {
      double x = cos(a), y = sin(a);
      
      double xs = x1*x + y1*y;
      double ys = x2*x + y2*y;

          xs = C[3]*cos(a);
          ys = C[4]*cos(a-C[5]/50);

      glColor3d(1, 0, 1);
      glVertex2d(0, 0);
      glVertex2d(xs, ys);

      glColor3d(0, 1, 0);
      glVertex2d(xs, ys);
      glVertex2d(x, y);
   }   
   glEnd();

   glLineWidth(3);
   glBegin(GL_LINES);
   glColor3d(1, 0, 0);
   glVertex2d(0, 0);
   glVertex2d(x1, y1);
   glVertex2d(0, 0);
   glVertex2d(x2, y2);

   glEnd();
#endif
}

static void display( void )
{
   glClear(GL_COLOR_BUFFER_BIT);

   int height2 = height - 30;
   unsigned char data[width*height2*4];
   int i, j, k;
   for(i = 0; i<width; i++)
      for(j = 0; j<height2; j++) {
         double x = minx + (double)i*(maxx - minx)/width;
         double y = miny + (double)j*(maxy - miny)/height2;
         func(x, y, data + (j * width + i) * 3);
      }

   glRasterPos2d(0, 30.0/height);
   glDrawPixels(width, height-30, GL_RGB, GL_UNSIGNED_BYTE, data);

   glPushMatrix();
   double yscale = (double)height2/height/(maxy-miny);
   glScaled(1/(maxx-minx), yscale, 1);
   glTranslated((maxx-minx)/2, (maxy-miny)/2+(double)(height-height2)/height/yscale, 0);

   drawstuff();

   glLineWidth(2);
   if(axes) {
      glColor3d(1, 1, 1);
      glBegin(GL_LINES);
      double v, step = (maxy-miny)/10, mins = step / 4;
      for(v = miny; v < maxy; v+=step) {
         glVertex2d(mins, v);
         glVertex2d(-mins, v);
      }

      for(v = 0; v < maxx; v+=step) {
         glVertex2d(v, mins);
         glVertex2d(v, -mins);
      }

      for(v = 0; v >= minx; v-=step) {
         glVertex2d(v, mins);
         glVertex2d(v, -mins);
      }

      glVertex2d(0, miny);
      glVertex2d(0, maxy);
      glVertex2d(minx, 0);
      glVertex2d(maxx, 0);

      glEnd();
   }

   if(cursors) {
      glRotated(xa, 0, 0, 1);

      glLineWidth(3);
      glColor3d(0, 1, 1);
      glBegin(GL_LINES);
      glVertex2d(xl, -10); 
      glVertex2d(xl, 10); 
      glVertex2d(xr, -10); 
      glVertex2d(xr, 10); 
      glEnd();
   }

   glPopMatrix();

   glRasterPos2d(0, 0);
   for(i = 0; i < (sizeof C) / (sizeof *C); i++) {
      glColor3dv(colors[i]);
      synccolor();
      drawprintf("%d:%.2f ", i, C[i]);
   }

   glColor3d(1, 0, 0);

   glRasterPos2d(0, 15.0/height);
   drawprintf("xl: %.2f xr: %.2f xa %.2f xd: %.2f",
              xl, xr, xa, xr - xl);

   glutSwapBuffers();
}

static void idle(void)
{
   const double c = .1;

   int i;
   for(i = 0; i < (sizeof C) / (sizeof *C); i++) {
      switch(K[i]) {
      case 0: dC[i] = 0; goto again;
      case 1: dC[i] += c; break;
      case 2: dC[i] -= c; break;
      }
      C[i] += dC[i];

      glutPostRedisplay();
   again:;
   }
}

static void key(unsigned char k, int down)
{
   if(glutGetModifiers() & GLUT_ACTIVE_SHIFT)
      down *= 2;

   switch(k) {
   case 'a': if(down) axes = !axes; break;
   case 'c': if(down) cursors = !cursors; break;
   case 27:
      exit(0);
   case '1': case '!': K[0] = down; break;
   case '2': case '@': K[1] = down; break;
   case '3': case '#': K[2] = down; break;
   case '4': case '$': K[3] = down; break;
   case '5': case '%': K[4] = down; break;
   case '6': case '^': K[5] = down; break;
   case '7': case '&': K[6] = down; break;
   case '8': case '*': K[7] = down; break;
   case '9': case '(': K[8] = down; break;
   case '0': case ')': K[9] = down; break;
   }

   glutPostRedisplay();
}

static void keydown(unsigned char k, int x, int y)
{
   key(k, 1);
}

static void keyup(unsigned char k, int x, int y)
{
   key(k, 0);
}

static void specialkey(int k, int x, int y)
{
   (void) x;
   (void) y;

   const double c = .05;
   const double d = 3;

   switch(k) {
   case GLUT_KEY_UP: xa += d; break;
   case GLUT_KEY_DOWN: xa -= d; break;
   case GLUT_KEY_LEFT: xl += c, xr -= c; break;
   case GLUT_KEY_RIGHT: xl -= c, xr += c; break;
   }

   glutPostRedisplay();
}

/* new window size or exposure */
static void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   width = w;
   height = h;

   glMatrixMode (GL_PROJECTION);
   //glLoadIdentity ();
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
   gluOrtho2D(0, 1, 0, 1);
}

int main( int argc, char *argv[] )
{
   glutInit(&argc, argv);
   glutInitWindowPosition(0, 0);
   glutInitWindowSize(600, 500);
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
   glutCreateWindow("Plot2d!");

   C[0] = C[1] = 1;
   C[3] = C[4] = 1;

   glutReshapeFunc( reshape );
   glutKeyboardFunc( keydown );
   glutKeyboardUpFunc( keyup );
   glutSpecialFunc( specialkey );
   glutDisplayFunc( display );

   glutIdleFunc(idle);

   glutMainLoop();
   return 0;
}
