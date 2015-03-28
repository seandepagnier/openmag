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

/*
 * Plot mag and gps data using triangles
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>

#include <math.h>
#include <GL/glut.h>

#include <linalg/vector.h>
#include <linalg/rotate.h>

#include "trianglegrid.h"

#define die(...) do { fprintf(stderr, ##__VA_ARGS__); exit(1); } while(0)

static FILE *output;
static int gps = -1, mag = -1;
static int noshow = 0;

static float gpspos[3] = {NAN}, horizdiution = 1;

static int readline(int fd, char *buf, int *off)
{
  while(read(fd, buf+*off, 1) == 1) {
    if(buf[*off] == '\n') {
         buf[*off+1] = '\0';
         *off = 0;
	 return 1;
    }
    if(++*off == 128) {
      fprintf(stderr, "out of buffer\n");
      *off = 0;
    }
  }
  return 0;
}

void reportdata(float p[3], float magmag)
{
  if(output) {
    fprintf(output, "%f %f %f %f\n", p[0], p[1], p[2], magmag);
    fflush(output);
  }

  trianglegrid_addpoint(p, magmag);
  if(!noshow)
    glutPostRedisplay();
}

/* convert from lat, lon, and alt to vector in meters,
   starting with the first position as the origin */
static void convert_position(float p[3], float lat, float lon, float alt)
{
  static float flat=10000, flon, falt;
  if(flat == 10000) {
    flat = lat;
    flon = lon;
    falt = alt;
  }

  const float earthradius = 6360000.0;
  p[0] = earthradius * sin(deg2rad(lon - flon)) * cos(deg2rad(lat));
  p[1] = earthradius * sin(deg2rad(lat - flat));
  p[2] = (alt - falt) / 10;
}

void idle(void)
{
  static float magmag;

   if(mag != -1) { /* read from mag */
     static char buf[256];
     static int off;
     float m[3];
     while(readline(mag, buf, &off))
       if(sscanf(buf, "mag: %f %f %f", m+0, m+1, m+2) == 3)
	 magmag = magnitude(m);
   }

  if(gps != -1) { /* read from gps */
    static char buf[256];
    static int off;
    while(readline(gps, buf, &off)) {
      char ns, ew;
      float time, north, east;
      int fix, satcount;
      float alt;
      if(sscanf(buf, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f",
		&time, &north, &ns, &east, &ew,
		&fix, &satcount, &horizdiution, &alt) == 9) {
	if(!fix) {
	  printf("no gps fix, sats: %d\n", satcount);
	  continue;
	}

	if(ns == 'S')
	  north = -north;
	if(ns == 'W')
	  east = -east;
	/* take the last mag reading and report datapoint */
	convert_position(gpspos,
			 (int)(north/100) + fmod(north, 100)/60,
			 (int)(east/100) + fmod(east, 100)/60,
			 alt);
	static float lastpos[3] = {-1, -1, -1};
	if(dist(gpspos, lastpos) > .25 && magmag) {
	  reportdata(gpspos, magmag);
	  memcpy(lastpos, gpspos, sizeof lastpos);
	}

	if(!noshow)
	  glutPostRedisplay();
      }
    }
  }
}

static float tx, ty, tz = -3;
void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   glPushMatrix();
   glTranslatef(tx, ty, tz);

   trianglegrid_render();

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glPushMatrix();
   glTranslatef(gpspos[0], gpspos[1], gpspos[2]);
   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1, 1, 1, 1);
   glVertex3f(0, 0, 0);
   glColor4f(1, 0, 0, .2);
   float a;
   for(a = 0; a<2*M_PI+.1; a+=M_PI/10)
     glVertex3f(horizdiution*cos(a), horizdiution*sin(a), 0);

   glEnd();
   glPopMatrix();
   glDisable(GL_BLEND);

   glPopMatrix();

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   GLfloat ar = (float) w / (float) h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum( -.1*ar, .1*ar, -.1, .1, .1, 100);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
}

static void
key(unsigned char k, int x, int y)
{
   switch (k) {
  case 27:  /* Escape */
    exit(0);
    break;
   case 'w':
   {
     static int w;
     glPolygonMode(GL_FRONT_AND_BACK, (w = !w) ? GL_LINE: GL_FILL);
   } break;
   case 'f':
     glutFullScreen();
     break;
  default:
    return;
  }
  glutPostRedisplay();
}

