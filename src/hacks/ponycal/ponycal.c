/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

/* this program applies my calibration algorithms to a bunch of raw
   measurements input in stdin in the format:
   NUM <accel accel accel> <mag mag mag> incline azimuth
*/

#include <stdlib.h>
#include <math.h>

#include <calibration/fit.h>
#include <calibration/calibrationserver.h>

#include <linalg/vector.h>

int main(int argc, char *argv[])
{
   struct point {
      int num;
      float data[2][3];
      float incline, azimuth;
   } points[1000];

   /* read in the points */
   int numpoints = 0;
   char line[4096];
   while(fscanf(stdin, "%d %f %f %f %f %f %f %f %f\n",
                &points[numpoints].num,
                &points[numpoints].data[0][0],
                &points[numpoints].data[0][1],
                &points[numpoints].data[0][2],
                &points[numpoints].data[1][0],
                &points[numpoints].data[1][1],
                &points[numpoints].data[1][2],
                &points[numpoints].incline,
                &points[numpoints].azimuth) == 9)
      numpoints++;

   printf("read in %d points\n", numpoints);

   int sensor, i, j, k;
   float sensorX[2][9];

   /* run recursive least squares on all the points for each
      sensor to get normalized orthogonal coordinates */
   for(sensor=0; sensor<2; sensor++) {
      float J[numpoints][9], R[numpoints];
      float p[numpoints][3];
      for(i = 0; i<numpoints; i++)
         for(j = 0; j<3; j++)
            p[i][j] = points[i].data[sensor][j];

      InitialEstimateEllipse(p, numpoints, sensorX[sensor]+6, sensorX[sensor]+0);
      sensorX[sensor][1] = 1;
      sensorX[sensor][2] = 1;
      sensorX[sensor][3] = sensorX[sensor][4] = sensorX[sensor][5] = 0;

      for(i=0; i<4; i++) {
         for(j = 0; j<numpoints; j++)
            SetupFitRotatedEllipsoid(J[j], R+j, sensorX[sensor], points[j].data[sensor]);
         ApplyLeastSquares(sensorX[sensor], J, R, 9, numpoints);
      }

      for(i = 0; i<numpoints; i++)
         SetupFitRotatedEllipsoid(NULL, R+j, sensorX[sensor], points[j].data[sensor]);

      float dev = CalcStdDeviation(R, numpoints);
      
      const char *sensornames[2] = {"accel", "mag"};
      printf("%s: (%.3g %.3g %.3g) {%.3g %.3g %.3g} [%.4g %.4g] %.3g %.3g\n",
             sensornames[sensor],
             sensorX[sensor][6], sensorX[sensor][7], sensorX[sensor][8],
             sensorX[sensor][3], sensorX[sensor][4], sensorX[sensor][5],
             sensorX[sensor][1], sensorX[sensor][2], 1/sensorX[sensor][0],
             dev);
   }

   /* apply previous calibration to each point */
   for(i = 0; i<numpoints; i++) {
      for(sensor = 0; sensor<2; sensor++) {
         ApplyRotatedEllipsoid(sensorX[sensor], points[i].data[sensor], 0);
         vector_scale(sensorX[sensor][0], points[i].data[sensor], 3);
      }
   }

   for(i = 0; i<numpoints; i++) {
      for(sensor = 0; sensor<2; sensor++)
        printf("<%f %f %f> %f ",
               points[i].data[sensor][0],
               points[i].data[sensor][1],
               points[i].data[sensor][2],
               magnitude(points[i].data[sensor]));
      printf("\n");
   }

   /* calculate the misalignment between the two sensors */
   float alignX[4] = {0};
   float J[numpoints][4], R[numpoints];
   for(i=0; i<4; i++) {
      for(j = 0; j<numpoints; j++)
         SetupFitQuaternion(J[j], R+j, alignX, points[j].data[0], points[j].data[1]);
      ApplyLeastSquares(alignX, J, R, 4, numpoints);
   }

   for(i = 0; i<numpoints; i++)
      SetupFitQuaternion(NULL, R+j, alignX, points[j].data[0], points[j].data[1]);

   float dev = CalcStdDeviation(R, numpoints);
      
   printf("align: {%.3g %.3g %.3g} <%.3g> %.3g\n",
          alignX[0], alignX[1], alignX[2], asin(alignX[3])*180/M_PI, dev);

   return 0;
}
