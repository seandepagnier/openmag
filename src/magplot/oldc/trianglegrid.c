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

/* take points and form a triangle mesh using them */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <GL/gl.h>

#include <linalg/vector.h>

#define die(...) do { fprintf(stderr, ##__VA_ARGS__); exit(1); } while(0)
#define MAX(X, Y) (X > Y ? X : Y)
#define MIN(X, Y) (X < Y ? X : Y)

struct point {
  float p[3];
  float c;
  struct point *next;
};

static struct point *points = NULL;

struct triangle {
  struct point *p[3]; /* points */
  struct triangle *next;
};

static struct triangle *triangles = NULL;

struct point *newpoint(float p[3], float c)
{
  struct point *point = malloc(sizeof *point);
  memcpy(point->p, p, sizeof point->p);
  memcpy(&point->c, &c, sizeof point->c);
  point->next = points;
  points = point;
  return point;
}

/* return positive if facing up, negative if facing down, 0 if linear */
static float tridir(struct point *p0, struct point *p1, struct point *p2)
{
  return (p1->p[0] - p0->p[0])*(p2->p[1] - p0->p[1])
       - (p2->p[0] - p0->p[0])*(p1->p[1] - p0->p[1]);

}

static float square(float x) { return x*x; }

static float dist_2d(struct point *p0, struct point *p1)
{
  float v[2];
  vector_sub(v, p0->p, p1->p, 2);
  return 0;//sqrt(v[0]*v[0] + v[1]*v[1]);
}

static void triangle_longest_side(struct triangle *t, int *ind)
{
  /* find the longest side */
  int i;
  float max = -1;
  for(i=0; i<3; i++) {
    float d = dist_2d(t->p[i], t->p[(i+1)%3]);
    if(d > max) {
      max = d;
      *ind = i;
    }
  }
}

/* find a triangle with the given two points, and report the index of the
   first point in ind */
static struct triangle *find_triangle_with_side(struct point *s0,
						struct point *s1, int *ind)
{
  struct triangle *t;
  int i;
  for(t=triangles; t; t=t->next)
    for(i = 0; i<3; i++)
      if(t->p[i] == s0 && t->p[(i+1)%3] == s1) {
	*ind = i;
	return t;
      }
  return NULL;
}

void deletetriangle(struct triangle *t)
{
  struct triangle **i;
  for(i=&triangles; *i; i=&(*i)->next)
    if(*i == t) {
      *i = t->next;
      free(t);
      break;
    }
}

void newtriangle(struct point *p0, struct point *p1, struct point *p2)
{
  struct triangle *t = malloc(sizeof *t);
  t->p[0] = p0;

  /* make only right orientation */
  float c = tridir(p0, p1, p2);
  if(c > 0) {
    t->p[1] = p1;
    t->p[2] = p2;
  } else if(c < 0) {
    t->p[1] = p2;
    t->p[2] = p1;
  } else /* don't add points that are really lines */
    return;

  struct point side[2];
  int it, iu;
  triangle_longest_side(t, &it);
  struct triangle *u = find_triangle_with_side(t->p[(it+1)%3], t->p[it], &iu);
  if(u) { /* split the triangles */
//    newtriangle(t->p[(it+2)%3], t->p[it], u->p[(iu+2)%3]);
//    newtriangle(u->p[(iu+2)%3], u->p[iu], t->p[(it+2)%3]);
    deletetriangle(t);
//    deletetriangle(u);
  }

  t->next = triangles;
  triangles = t;
}

/* return 1 if point is in the triangle, zero otherwise */
static int point_in_triangle(struct point *p, struct triangle *t)
{
  float dot2(float x[2], float y[2]) { return x[0]*y[0] + x[1]*y[1]; }
  float v0[2], v1[2], v2[2];
  vector_sub(v0, t->p[2]->p, t->p[0]->p, 2);
  vector_sub(v1, t->p[1]->p, t->p[0]->p, 2);
  vector_sub(v2, p->p, t->p[0]->p, 2);

  float dot00 = dot2(v0, v0), dot01 = dot2(v0, v1), dot02 = dot2(v0, v2);
  float dot11 = dot2(v1, v1), dot12 = dot2(v1, v2);
  
  float invd = 1 / (dot00 * dot11 - dot01 * dot01);

  float u = (dot11 * dot02 - dot01 * dot12) * invd;
  float v = (dot00 * dot12 - dot01 * dot02) * invd;

  return (u >= 0) && (v >= 0) && (u + v <= 1);
}