static void RotateAfter(double ang, double x, double y, double z)
{
   double m[16];
   glGetDoublev(GL_MODELVIEW_MATRIX, m);
   glLoadIdentity();
   glRotated(ang, x, y, z);
   glMultMatrixd(m);
}

static void
special(int key, int x, int y)
{
  if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
   switch(key) {
   case GLUT_KEY_LEFT:        RotateAfter(5, 0, 1, 0);   break;
   case GLUT_KEY_RIGHT:       RotateAfter(-5, 0, 1, 0);  break;
   case GLUT_KEY_UP:          RotateAfter(5, 1, 0, 0);   break;
   case GLUT_KEY_DOWN:        RotateAfter(-5, 1, 0, 0);  break;
   case GLUT_KEY_PAGE_UP:      RotateAfter(5, 0, 0, 1);   break;
   case GLUT_KEY_PAGE_DOWN:      RotateAfter(-5, 0, 0, 1);  break;
   }
  } else {
   switch(key) {
   case GLUT_KEY_LEFT:  tx++; break;
   case GLUT_KEY_RIGHT: tx--; break;
   case GLUT_KEY_UP:    ty--; break;
   case GLUT_KEY_DOWN:  ty++; break;
   case GLUT_KEY_PAGE_UP:    tz--; break;
   case GLUT_KEY_PAGE_DOWN:  tz++; break;
   }
  }
   glutPostRedisplay();
}

void initinput(int argc, char **argv)
{
   const char usage[] =
      "Usage: magplot [OPTIONS]\n"
      "Options may include: \n"
      "-h, --help  print this message\n"
      "-f, --file [FILENAME] read input from a file\n"
      "-o, --output [FILENAME] write output to a file\n"
      "-n, --noshow  Don't display anything, just log data\n"
      "-g, --gps [DEVICE] device to get gps data from\n"
      "-m, --mag [DEVICE] device to get magnetometer data from\n";

   const struct option long_options[] = {
      {"help", 0, 0, 'h'},
      {"file", 1, 0, 'f'},
      {"output", 1, 0, 'o'},
      {"noshow", 0, 0, 'n'},
      {"gps", 1, 0, 'g'},
      {"mag", 1, 0, 'm'},
      {0, 0, 0, 0}};

   for(;;)
      switch(getopt_long(argc, argv, "hf:o:ng:m:", long_options, NULL)) {
      case -1:
	return;
      case 'h':
         fputs(usage, stdout);
         exit(0);
      case 'f':
      {
	FILE *file;
	if(!strcmp(optarg, "-"))
	  file = stdin;
	else
	  if(!(file = fopen(optarg, "r")))
	    die("Failed to open '%s' for reading\n", optarg);
	float p[3], magmag;
	int oldnoshow = noshow;
	noshow = 1;
	while(fscanf(file, "%f %f %f %f\n", p+0, p+1, p+2, &magmag) == 4)
	  reportdata(p, magmag);
	noshow = oldnoshow;
      } break;
      case 'o':
	if(!strcmp(optarg, "-"))
	  output = stdout;
	else if(!(output = fopen(optarg, "w")))
	  die("Failed to open '%s' for reading\n", optarg);
	break;
      case 'g':
	if((gps = open(optarg, O_RDONLY)) == -1)
	  die("failed to open device: %s\n", optarg);
	if(fcntl(gps, F_SETFL, O_NONBLOCK) == -1)
	  die("could not set gps to non-blocking\n");
	 break;
      case 'm':
      {
	char buf[1024];
	sprintf(buf, "./dataclient %s -c \"set sensors/mag/outputrate 2\" "
		"-c \"set sensors/mag/outputtype filteredcalibrated\"",
		optarg);
	system(buf);

	if((mag = open(optarg, O_RDWR)) == -1)
	  die("failed to open device: %s\n", optarg);
	if(fcntl(mag, F_SETFL, O_NONBLOCK) == -1)
	  die("could not set mag to non-blocking\n");
      } break;
      case 'n':
	noshow = 1;
	break;
      }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   initinput(argc, argv);

   if(noshow)
     for(;;) {
       idle();
       usleep(1);
     }

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
   glutCreateWindow ("magplot");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(key);
   glutSpecialFunc(special);
   glutIdleFunc(idle);

   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
