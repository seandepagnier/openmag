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

/* this program applies my calibration algorithms to a bunch of raw
   measurements input in stdin in the format:
    Gx     Gy     Gz     Mx     My     Mz    Decl   Incl   Roll   |G|    |M|     a      d

    The first 16 points are laser rotations (sets of 4)
*/

#include <stdlib.h>
#include <math.h>

#include <calibration/fit.h>
#include <calibration/compute.h>
#include <calibration/calibrationserver.h>

#include <linalg/vector.h>
#include <linalg/quaternion.h>

int main(int argc, char *argv[])
{
   float points[2][1000][3];

   const char *sensornames[2] = {"accel", "mag"};

   /* read in the points */
   int numpoints = 0;
   char line[4096];
   while(fgets(line, sizeof line, stdin))
     if(sscanf(line, "%f %f %f %f %f %f",
               &points[0][numpoints][0],
               &points[0][numpoints][1],
               &points[0][numpoints][2],
               &points[1][numpoints][0],
               &points[1][numpoints][1],
               &points[1][numpoints][2]) == 6)
      numpoints++;

   printf("read in %d points\n", numpoints);

   int sensor, i, j, k;
   float sensorX[2][9];

   /* run recursive least squares on all the points for each
      sensor to get normalized orthogonal coordinates */
   for(sensor=0; sensor<2; sensor++) {
      float J[numpoints][9], R[numpoints];
      InitialEstimateEllipse(points[sensor], numpoints, sensorX[sensor]+6, sensorX[sensor]+0);
      sensorX[sensor][0] = 1/sensorX[sensor][0];
      sensorX[sensor][1] = 1;
      sensorX[sensor][2] = 1;
      sensorX[sensor][3] = sensorX[sensor][4] = sensorX[sensor][5] = 0;

      for(i=0; i<4; i++) {
         for(j = 0; j<numpoints; j++)
            SetupFitRotatedEllipsoid(J[j], R+j, sensorX[sensor], points[sensor][j]);
         ApplyLeastSquares(sensorX[sensor], J, R, 9, numpoints);
      }

      for(i = 0; i<numpoints; i++)
         SetupFitRotatedEllipsoid(NULL, R+j, sensorX[sensor], points[sensor][j]);

      float dev = CalcStdDeviation(R, numpoints);
      
      printf("%s: (%.3g %.3g %.3g) {%.3g %.3g %.3g} [%.4g %.4g] %.3g %.3g\n",
                  sensornames[sensor],
                  sensorX[sensor][6], sensorX[sensor][7], sensorX[sensor][8],
                  sensorX[sensor][3], sensorX[sensor][4], sensorX[sensor][5],
                  sensorX[sensor][1], sensorX[sensor][2], 1/sensorX[sensor][0],
                  dev);
   }

   /* calculate the misalignment between the two sensors */
   float alignX[4] = {0};
   float J[numpoints][4], R[numpoints];
   /* apply previous calibration to each point */
   for(sensor=0; sensor<2; sensor++) {
      for(i = 0; i<numpoints; i++) {
         ApplyRotatedEllipsoid(sensorX[sensor], points[sensor][i], 0);
         vector_scale(sensorX[sensor][0], points[sensor][i], 3);
      }
   }

   for(i=0; i<4; i++) {
      for(j = 0; j<numpoints; j++)
         SetupFitQuaternion(J[j], R+j, alignX, points[0][j], points[1][j]);
      ApplyLeastSquares(alignX, J, R, 4, numpoints);
   }

   for(i = 0; i<numpoints; i++)
      SetupFitQuaternion(NULL, R+j, alignX, points[0][j], points[0][j]);

   float dev = CalcStdDeviation(R, numpoints);
      
   printf("align: {%.3g %.3g %.3g} <%.3g> %.3g %g degrees\n",
          alignX[0], alignX[1], alignX[2], asin(alignX[3])*180/M_PI,
          dev, 360/M_PI*asin(magnitude(magalignX)));

   /* apply the quaternion rotation */
   for(i = 0; i<numpoints; i++)
      ApplyQuaternion(points[1][i], alignX, 0);

   /* now calculate the laser alignment for each of the first 4 points */
   float laseralignment[2][4][4];
   for(sensor = 0; sensor<2; sensor++)
      for(i = 0; i<4; i++) {
         AlignAxis(laseralignment[sensor][i], points[sensor] + i*4, 4);
         printf("alignment for %s %d: <%g %g %g %g> %g degrees\n",
                sensornames[sensor], i,
                laseralignment[sensor][i][0],
                laseralignment[sensor][i][1],
                laseralignment[sensor][i][2],
                laseralignment[sensor][i][3],
                180 - 360/M_PI*asin(laseralignment[sensor][i][0]));
      }

   /* now apply the rotations to the points and output
      azimuth, incline, roll, and dip */
   for(i = 0; i<numpoints; i++) {
      for(sensor=0; sensor<2; sensor++)
         rotvecquat(points[sensor][i], laseralignment[sensor][1]);
      float azimuth, incline, roll;
      incline = compute_pitch(points[0][i]);
      roll = compute_roll(points[0][i]);

      float m[3] = {points[1][i][0], points[1][i][1], points[1][i][2]};
      rotateoutmag(m, points[0][i]);
      azimuth = atan2(-m[1], m[0]) * 180 / M_PI;

      if(azimuth < 0)
         azimuth += 360;

      printf("%d:\t%.1f\t%.1f\t%.1f\t%.4f\t%.4f\n", i+1, azimuth, incline, roll,
             magnitude(points[0][i]), magnitude(points[1][i]));
   }

   return 0;
}