static int line_intersects_line(struct point *p0, struct point *p1,
				struct point *q0, struct point *q1)
{
  if(tridir(p0, p1, q0) * tridir(p0, p1, q1) < 0
     && tridir(q0, q1, p0) * tridir(q0, q1, p1) < 0)
    return 1;
  return 0;
}

static int triangle_intersects_triangle(struct triangle *t1,
					struct triangle *t2)
{
  int i, j;
  for(i = 0; i<3; i++)
    for(j = 0; j<3; j++)
      if(line_intersects_line(t1->p[i], t1->p[(i+1)%3],
			      t2->p[j], t2->p[(j+1)%3]))
	return 1;
  return 0;
}

static struct triangle *
triangle_intersects_any_triangle(struct triangle *triangles,
				 struct triangle *t)
{
  struct triangle *i;
  for(i = triangles; i; i=i->next)
    if(triangle_intersects_triangle(t, i))
      return i;
  return NULL;
}

void trianglegrid_addpoint(float p[3], float c)
{
  /* don't allow duplicate points */
  struct point *np;
  for(np = points; np; np = np->next)
    if(dist(p, np->p) < .1)
      return;

  np = newpoint(p, c);

#if 1
  /* If the new point lies in any triangle, split it 3 ways */
  struct triangle *t;
  for(t = triangles; t; t=t->next) {
    if(point_in_triangle(np, t)) {
      newtriangle(np, t->p[1], t->p[2]);
      newtriangle(t->p[0], np, t->p[2]);
#if 1
      newtriangle(t->p[0], t->p[1], np);
      deletetriangle(t);
#else
      t->p[2] = np;
#endif
      return;
    }
  }
#endif

  /* find all the points close enough to build triangles with */
  struct point *cpoints[100], *pc;
  int numcpoints = 0;
  for(pc = points; pc; pc=pc->next) {
    float d = dist(pc->p, np->p);
    if(d < 2) {
      cpoints[numcpoints++] = pc;
      if(numcpoints == (sizeof cpoints) / (sizeof *cpoints))
	die("out of cpoints\n");
    }
  }

  /* Do not make a triangle if it will overlap with an existing triangle,
        unless it the triangle was just made, in that case split
        the existing triangle adding the new point.
  */
  struct triangle *origtriangles = triangles;
  int i, j, k;
  for(i=0; i<numcpoints; i++) {
    for(j=i+1; j<numcpoints; j++) {
      struct triangle nt = {{np, cpoints[i], cpoints[j]}};
      struct triangle *t = triangle_intersects_any_triangle(triangles,
							    &nt);
      /* don't make triangles that intersect existing triangles */
      if(!t)
	newtriangle(np, cpoints[i], cpoints[j]);
    }
  }
}

static void colorspace(float c, float rgb[3])
{
  int i;
  for(i=0; i<3; i++) {
    rgb[i] = 1 - 2*fabs(i/2.0-c);
    if(rgb[i] < 0)
      rgb[i] = 0;
  }
}

static void make_gl_point(struct point *p, float min, float max)
{
  float c = (p->c - (min + max) / 2) / (max - min) + .5;
  float rgb[3];
  colorspace(c, rgb);
//  glColor3fv(rgb);
  glColor3f(p->p[0], p->p[1], p->p[2]);
  glVertex3fv(p->p);
}

void trianglegrid_render(void)
{
  float min=10, max=-1;
  struct point *p;
  for(p = points; p; p=p->next) {
    if(p->c < min)
      min = p->c;
    if(p->c > max)
      max = p->c;
  }
  
  struct triangle *t;
  glBegin(GL_TRIANGLES);
  for(t = triangles; t; t=t->next) {
    int i;
    for(i=0; i<3; i++)
      make_gl_point(t->p[i], min, max);
  }
  glEnd();

  glBegin(GL_POINTS);
  for(p = points; p; p=p->next)
    make_gl_point(p, min, max);
  glEnd();

}
