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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <linalg/vector.h>

#include <calibration/fit.h>

#define NOSERVER
#include <calibration/calibrationserver.h>

#if 1
#define SCALE 1333.0
#define BIASX 0
#define BIASY 0
#define BIASZ 0
#else
#define SCALE 47000.0
#define BIASX 37589
#define BIASY 20718
#define BIASZ -29927
#endif

float sqr(float x) { return x*x; }

/* nth order rotated ellipsoid
   you can try various orders then look at the calculated coefficients
   to decide which order is needed for a given sensors

   x+y+z = 1
   xx+yy+zz+xy+yz+zx+x+y+z = 1
   xxx+yyy+zzz+xxy+xxz+yyx+yyz+zzx+zzy+xyz+xx+yy+zz+xy+yz+xz+x+y+z = 1
   ...
 */

struct state {
  int x, y, z;
} states[1000];
int statecount;

void state_add(int x, int y, int z)
{
  int i;
  struct state s = {x, y, z};
  for(i = 0; i<statecount; i++)
    if(state_eq(s, states[i]))
      return; /* already have it */
  states[statecount++] = s;
}

void state_permutate(int x, int y, int z, int order)
{
  if(x || y || z)
    state_add(x, y, z);
  if(order--) {
    state_permutate(x+1, y, z, order);
    state_permutate(x, y+1, z, order);
    state_permutate(x, y, z+1, order);
  }
}

void state_remove(int x, int y, int z)
{
  struct state s = {x, y, z};
  int i;
  for(i = 0; i<statecount; i++)
    if(state_eq(s, states[i])) {
      memmove(states+i, states+i+1, (statecount-i-1)*sizeof(*states));
      statecount--;
      return;
    }
  printf("warning, failed to remove state <%d %d %d>\n", x, y, z);
}

