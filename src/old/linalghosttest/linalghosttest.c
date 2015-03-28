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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <calibration/fit.h>

#include <linalg/vector.h>
#include <linalg/matrix.h>
#include <linalg/quaternion.h>

#include <math.h>

int main(void)
{
   int c;

   float bias[3], scale[3];
   InitialEstimateSphere(points, n, bias, scale);
   if(CalcBestFitSphere(points, n, bias, scale)) {
      printf("sphere fit: <%f, %f, %f> (%f)\n", bias[0], bias[1], bias[2], scale[0]);
      printf("sphere deviation: %f\n", CalcAvgDeviation(points, n, bias, scale, 0));
   } else
      printf("sphere no fit\n");

   InitialEstimateEllipse(points, n, bias, scale);
   //   for(c = 0; c<5; c++)
   if(CalcBestFitEllipse(points, n, bias, scale)) {
      printf("ellipse fit: <%f, %f, %f> (%f, %f, %f)\n",
             bias[0], bias[1], bias[2],
             scale[0], scale[1], scale[2]);
      printf("ellipse deviation: %f\n", CalcAvgDeviation(points, n, bias, scale, 0));
   } else
      printf("ellipse no fit\n");

   InitialEstimateEllipse(points, n, bias, scale);
   float rotation[3] = {0};

   for(c = 0; c<50; c++)
   if(CalcBestFitRotatedEllipse(points, n, bias, rotation, scale)) {
      printf("rotated ellipse fit: <%f, %f, %f> (%f, %f, %f) {%f %f %f}\n",
             bias[0], bias[1], bias[2],
             scale[0], scale[1], scale[2],
             rotation[0], rotation[1], rotation[2]);
      printf("rotated ellipse deviation: %f\n",
             CalcAvgDeviation(points, n, bias, scale, rotation));
   } else
      printf("rotated ellipse no fit\n");

   /* alignment test, try to calculate what q1 is in q2 */
   const int num = 20;
   float q1[4] = {1, 2, 3, 4}, q2[4] = {.5, .5, .5, .5};
   quatnormalize(q1);

   srand(time(NULL));

   /* set the dip angle +- 90.. should work for +- 180*/
   const float ang = randnum()*90;

   /* fabricate some points */
   float pa[num][3], pb[num][3];
   for(c = 0; c<num; c++) {
       float p[3] = {randnum(), randnum(), randnum()};
      normalize(p);
      memcpy(pb[c], p, sizeof p);
//      printf("%d %f %f %f  ", c, p[0], p[1], p[2]);

      float qd[4], pp[3];
      float oo[3] = {randnum(), randnum(), randnum()};
      normalize(oo);
      cross(pp, p, oo);
      normalize(pp);
      angvec2quat(qd, M_PI/180 * ang, pp);
      rotvecquat(p, qd);

      rotvecquat(p, q1);
      /* add some noise */
      p[0] += randnum() / 100;
      p[1] += randnum() / 100;
      p[2] += randnum() / 100;

      memcpy(pa[c], p, sizeof p);
//      printf("%f %f %f\n", p[0], p[1], p[2]);
   }

   int hemi = 0;
   for(c = 0; c<1000; c++)
       if(CalcBestFitQuaternion(pa, pb, num, q2, hemi)) {
           if(c%100 == 0)
               printf("<%f %f %f %f> <%f %f %f %f>\n",
                      q1[0], q1[1], q1[2], q1[3],
                      q2[0], q2[1], q2[2], q2[3]);
       } else
           printf("quaternion fit failed\n");

   float m = quatmagnitude(q1), n = quatmagnitude(q2);
   quatnormalize(q1);
   quatnormalize(q2);

   printf("(%f %f %f %f) (%f %f %f %f) %f %f %f %f\n",
          q1[0], q1[1], q1[2], q1[3],
          q2[0], q2[1], q2[2], q2[3],
          m, n, ang, 180/M_PI*acos(1/(n*n)));

   return 0;
}
