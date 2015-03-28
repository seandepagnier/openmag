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
#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>

#include <unistd.h>
#include <errno.h>

#include <string.h>

#include <poll.h>

#include <debug.h>

#include <linalg/vector.h>

#include <calibration/fit.h>

#include <GL/glut.h>

#define MAX_POINTS 10000

float points[MAX_POINTS][3];
int numpoints = 0;
float scalea[3] = {50000, 50000, 50000}, biasa[3];
float curpoint[3];

float userscale = 1;

/* host app for calibration experimentation */

/* Read in integer triples from standard input, and display
   a best fit ellipoid to the data with opengl */

static int interactive;

static char prefix[64] = "";

static void TranslateAfter(float x, float y, float z)
{
   float m[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, m);
   glLoadIdentity();
   glTranslatef(x, y, z);
   glMultMatrixf(m);
}

static void RotateAfter(float ang, float x, float y, float z)
{
   float m[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, m);
   glLoadIdentity();
   glRotatef(ang, x, y, z);
   glMultMatrixf(m);
}

static void display( void )
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix(); /*MODELVIEW*/

   double s = 1/magnitude(scalea) * userscale;

   glScalef(s, s, s);
   TranslateAfter( 0, 0, -1 );

   /* draw points */
   glPushMatrix();

   glColor3f(1,0,0);
   glTranslatef(-biasa[0], -biasa[1], -biasa[2]);
   int i;
   glBegin(GL_POINTS);
   for(i = 0; i<numpoints; i++)
      glVertex3f(points[i][0],
                 points[i][1],
                 points[i][2]);

   glEnd();

   glColor3f(0,1,0);
   glBegin(GL_LINES);
   glVertex3fv(curpoint);
   glVertex3f(biasa[0], biasa[1], biasa[2]);
   glEnd();

   glPopMatrix();

   /* draw bestfit ellipse */
   glColor3f(1,1,1);
   glPushMatrix();
   glScalef(scalea[0], scalea[1], scalea[2]);
//   glutWireSphere(1, 16, 8);
   glPopMatrix();

   glPopMatrix();

   glutSwapBuffers();
}

static void idle(void)
{
   static char buf[256];
   static int bufpos;

   int i;
   for(i = 0; i<100; i++) {
      switch(read(0, buf + bufpos, 1)) {
      case 0:
         glutIdleFunc(NULL);
      case -1:
         break;
      }

      if(buf[bufpos++] != '\n')
         continue;

      bufpos = 0;

      float p[3];
      if(sscanf(buf, "mag: %f %f %f\n", p+0, p+1, p+2) == 3)
         memcpy(curpoint, p, sizeof p);
      else
         if(sscanf(buf, "%f %f %f\n", p+0, p+1, p+2) == 3) {
//      printf("got a point!\n");
            memcpy(points[numpoints++], p, sizeof p);
         }
   }

   if(interactive)
       glutPostRedisplay();
   else
      exit(0);
}

const float step = 5;
static void key(unsigned char k, int x, int y)
{
   static GLuint mode = 0;
   (void) x;
   (void) y;
   switch (k) {
   case '\b':
      RotateAfter(step, 0, 0, -1);
      break;
   case 27:
      exit(0);
   }
   glutPostRedisplay();
}

static void specialkey(int key, int x, int y)
{
   (void) x;
   (void) y;
   switch (key) {
   case GLUT_KEY_UP:
      RotateAfter(step, 1, 0, 0);
      break;
   case GLUT_KEY_DOWN:
      RotateAfter(step, -1, 0, 0);
      break;
   case GLUT_KEY_LEFT:
      RotateAfter(step, 0, 1, 0);
      break;
   case GLUT_KEY_RIGHT:
      RotateAfter(step, 0, -1, 0);
      break;
   case GLUT_KEY_PAGE_UP:
      userscale *= 1.1;
      break;
   case GLUT_KEY_PAGE_DOWN:
      userscale *= .9;
      break;
   case GLUT_KEY_INSERT:
      RotateAfter(step, 0, 0, 1);
      break;
   }
   glutPostRedisplay();
}


/* new window size or exposure */
static void reshape(int width, int height)
{
   GLfloat ar = (float) width / (float) height;
   glViewport(0, 0, (GLint)width, (GLint)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum( -.1*ar, .1*ar, -.1, .1, .1, 1500 );
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

static void init( void )
{
   glEnable(GL_DEPTH_TEST);
   glClearColor(0, 0, 0, 0);
   glPointSize(5);
}

/* usage: [-n]
   -m   mag mode
   -n   no display
*/
int main( int argc, char *argv[] )
{
   int arg;
   int initpoints = 1;
   interactive = 1; /* default */

   for(arg = 1; arg < argc; arg++) {
      if(!strcmp(argv[arg], "-n"))
         interactive = 0;
      if(!strcmp(argv[arg], "-h")) {
         printf("-n not graphical and interactive\n");
      }
   }
   

   //   CalcBestFit(sensor);

   if(!interactive)
      idle();

   /* interactive mode */
   glutInit(&argc, argv);
   glutInitWindowPosition(0, 0);
   glutInitWindowSize(600, 500);
   glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
   glutCreateWindow("Points!");

   glutIdleFunc(idle);

   glutReshapeFunc( reshape );
   glutKeyboardFunc( key );
   glutSpecialFunc( specialkey );
   glutDisplayFunc( display );

   fcntl(0, F_SETFL, O_NONBLOCK);

   init();

   glutMainLoop();
   return 0;
}