int state_eq(struct state a, struct state b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

void state_show(struct state s)
{
  void showcoeff(int num, char c) {
    int i;
    for(i=0; i<num; i++)
      printf("*%c", c);
  }
  showcoeff(s.x, 'x');
  showcoeff(s.y, 'y');
  showcoeff(s.z, 'z');
}

int main(int argc, char *argv[])
{
  float scale = 1;
  int order = 2;

  float points[1000][6];
  int count = 0;
  char buffer[1024];
  while(fgets(buffer, sizeof buffer, stdin)) {
    if(sscanf(buffer, "%f %f %f %f %f %f",
              points[count]+0, points[count]+1, points[count]+2,
	      points[count]+3, points[count]+4, points[count]+5) < 3)
      printf("error, line(%d) has less than 3 values\n", count);
    points[count][0] /= scale;
    points[count][1] /= scale;
    points[count][2] /= scale;
    count++;
  }

  int i, j;

  state_permutate(0, 0, 0, order);

  /* get rid of some states */

#if 0
  state_remove(1, 1, 0);
  state_remove(0, 1, 1);
  state_remove(1, 0, 1); // accel needs
#endif

#if 0
  state_remove(2, 2, 0);
  state_remove(0, 2, 2);
  state_remove(2, 0, 2);
#endif

#if 0
  state_remove(1, 2, 0); // accel needs
  state_remove(2, 1, 0);
  state_remove(0, 1, 2);
  state_remove(0, 2, 1); // accel needs
  state_remove(1, 0, 2); // accel needs
  state_remove(2, 0, 1); // accel needs
#endif

#if 1
#if 1
  state_remove(1, 1, 2);
  state_remove(1, 2, 1);
  state_remove(2, 1, 1);
  state_remove(1, 1, 1);
#endif

#if 1
  state_remove(0, 1, 3);
  state_remove(0, 3, 1);
  state_remove(1, 0, 3);
  state_remove(1, 3, 0);
  state_remove(3, 1, 0);
  state_remove(3, 0, 1);
#endif
#endif

  float J[count][statecount];
  float R[count];
  float X[statecount];

  memset(X, 0, sizeof X);
  

  int iterations = 0;
  float total=1000000, lasttotal = 1/0.0;
  while(total < lasttotal) {
    lasttotal = total;
    total = 0;
    for(i = 0; i<count; i++) {
      R[i] = 1;
      for(j = 0; j<statecount; j++) {
        J[i][j] = pow(points[i][0], states[j].x)
          * pow(points[i][1], states[j].y)
          * pow(points[i][2], states[j].z);
        R[i] -= X[j] * J[i][j];
      }
      total += R[i]*R[i];
    }
    
    if(ApplyLeastSquares(X, J, R, statecount, count))
      printf("failed to invert matrix!!\n");
    iterations++;
  }

  for(i=0; i<statecount; i++) {
//    state_show(states[i]);
    printf("X[%d] = %g;\n",i, X[i]);
  }

  for(i=0; i<statecount; i++) {

    if(i)
      printf(" + ");

//    putchar('A' + i);
    printf("%.10f", X[i]);
    state_show(states[i]);
  }
  printf(" = 1\n");
     

  float dev = sqrt(total / count);
  printf("%d: %.10f  error: %.4f degrees  states: %d\n",
         iterations, dev,
         asin(dev) * 180 / M_PI,
         statecount);

#if 1
/* now with the original algorithm */
  {
    statecount = 9;

  float J[count][statecount];
  float R[count];
  float X[statecount];

  memset(X, 0, sizeof X);
  X[3] = 1/SCALE;
  X[4] = X[5] = 1;
  X[0] = BIASX;
  X[1] = BIASY;
  X[2] = BIASZ;

  int iterations = 0;
  float total=1000000, lasttotal = 1/0.0;
  while(total < lasttotal) {
    lasttotal = total;
    total = 0;
    for(i = 0; i<count; i++) {
      SetupFitRotatedEllipsoid(J[i], R + i, X, points[i]);
      total += R[i]*R[i];
    }
    
    if(ApplyLeastSquares(X, J, R, statecount, count))
      printf("failed to invert matrix!!\n");
    iterations++;
  }

  for(i = 0; i<count; i++) {
    SetupFitRotatedEllipsoid(0, R + i, X, points[i]);
    printf("R[%d] = %f\n", i+1, R[i]);
  }

  float dev = sqrt(total / count);
  printf("%d: %.10f  error: %.4f degrees  states: %d\n",
         iterations, dev,
         asin(dev) * 180 / M_PI,
         statecount);

# if 1
  printf("X = [%g, %g, %g, %g, %g, %g, %g, %g, %g]\n",
         X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8]);
# endif
  }
#endif
#if 1
/* new algorithm */
  {
    statecount = 15;

  float J[count][statecount];
  float R[count];
  float X[statecount];

  memset(X, 0, sizeof X);
  X[3] = 1/SCALE;
  X[4] = X[5] = 1;
  X[0] = BIASX;
  X[1] = BIASY;
  X[2] = BIASZ;

  int iterations = 0;
  float total=1000000, lasttotal = 1/0.0;
  while(total < lasttotal) {
    int i;
    lasttotal = total;
    total = 0;
    for(i = 0; i<count; i++) {
      SetupFitRotatedEllipsoid3rdOrder(J[i], R + i, X, points[i]);
      total += R[i]*R[i];
    }
    
    if(ApplyLeastSquares(X, J, R, statecount, count))
      printf("failed to invert matrix!!\n");
    iterations++;
  }

  float dev = sqrt(total / count);
  printf("%d: %.10f  error: %.4f degrees  states: %d\n",
         iterations, dev,
         asin(dev) * 180 / M_PI,
         statecount);
# if 1
  printf("X = (%g %g %g) %g [%g %g] "
	 "<%g %g %g %g %g %g %g %g %g>\n",
         X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8], X[9], X[10],
	 X[11], X[12], X[13], X[14]);
# endif
  }
#endif

#if 1
/* new transformation algorithm */
  {
    statecount = 13;

  int m = 2;
  float J[count*m][statecount];
  float R[count*m];
  float X[statecount];

  memset(X, 0, sizeof X);
  X[3] = 1/SCALE;
  X[4] = X[5] = 1;
  X[0] = BIASX;
  X[1] = BIASY;
  X[2] = BIASZ;
  X[12] = .91;

  int iterations = 0;
  float total=1000000, lasttotal = 1/0.0;
  while(total < lasttotal) {
    int i;
    lasttotal = total;
    total = 0;
    for(i = 0; i<count; i++) {
      SetupFitTransformationMatrix_m(J[m*i], R + m*i, X, points[i]);
      total += R[m*i]*R[m*i];
      SetupFitTransformationMatrix_g(J[m*i+1], R + m*i+1, X, points[i], points[i]+3);
      total += R[m*i+1]*R[m*i+1];
    }
    
    if(ApplyLeastSquares(X, J, R, statecount, count*m))
      printf("failed to invert matrix!!\n");
    iterations++;
  }

  float dev = sqrt(total / (m*count));
  printf("%d: %.10f  error: %.4f degrees  states: %d\n",
         iterations, dev,
         asin(dev) * 180 / M_PI,
         statecount);
# if 1
  printf("X = (%g %g %g) %g [%g %g] "
	 "<%g %g %g %g %g %g> %g\n",
         X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8], X[9], X[10],
	 X[11], X[12]);
# endif
  }
#endif

  return 0;
}
