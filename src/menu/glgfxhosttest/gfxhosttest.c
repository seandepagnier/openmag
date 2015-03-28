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

#include <fcntl.h>
#include <unistd.h>

#include <gfx/gfx.h>
#include <gfx/font.h>

#include <string.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include "common.h"

uint16_t framebuffer[WIDTH*HEIGHT];

float accel[3], mag[3];

void setpixel(int x, int y)
{
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        printf("warning, pixel (%d, %d) out of range\n", x, y);
        return;
    }

    uint16_t *p = framebuffer + (y*WIDTH + x);
    p[0] = 0xffff;
}

static void drawstuff()
{
   draw_gltinystuff();
   //       gfx_circle(WIDTH/2, HEIGHT/2, HEIGHT/3);
    //    gfx_ellipse_1(0, accel[1], WIDTH/2, HEIGHT/2, 100);
    //    gfx_ellipse(0, accel[1], WIDTH/2, HEIGHT/2, 120);

       //   gfx_ellipse(accel[0]/accel[2], accel[1]/accel[2], WIDTH/2, HEIGHT/2, 120);


    //    gfx_compass(accel, mag, WIDTH/2, HEIGHT/2, 64);


   //   gfx_string(20, 50, ALIGN_CENTER, fonts + 0, "hi there! %^#783jnkeKLE");
   //   gfx_string(0, 10, ALIGN_LEFT, fonts + 1, "hi there! %^#783jnkeKLE");
}

static void display( void )
{
   glClear(GL_COLOR_BUFFER_BIT);

   memset(framebuffer, 0, sizeof framebuffer);
   drawstuff();

   char fb24[WIDTH*HEIGHT*3];
   int x, y;
   for(x = 0; x<WIDTH; x++)
      for(y = 0; y<HEIGHT; y++) {
         fb24[(y*WIDTH+x)*3+0] = framebuffer[y*WIDTH+x] >> 8;
         fb24[(y*WIDTH+x)*3+1] = framebuffer[y*WIDTH+x] >> 8;
         fb24[(y*WIDTH+x)*3+2] = framebuffer[y*WIDTH+x] >> 8;
      }

   glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, WIDTH, HEIGHT,
                0, GL_RGB, GL_UNSIGNED_BYTE, fb24);

#if 0
   glTranslatef(.5, .5, 0);
   glRotatef(1, 0, 0, 1);
   glTranslatef(-.5, -.5, 0);
#endif

   glBegin(GL_QUADS);
   glTexCoord2f(    0,      0); glVertex2d(0, 5.0/6.0);
   glTexCoord2f(WIDTH,      0); glVertex2d(1, 5.0/6.0);
   glTexCoord2f(WIDTH, HEIGHT); glVertex2d(1, 1.0/6.0);
   glTexCoord2f(    0, HEIGHT); glVertex2d(0, 1.0/6.0);
   glEnd();

   glutSwapBuffers();
   glutPostRedisplay();
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
}

static void key(unsigned char k, int x, int y)
{
    switch (k) {
    case 27:
        exit(0);
        break;
    }
}

static void idle(void)
{
   char buffer[1024];
   while(fgets(buffer, sizeof buffer, stdin)) {
      float ax, ay, az;
      if(sscanf(buffer, "accel: %f %f %f\n", &ax, &ay, &az) == 3)
         accel[0] = ax, accel[1] = ay, accel[2] = az;
      else
      if(sscanf(buffer, "mag: %f %f %f\n", &ax, &ay, &az) == 3)
         mag[0] = ax, mag[1] = ay, mag[2] = az;
      glutPostRedisplay();
   }
}

int main( int argc, char *argv[] )
{
   glutInit(&argc, argv);
   glutInitWindowSize(WIDTH, HEIGHT);
   glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE | GLUT_DOUBLE);
   glutCreateWindow("gfxhosttest");

   glutReshapeFunc( reshape );
   glutDisplayFunc( display );
   glutKeyboardFunc(key);

   glutIdleFunc(idle);
   fcntl(0, F_SETFL, O_NONBLOCK);

   glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glEnable(GL_TEXTURE_RECTANGLE_ARB);
   glClearColor (0.4, 0.0, 0.4, 0.0);

   init_gltiny();

   glutMainLoop();
   return 0;
}
