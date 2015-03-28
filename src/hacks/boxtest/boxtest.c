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

#include <string.h>

#include <GL/glut.h>

#define WIDTH 640
#define HEIGHT 480

float pitch, roll, yaw;

static void display( void )
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glLoadIdentity ();             /* clear the matrix */
           /* viewing transformation  */
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glScalef (1.0, 2.0, 1.0);      /* modeling transformation */ 

   glPushMatrix();
   glRotatef(yaw, 0, -1, 0);
   glRotatef(pitch, 1, 0, 0);
   glRotatef(roll, 0, 0, 1);
   glutWireCube (1.0);
   glPopMatrix();

   glutSwapBuffers();
}

/* new window size or exposure */
static void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
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
      sscanf(buffer, "%f %f %f\n", &pitch, &roll, &yaw);
      glutPostRedisplay();
   }
}

int main( int argc, char *argv[] )
{
   glutInit(&argc, argv);
   glutInitWindowSize(WIDTH, HEIGHT);
   glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE | GLUT_DOUBLE);
   glutCreateWindow("boxtest");

   glutReshapeFunc( reshape );
   glutDisplayFunc( display );
   glutKeyboardFunc(key);

   fcntl(0, F_SETFL, O_NONBLOCK);
   glutIdleFunc(idle);

   glutMainLoop();
   return 0;
}
