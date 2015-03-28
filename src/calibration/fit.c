/* Copyright (C) 2007, 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
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

#include <linalg/vector.h>
#include <linalg/rotate.h>
#include <linalg/quaternion.h>
#include <linalg/matrix.h>

#include <calibration/fit.h>

#include <stdint.h>
#include <string.h>
#include <debug.h>

/* n is the number of states, m the number of measurements

   applies the following:

   K = PH'(HPH' + R)^-1
   X = X + K*Z
   P = (I - KH)P(I-KH)' + KRK'

   K: nxm  P: nxn  H: mxn  R: mxm  Z: mx1  X: nx1
*/

int _ApplyRecursiveLeastSquares(float *P, const float *H, const float *R,
                                const float Z[], float X[],
                                int m, int n)
{
   float K[n][m];

   /* K = PH' */
   matrix_mult_tr(K, P, H, n, n, m);

   /* a = HPH' */
   float a[m][m];
   matrix_mult(a, H, K, m, n, m);

   /* a = HPH' + R */
   matrix_add(a, a, R, m, m);

   /* a = (HPH' + R)^-1 */
   if(matrix_invert(a, m)) {
      DEBUG("matrix invert failed!\n");
      return 1;
   }

   /* K = PH'(HPH' + R)^-1 */
   matrix_mult(K, K, a, n, m, m);

   /* b = K*Z */
   float b[n];
   matrix_mult(b, K, Z, n, m, 1);

   /* X = X + K*Z */
   vector_add(X, X, b, n);

   /* d = KH */
   float d[n][n];
   matrix_mult(d, K, H, n, m, n);

   /* negate d */
   int i, j; 
   for(i = 0; i<n; i++)
      for(j = 0; j<n; j++)
         d[i][j] = -d[i][j];

   /* add in the identity matrix */
   for(i = 0; i<n; i++)
      d[i][i]++;

   /* P = (I-KH)P(I-KH)' */
   matrix_mult_triple(P, d, P, n, n);

   /* d = KRK' */
   matrix_mult_triple(d, K, R, n, m);

   /* P = (I-KH)P(I-KH)' + KRK' */
   matrix_add(P, P, d, n, n);

   return 0;
}

/* setup recursive least squares to sphere with
   center: <X[0], X[1], X[2]>
   radius: X[3]

   input x is a 3 vector of the raw data

   (x-a)^2 + (y-b)^2 + (z-c)^2 = d^2
*/
void SetupFitSphere(float H[4], float Z[1], const float X[4], const float x[3])
{
   float d[3];
   vector_sub(d, x, X, 3);

   if(H) {
     H[0] = -2*d[0];
     H[1] = -2*d[1];
     H[2] = -2*d[2];
     H[3] = -2*X[3];
   }

   Z[0] = X[3]*X[3] - magnitude2(d);
}

/*
   like a normal sphere fit, but also takes a temperature input
   and uses 7 states estimating a linear fit of bias over temperature
   (x-a-t*d)^2 + (y-b-t*e)^2 + (z-c-t*f)^2 = g^2
*/
void ApplyThermalSphere(const float X[7], float x[3], float t)
{
   vector_sub(x, x, X, 3);
   float tv[3] = {X[3], X[4], X[5]};
   vector_scale(t, tv, 3);
   vector_sub(x, x, tv, 3);
}

void SetupFitThermalSphere(float H[7], float Z[1], const float X[7],
                           const float x[3], float t)
{
   float d[3] = {x[0], x[1], x[2]};
   ApplyThermalSphere(X, d, t);
   
   if(H) {
     H[0] = -2*d[0];
     H[1] = -2*d[1];
     H[2] = -2*d[2];
     H[3] = -2*t*d[0];
     H[4] = -2*t*d[1];
     H[5] = -2*t*d[2];
     H[6] = -2*X[6];
   }

   Z[0] = X[6]*X[6] - magnitude2(d);
}

/*
   for the rotated ellipsoid use the truth equation:
   m0*m0 + m1*m1 + m2*m2 = 1

   b = <x[0] - X[0], x[1] - X[1], x[2] - X[2]>

   m0 = (X[3]*(b[0]))
   m1 = (X[3]*(X[4]*b[1] + X[6]*b[0]))
   m2 = (X[3]*(X[5]*b[2] + X[7]*b[0] + X[8]*b[1]))

   Fill in the 9 partial derivatives into H, and the residule in Z
   X is the state, and x is the measurement

   take data in x, and apply the calibration state in X for the rotated
   ellipsoid.  If b is not null, then store just the bias updates there
*/
void ApplyRotatedEllipsoid(const float X[9], float x[3], float *b)
{
    float B[3];
    if(!b)
       b = B;

    vector_sub(b, x, X, 3);
    x[0] = b[0];
    x[1] = (X[4]*b[1] + X[6]*b[0]);
    x[2] = (X[5]*b[2] + X[7]*b[0] + X[8]*b[1]);
}

void SetupFitRotatedEllipsoid(float *H, float *Z, const float X[9],
                              const float x[3])
{
   float b[3], C[3] = {x[0], x[1], x[2]};
   ApplyRotatedEllipsoid(X, C, b);

   float mag = magnitude2(C);

   if(H) {
      float X3_2 = 2*X[3];
      float M = X3_2*X[3];

      int i;
      for(i=0; i<3; i++)
         C[i] *= M;

      H[0] = -(X[7]*C[2] + X[6]*C[1] + C[0]);
      H[1] = -(X[8]*C[2] + X[4]*C[1]);
      H[2] = -(X[5]*C[2]);
      H[3] = X3_2*mag;
      H[4] = b[1]*C[1];
      H[5] = b[2]*C[2];
      H[6] = b[0]*C[1];
      H[7] = b[0]*C[2];
      H[8] = b[1]*C[2];
   }

   Z[0] = 1 - X[3]*X[3]*mag;
}

/*
   for the 3rd order rotated ellipsoid use the truth equation:
   m0*m0 + m1*m1 + m2*m2 = 1

   b = <x[0] - X[0], x[1] - X[1], x[2] - X[2]>
   r = <b[0] +  X[9]*b[0]^2 + X[10]*b[0]^3,
        b[1] + X[11]*b[1]^2 + X[12]*b[1]^3,
        b[2] + X[13]*b[2]^2 + X[14]*b[2]^3>

   m0 = (X[3]*(r[0]))
   m1 = (X[3]*(X[4]*r[1] + X[6]*r[0]))
   m2 = (X[3]*(X[5]*r[2] + X[7]*r[0] + X[8]*r[1]))

     (X[3]*(((x[0] - X[0]) +  X[9]*(x[0] - X[0])^2 + X[10]*(x[0] - X[0])^3)))^2
   + (X[3]*(X[4]*((x[1] - X[1]) + X[11]*(x[1] - X[1])^2 + X[12]*(x[1] - X[1])^3)
      + X[6]*((x[0] - X[0]) +  X[9]*(x[0] - X[0])^2 + X[10]*(x[0] - X[0])^3)))^2
   + (X[3]*(X[5]*((x[2] - X[2]) + X[13]*(x[2] - X[2])^2 + X[14]*(x[2] - X[2])^3)
      + X[7]*((x[0] - X[0]) +  X[9]*(x[0] - X[0])^2 + X[10]*(x[0] - X[0])^3)
      + X[8]*((x[1] - X[1]) + X[11]*(x[1] - X[1])^2 + X[12]*(x[1] - X[1])^3)))^2

   15 unknowns needed

   take data in x, and apply the calibration state in X for the rotated
   ellipsoid with 3rd order coefficients.  If b is not null, then store
   just the bias updates there.  If r is not null, then store the 3rd order
   application there for each axis.
*/

void ApplyRotatedEllipsoid3rdOrder(const float X[15], float x[3], float *b, float *r)
{
   float B[3], R[3];
   if(!b)
      b = B;
   if(!r)
      r = R;

   vector_sub(b, x, X, 3);

   int i;
   for(i=0; i<3; i++) {
      float s = b[i]*b[i];
      r[i] = b[i] + X[2*i+9]*s + X[2*i+10]*s*b[i];
   }
   
   x[0] = r[0];
   x[1] = X[4]*r[1] + X[6]*r[0];
   x[2] = X[5]*r[2] + X[7]*r[0] + X[8]*r[1];
}

void SetupFitRotatedEllipsoid3rdOrder(float *H, float *Z, const float X[15],
                                      const float x[3])
{
   float b[3], r[3], C[3] = {x[0], x[1], x[2]};
   ApplyRotatedEllipsoid3rdOrder(X, C, b, r);

   float mag = magnitude2(C);

   if(H) {
      float X3_2 = 2*X[3];
      float M = X3_2*X[3];
      float b2[3];

      int i;
      for(i=0; i<3; i++) {
         C[i] *= M;
         b2[i] = b[i]*b[i];
      }
      float t[3] = {X[7]*C[2] + X[6]*C[1] + C[0],
		    X[8]*C[2] + X[4]*C[1], X[5]*C[2]};

      H[0] =t[0]*(-3*b2[0]*X[10]-2*b[0]*X[9]-1);
      H[1] =t[1]*(-3*b2[1]*X[12]-2*b[1]*X[11]-1);
      H[2] =t[2]*(-3*b2[2]*X[14]-2*b[2]*X[13]-1);
      H[3] =X3_2*mag;
      H[4] =r[1]*C[1];
      H[5] =r[2]*C[2];
      H[6] =r[0]*C[1];
      H[7] =r[0]*C[2];
      H[8] =r[1]*C[2];
      H[9] =b2[0]*t[0];
      H[10]=b[0]*H[9];
      H[11]=b2[1]*t[1];
      H[12]=b[1]*H[11];
      H[13]=b2[2]*t[2];
      H[14]=b[2]*H[13];
   }

   Z[0] = 1 - X[3]*X[3]*mag;
}

/*
   for the transformation matrix:
   m0*m0 + m1*m1 + m2*m2 = 1
   g.m/magnitude=dip

   b = <x[0] - X[0], x[1] - X[1], x[2] - X[2]>

   m0 = (X[3]*(     b[0] + X[6]*b[1] +  X[9]*b[2]))
   m1 = (X[3]*(X[4]*b[1] + X[7]*b[0] + X[10]*b[2]))
   m2 = (X[3]*(X[5]*b[2] + X[8]*b[0] + X[11]*b[1]))

   13 unknowns needed

   take data in x, and apply the calibration state in X for the rotated
   ellipsoid with 3rd order coefficients.  If b is not null, then store
   just the bias updates there.
*/

void ApplyTransformationMatrix(const float X[13], float x[3], float *b)
{
   float B[3];
   if(!b)
      b = B;

   vector_sub(b, x, X, 3);
   x[0] =      b[0] + X[6]*b[1] +  X[9]*b[2];
   x[1] = X[4]*b[1] + X[7]*b[0] + X[10]*b[2];
   x[2] = X[5]*b[2] + X[8]*b[0] + X[11]*b[1];
}

/* the rows for magnitude=1 in the transformation matrix

    (X[3]*(     (x[0]-X[0]) + X[6]*(x[1]-X[1]) +  X[9]*(x[2]-X[2])))^2
   +(X[3]*(X[4]*(x[1]-X[1]) + X[7]*(x[0]-X[0]) + X[10]*(x[2]-X[2])))^2
   +(X[3]*(X[5]*(x[2]-X[2]) + X[8]*(x[0]-X[0]) + X[11]*(x[1]-X[1])))^2
*/
void SetupFitTransformationMatrix_m(float *H, float *Z, const float X[13],
                                    const float x[3])
{
   float b[3], C[3] = {x[0], x[1], x[2]};
   ApplyTransformationMatrix(X, C, b);

   float mag = magnitude2(C);

   if(H) {
     float X3_2 = 2*X[3];
     float M = X3_2*X[3];

      int i;
      for(i=0; i<3; i++)
         C[i] *= M;

     H[0] = -  X[8]*C[2] -  X[7]*C[1] -      C[0];
     H[1] = - X[11]*C[2] -  X[4]*C[1] - X[6]*C[0];
     H[2] = -  X[5]*C[2] - X[10]*C[1] - X[9]*C[0];
     H[3] = X3_2*mag;
     H[4] = b[1]*C[1];
     H[5] = b[2]*C[2];
     H[6] = b[1]*C[0];
     H[7] = b[0]*C[1];
     H[8] = b[0]*C[2];
     H[9] = b[2]*C[0];
     H[10]= b[2]*C[1];
     H[11]= b[1]*C[2];
     H[12]= 0;
   }

   Z[0] = 1 - X[3]*X[3]*mag;
}

/* the rows for g.m/magnitude=dip in the transformation matrix

   ((X[3]*(     (x[0]-X[0]) + X[6]*(x[1]-X[1]) +  X[9]*(x[2]-X[2])))*g[0]
   +(X[3]*(X[4]*(x[1]-X[1]) + X[7]*(x[0]-X[0]) + X[10]*(x[2]-X[2])))*g[1]
   +(X[3]*(X[5]*(x[2]-X[2]) + X[8]*(x[0]-X[0]) + X[11]*(x[1]-X[1])))*g[2])
   / sqrt((X[3]*(     (x[0]-X[0]) + X[6]*(x[1]-X[1]) +  X[9]*(x[2]-X[2])))^2
   + (X[3]*(X[4]*(x[1]-X[1]) + X[7]*(x[0]-X[0]) + X[10]*(x[2]-X[2])))^2
   + (X[3]*(X[5]*(x[2]-X[2]) + X[8]*(x[0]-X[0]) + X[11]*(x[1]-X[1])))^2)
   - X[12]

 */
void SetupFitTransformationMatrix_g(float *H, float *Z, const float X[13],
                                    const float x[3], const float g[3])
{
   float b[3], C[3] = {x[0], x[1], x[2]};
   ApplyTransformationMatrix(X, C, b);

   float mag = magnitude2(C);
   float smag = sqrt(mag);
   float D = dot(g, C) / smag;
   if(H) {
     int i;
     for(i=0; i<3; i++)
       C[i] *= D/mag;

     float h[3];
     for(i=0; i<3; i++)
       h[i] = g[i] / smag - C[i];

     H[0] = - X[8]*h[2] - X[7]*h[1] -      h[0];
     H[1] = -X[11]*h[2] - X[4]*h[1] - X[6]*h[0];
     H[2] = - X[5]*h[2] -X[10]*h[1] - X[9]*h[0];
     H[3] = 0;
     H[4] = b[1]*h[1];
     H[5] = b[2]*h[2];
     H[6] = b[1]*h[0];
     H[7] = b[0]*h[1];
     H[8] = b[0]*h[2];
     H[9] = b[2]*h[0];
     H[10]= b[2]*h[1];
     H[11]= b[1]*h[2];
     H[12]= -1;
   }

   Z[0] = X[12] - D;
}

/*
  g . q*m*q' = d

  q*m = {q0*m0 - q1*m1 - q2*m2 - q3*m3,
         q0*m1 + q1*m0 + q2*m3 - q3*m2,
         q0*m2 - q1*m3 + q2*m0 + q3*m1,
         q0*m3 + q1*m2 - q2*m1 + q3*m0}

  q*m*q' = {
  +(q0*m0 - q1*m1 - q2*m2 - q3*m3) * q0
  +(q0*m1 + q1*m0 + q2*m3 - q3*m2) * q1
  +(q0*m2 - q1*m3 + q2*m0 + q3*m1) * q2
  +(q0*m3 + q1*m2 - q2*m1 + q3*m0) * q3

  -(q0*m0 - q1*m1 - q2*m2 - q3*m3) * q1
  +(q0*m1 + q1*m0 + q2*m3 - q3*m2) * q0
  -(q0*m2 - q1*m3 + q2*m0 + q3*m1) * q3
  +(q0*m3 + q1*m2 - q2*m1 + q3*m0) * q2

  -(q0*m0 - q1*m1 - q2*m2 - q3*m3) * q2
  +(q0*m1 + q1*m0 + q2*m3 - q3*m2) * q3
  +(q0*m2 - q1*m3 + q2*m0 + q3*m1) * q0
  -(q0*m3 + q1*m2 - q2*m1 + q3*m0) * q1

  -(q0*m0 - q1*m1 - q2*m2 - q3*m3) * q3
  -(q0*m1 + q1*m0 + q2*m3 - q3*m2) * q2
  +(q0*m2 - q1*m3 + q2*m0 + q3*m1) * q1
  +(q0*m3 + q1*m2 - q2*m1 + q3*m0) * q0

  the truth equation:
  (m0*g[0] + m1*g[1] + m2*g[2])/sqrt(m0*m0 + m1*m1 + m2*m2) = dip;

   expanded from above:
  (((q[1]*m0 + q[2]*m1 + q[3]*m2) * q[1]
   +(q[0]*m0 + q[2]*m2 - q[3]*m1) * q[0]
   -(q[0]*m1 - q[1]*m2 + q[3]*m0) * q[3]
   +(q[0]*m2 + q[1]*m1 - q[2]*m0) * q[2]) * g[0]
  +((q[1]*m0 + q[2]*m1 + q[3]*m2) * q[2]
   +(q[0]*m0 + q[2]*m2 - q[3]*m1) * q[3]
   +(q[0]*m1 - q[1]*m2 + q[3]*m0) * q[0]
   -(q[0]*m2 + q[1]*m1 - q[2]*m0) * q[1]) * g[1]
  +((q[1]*m0 + q[2]*m1 + q[3]*m2) * q[3]
   -(q[0]*m0 + q[2]*m2 - q[3]*m1) * q[2]
   +(q[0]*m1 - q[1]*m2 + q[3]*m0) * q[1]
   +(q[0]*m2 + q[1]*m1 - q[2]*m0) * q[0]) * g[2])
   / sqrt(m0*m0 + m1*m1 + m2*m2)
   = dip

   fill in for m0, m1, m2...

   m0 = (X[0]*((x[0]-X[6])))
   m1 = (X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))
   m2 = (X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))

  (((q[1]*(X[0]*((x[0]-X[6]))) + q[2]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))
  + q[3]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))) * q[1]
  +(q[0]*(X[0]*((x[0]-X[6]))) + q[2]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6])
  + X[5]*(x[1]-X[7]))) - q[3]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))) * q[0]
  -(q[0]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6]))) - q[1]*(X[0]*((x[2]-X[8])*X[2]
  + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7]))) + q[3]*(X[0]*((x[0]-X[6])))) * q[3]
  +(q[0]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))
  + q[1]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6]))) - q[2]*(X[0]*((x[0]-X[6])))) * q[2]) * g[0]

  +((q[1]*(X[0]*((x[0]-X[6]))) + q[2]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))
  + q[3]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))) * q[2]
  +(q[0]*(X[0]*((x[0]-X[6]))) + q[2]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6])
  + X[5]*(x[1]-X[7]))) - q[3]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))) * q[3]
  +(q[0]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6]))) - q[1]*(X[0]*((x[2]-X[8])*X[2]
  + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7]))) + q[3]*(X[0]*((x[0]-X[6])))) * q[0]
  -(q[0]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))
  + q[1]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6]))) - q[2]*(X[0]*((x[0]-X[6])))) * q[1]) * g[1]

  +((q[1]*(X[0]*((x[0]-X[6]))) + q[2]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))
  + q[3]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))) * q[3]
  -(q[0]*(X[0]*((x[0]-X[6]))) + q[2]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6])
  + X[5]*(x[1]-X[7]))) - q[3]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))) * q[2]
  +(q[0]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6]))) - q[1]*(X[0]*((x[2]-X[8])*X[2]
  + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7]))) + q[3]*(X[0]*((x[0]-X[6])))) * q[1]
  +(q[0]*(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))
  + q[1]*(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6]))) - q[2]*(X[0]*((x[0]-X[6])))) * q[0]) * g[2])

  / sqrt((X[0]*((x[0]-X[6])))*(X[0]*((x[0]-X[6])))
  + (X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))
  *(X[0]*((x[1]-X[7])*X[1] + X[3]*(x[0]-X[6])))
  + (X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7])))
  *(X[0]*((x[2]-X[8])*X[2] + X[4]*(x[0]-X[6]) + X[5]*(x[1]-X[7]))))
   = dip

*/

void ApplyQuaternion(float *x, const float X[3], float *q)
{
   float Q[4];
   if(!q)
      q = Q;
   q[0] = sqrt(1 - magnitude2(X));
   q[1] = X[0];
   q[2] = X[1];
   q[3] = X[2];
   if(x)
      rotvecquat(x, q);
}

/* just fill in the coefficients for quaternion rotation */
void SetupFitQuaternion(float *H, float Z[1], const float X[4],
                        const float g[3], const float m[3])
{
   float p[3] = {m[0], m[1], m[2]}, q[4];
   ApplyQuaternion(p, X, q);

   float mag = 1/magnitude(m);

   if(H) {
      float a = -m[0]*q[2]+m[1]*q[1]+m[2]*q[0];
      float b =  m[0]*q[3]+m[1]*q[0]-m[2]*q[1];
      float c =  m[0]*q[0]-m[1]*q[3]+m[2]*q[2];
      float d =  m[0]*q[1]+m[1]*q[2]+m[2]*q[3];

      H[0] = 2 * ( d*g[0]-a*g[1]+b*g[2]) * mag;
      H[1] = 2 * ( a*g[0]+d*g[1]-c*g[2]) * mag;
      H[2] = 2 * (-b*g[0]+c*g[1]+d*g[2]) * mag;
      H[3] = -1;
   }

   Z[0] = X[3] - dot(p, g) * mag;
}

float CalcDeviation(const float R[], int n)
{
   float t = 0;
   int i;
   for(i=0; i<n; i++)
      t += R[i]*R[i];
   return sqrt(t/n);
}

float CalcDeviationCallback(const float X[],
			    int (*makerow)(float *H, float *Z,
                                     const float *X, int i), int n)
{
   float t = 0;
   int i, num = 0;
   for(i = 0; i<n; i++) {
      float R;
      if(makerow(0, &R, X, i)) {
	 t += R*R;
	 num++;
      }
   }
   return  sqrt(t / num);
}

// calculates X += (JtJ)^-1*JtRt
int _ApplyLeastSquares(float X[], const float *pJ, const float R[], int m, int n)
{
   float (*J)[m] = (float(*)[m])pJ;
   int i, j, k;

   // Compute N=(JtRt)t, mxn*nx1 get mx1
   float N[m];
   for(i = 0; i<m; i++) {
      N[i] = 0;
      for(j = 0; j<n; j++)
         N[i] += J[j][i]*R[j];
   }

   // Compute S, S = JtJ, multiply mxn matrix with nxm and get mxm
   float S[m][m];
   for(i = 0; i<m; i++)
      for(j = 0; j<m; j++) {
         S[i][j] = 0;
         for(k = 0; k<n; k++)
            S[i][j] += J[k][i]*J[k][j];
      }

   // Invert S
   if(matrix_invert(S, m))
      return 1;

   // Compute X+=(S^-1*Nt)t, multiply mxm matrix with mx1 and get mx1
   for(i = 0; i<m; i++)
      for(j = 0; j<m; j++)
         X[i] += S[i][j]*N[j];

   return 0;
}

/* Use a callback to compute partials so that we don't need
   the ram required to store the jacobian matrix.  */
int ApplyLeastSquaresCallback(float X[], int (*makerow)(float *H, float *Z,
				 const float *X, int i), int m, int n)
{
   float H[m], R, S[m][m], N[m];
   int i, p, q, num;

   num = 0;
   /* clear out S and N */
   memset(S, 0, sizeof S);
   memset(N, 0, sizeof N);

   for(i = 0; i<n; i++)
      if(makerow(H, &R, X, i)) {
         for(p = 0; p<m; p++) {
            for(q = 0; q<m; q++)
               S[p][q] += H[p]*H[q];
            N[p] += H[p]*R;
         }
         num++;
      }

   // not enough points
//   if(num < m)
//      return 1;
   
   // Invert S
   if(matrix_invert(S, m))
      return 1;

   // Compute X+=(S^-1*Nt)t, multiply mxm matrix with mx1 and get mx1
   for(p = 0; p<m; p++)
      for(q = 0; q<m; q++)
	 X[p] += S[p][q]*N[q];

   return 0;
}

/* store the alignment rotation to alignment for points
   which are points around an axis */
float AlignAxis(float alignment[4], float (*points)[3], int count)
{
   float v[3] = {0}, R[count];
   int i;
   for(i = 0; i<count; i++)
      R[i] = 1;
   ApplyLeastSquares(v, points, R, 3, count);

   /* force sign since we assume that the laser is nominally aligned
      with the x axis */
   float x[3] = {1, 0, 0};
   if(v[0] < 0)
      vector_scale(-1, x, 3);
   
   /* compute quaternion rotation to align into laser coordinates */
   vec2vec2quat(alignment, v, x);

   for(i = 0; i<count; i++)
      R[i] = (1 - dot(v, points[i])) / (magnitude(v) * magnitude(points[i]));

   return CalcDeviation(R, count);
}

/* below are experimental functions */

/* Calculate an alignment axis and biases,
   allows computing laser alignment with multiple axes
   x + py + qz - v_n = 0
 */
float AlignAxes(float alignment[4], float bias[3],
                float (*points)[3], int count,
                int *axis, int axiscount)
{
  const int basestates = 2;
  float x[basestates+axiscount], J[count][basestates+axiscount], R[count];
  int i, j;

  for(j = 0; j<basestates+axiscount; j++)
    x[j] = 0;

  int l;
  for(l=0; l<1; l++) {
  for(i = 0; i<count; i++) {
    J[i][0] = points[i][1];
    J[i][1] = points[i][2];

    for(j = 0; j<axiscount; j++)
      J[i][j+basestates] = (j == axis[i]);

    R[i] = -((points[i][0]) +
             (x[0]*points[i][1]) +
             (x[1]*points[i][2]) +
             x[basestates+axis[i]]);
  }

#if 0
    for(i = 0; i<count; i++) {
      for(j = 0; j<basestates+axiscount; j++)
        printf("%.3f ", J[i][j]);
      printf(": %.3f\n", R[i]);
    }
#endif

    ApplyLeastSquares(x, J, R, basestates+axiscount, count);

#if 1
    for(i = 0; i<basestates+axiscount; i++)
      printf("%f ", x[i]);
    printf("\n");
#endif

#if 1
    float total = 0;
    for(i = 0; i<count; i++) {
      float val = points[i][0] + points[i][1]*x[0] + points[i][2]*x[1]
        + x[basestates+axis[i]];
      printf("%f ", val);
      total += val*val;
    }
    printf("\nstd: %f\n", sqrt(total/count));
#endif
  }

  /* warning, not done! */

   return 0;
#if 0
   /* force sign since we assume that the laser is nominally aligned
      with the x axis */
   float x[3] = {1, 0, 0};
   if(v[0] < 0)
      vector_scale(-1, x, 3);
   
   /* compute quaternion rotation to align into laser coordinates */
   vec2vec2quat(alignment, v, x);

   for(i = 0; i<count; i++)
      R[i] = (1 - dot(v, points[i])) / (magnitude(v) * magnitude(points[i]));

   return CalcStdDeviation(R, count);
#endif
}

/*
   (q1*x0 + q2*x1 + q3*x2) * q1
  +(q0*x0 + q2*x2 - q3*x1) * q0
  -(q0*x1 - q1*x2 + q3*x0) * q3
  +(q0*x2 + q1*x1 - q2*x0) * q2 = d_n

   partials:
   2*q3*x2+2*q2*x1+2*q1*x0
   2*q0*x2+2*q1*x1-2*q2*x0
   2*q2*x2-2*q3*x1+2*q0*x0

   with biases

  (((q1*(x0-b0) + q2*(x1-b1) + q3*(x2-b2)) * q1
   +(q0*(x0-b0) + q2*(x2-b2) - q3*(x1-b1)) * q0
   -(q0*(x1-b1) - q1*(x2-b2) + q3*(x0-b0)) * q3
   +(q0*(x2-b2) + q1*(x1-b1) - q2*(x0-b0)) * q2) = d_n

 */
float QuaternionAlignAxes(float alignment[4], float (*points)[3], int count,
                          int *axis, int axiscount)
{
  const int basestates = 2;
  float x[basestates+axiscount], J[count][basestates+axiscount], R[count];
  int i, j;

  for(j = 0; j<basestates+axiscount; j++)
    x[j] = 0;

#if 0
  x[0] = -0.017805;
  x[1] = -0.012116;
  x[4] = 0.080371;
  x[5] = 0.775368;
  x[6] = -0.749213;
#endif

#if 0
  x[2] = 0.080371;
  x[3] = 0.775368;
  x[4] = -0.749213;
#endif

  int l;
  for(l=0; l<2; l++) {
    float total = 0;
  for(i = 0; i<count; i++) {
    //normalize(points[i]);
    float x0 = points[i][0], x1 = points[i][1], x2 = points[i][2];

    float q1 = 0, q2 = x[0], q3 = x[1];
    //float q1 = 0, q2 = -0.017805, q3 = -0.012116;

    float q0 = sqrt(1 - q2*q2 - q3*q3);

//    float b0 = 0, b1 = x[2], b2 = x[3];
//    float b0 = 0, b1 = x[0], b2 = x[1];
    float b0 = 0, b1 = 0, b2 = 0;
#if 1
    J[i][0] = 2*q0*x2+2*q1*x1-2*q2*x0;
    J[i][1] = 2*q1*x2-2*q0*x1-2*q3*x0;
#else
    J[i][0] = 2*q0*(x2-b2)+2*q1*(x1-b1)-q2*(x0-b0)+q2*(b0-x0);
    J[i][1] = 2*q1*(x2-b2)-q0*(x1-b1)+q0*(b1-x1)-q3*(x0-b0)+q3*(b0-x0);
//    J[i][0] =( 2*q0*q3-2*q1*q2);
//    J[i][1] =(-2*q1*q3-2*q0*q2);
#endif

    for(j = 0; j<axiscount; j++)
      J[i][j+basestates] = j == axis[i];

    float q[4] = {q0, q1, q2, q3}, xs[3] = {x0-b0, x1-b1, x2-b2};
    //  normalize(xs);
    rotvecquat(xs, q);

    R[i] = -x[basestates+axis[i]] -xs[0];
    total += R[i]*R[i];

  }

#if 0
  for(i = 0; i<count; i++) {
    for(j = 0; j<basestates+axiscount; j++)
      printf("%.3f ", J[i][j]);
    printf(": %.3f\n", R[i]);
  }
#endif

  if(ApplyLeastSquares(x, J, R, basestates+axiscount, count))
    printf("matrix invert failed\n");

#if 0
  for(i = 0; i<basestates+axiscount; i++)
    printf("%f ", x[i]);
  printf("\n");
#endif

  alignment[0] = sqrt(1-x[0]*x[0]-x[1]*x[1]);
  alignment[1] = 0;
  alignment[2] = x[0];
  alignment[3] = x[1];

  if(l) {
    printf("<%f %f %f %f> %f degrees  std: %f\n",
           alignment[0], alignment[1], alignment[2], alignment[3],
           rad2deg(quatangle(alignment)), sqrt(total/count));
  }
  }

   return 0;
}

/*

  (((q1*(x0-b0) + q2*(x1-b1) + q3*(x2-b2)) * q1
   +(q0*(x0-b0) + q2*(x2-b2) - q3*(x1-b1)) * q0
   -(q0*(x1-b1) - q1*(x2-b2) + q3*(x0-b0)) * q3
   +(q0*(x2-b2) + q1*(x1-b1) - q2*(x0-b0)) * q2) = d_n

 */
float QuaternionRebias(float bias[3], float (*points)[3], int count,
                       int *axis, int axiscount)
{
  int i, j, k;

  float minb[3] = {0, 0, 0}, maxb[3] = {.1, .1, .1}, stepb = .005;
  float b[3], mindev = 1;
  for(b[0] = minb[0]; b[0] <= maxb[0]; b[0] += stepb) {
    printf("step: %.3f  best: <%g %g %g>\n", b[0], bias[0], bias[1], bias[2]);
    for(b[1] = minb[1]; b[1] <= maxb[1]; b[1] += stepb)
      for(b[2] = minb[2]; b[2] <= maxb[2]; b[2] += stepb) {
        float total = 0;
        for(i = 0; i<axiscount; i++) {
          int axisstart = -1, axisend = -1;
          for(j = 0; j<count; j++) {
            if(axis[j] == i)
              if(axisstart == -1)
                axisstart = j;
            if(axis[j] > i) {
              axisend = j;
              break;
            }
          }
          if(axisend == -1)
            axisend = j;
          
          int num = axisend - axisstart;
          float p[num][3];
          float q1[4], q2[4];
          AlignAxis(q1, points + axisstart, num);

          for(j=0; j<num; j++)
            for(k=0; k<3; k++)
              p[j][k] = points[j+axisstart][k] + b[k];

          float dev = AlignAxis(q2, p, num);
//          printf("dev: <%g %g %g> %g %g\n",
//                 b[0], b[1], b[2],
//                 q1[2]- q2[2], q1[3] - q2[3]);

          total += dev*dev;
        }
        total = sqrt(total / axiscount);
        if(total < mindev) {
          mindev = total;
          for(k=0; k<3; k++)
            bias[k] = b[k];
        }
      }
  }
  return mindev;
}

/* below are older functions which require a set of "stillpoints"
*/
void InitialEstimateSphere(float (*points)[3], int count, float bias[3], float scale[3])
{
   int i, j;
   /* set bias to the average */
   for(i = 0; i<3; i++) {
      bias[i] = 0;
      for(j = 0; j<count; j++) {
         float val = points[j][i];
         bias[i] += val;
      }
      bias[i] /= count;
   }   

   /* set scale to distance first point is from bias */
   float p[3] = {points[0][0] - bias[0],
                 points[0][1] - bias[1],
                 points[0][2] - bias[2]};
   scale[0] = scale[1] = scale[2] = magnitude(p);
}

/* calculate best fit for a sphere
   (x-a)^2 + (y-b)^2 + (z-c)^2 = r^2
*/
int CalcBestFitSphere(float (*points)[3], int count, float bias[3],
                      float scale[3])
{
    float J[count][4];
    float R[count];
    float X[4] = {bias[0], bias[1], bias[2], scale[0]};

    int i;
    for(i = 0; i<count; i++) {
        float dx = X[0] - points[i][0], dx2 = dx*dx;
        float dy = X[1] - points[i][1], dy2 = dy*dy;
        float dz = X[2] - points[i][2], dz2 = dz*dz;
        float r = sqrt(dx2 + dy2 + dz2);
        float rinv = 1/r;

        J[i][0] = dx * rinv;
        J[i][1] = dy * rinv;
        J[i][2] = dz * rinv;
        J[i][3] = -1;

        R[i] = X[3] - r;
    }

    if(ApplyLeastSquares(X, J, R, 4, count))
       return 0;

    if(X[3] < 0 || isnan(X[0]))
        return 0;

    for(i = 0; i<3; i++)
       bias[i] = X[i];
    scale[0] = scale[1] = scale[2] = X[3];

    return 1;
}

void InitialEstimateEllipse(float (*points)[3], int count, float bias[3],
                            float scale[3])
{
   /* set scale to max-min and bias to the average */
   int i, j;
   for(i = 0; i<3; i++) {
      bias[i] = 0;
      float min = 1/0.0, max = -1/0.0;
      for(j = 0; j<count; j++) {
         float val = points[j][i];
         if(min > val)
            min = val;
         if(max < val)
            max = val;
         bias[i] += val;
      }
      
      scale[i] = (max - min) / 2;
      bias[i] /= count;
   }
}

/* calculate the best fit 3d ellipse
   a*(x-d)^2 + b*(y-e)^2 + c*(z-f)^2 = 1
*/
int CalcBestFitEllipse(float (*points)[3], int count, float bias[3],
                       float scale[3])
{
    float J[count][6];
    float R[count];

    float X[6] = {1/scale[0]/scale[0],
                  1/scale[1]/scale[1],
                  1/scale[2]/scale[2],
                  bias[0],
                  bias[1],
                  bias[2]};

    int i;
    for(i = 0; i<count; i++) {
        float dx = points[i][0] - X[3], dx2 = dx*dx;
        float dy = points[i][1] - X[4], dy2 = dy*dy;
        float dz = points[i][2] - X[5], dz2 = dz*dz;

        J[i][0] = dx2;
        J[i][1] = dy2;
        J[i][2] = dz2;

        J[i][3] = -2*X[0]*dx;
        J[i][4] = -2*X[1]*dy;
        J[i][5] = -2*X[2]*dz;

        R[i] = 1 - X[0]*dx2 - X[1]*dy2 - X[2]*dz2;
    }

    if(ApplyLeastSquares(X, J, R, 6, count))
       return 0;

    if(X[0] < 0 || X[1] < 0 || X[2] < 0 || isnan(X[0]))
       return 0;

    // Convert
    for(i = 0; i<3; i++) {
       scale[i] = 1/sqrt(X[i]);
       bias[i] = X[i+3];
    }

    return 1;
}

/* calculate the best fit 3d rotated ellipse
   (a*(x-g))^2 + (b*(y-h) + d*(x-g))^2 + (c*(z-i) + e*(x-g) + f*(y-h))^2 = 1
*/
int CalcBestFitRotatedEllipse(float (*points)[3], int count, float bias[3],
                              float alignment[3], float scale[3])
{
   float J[count][9];
   float R[count];
   
   float X[9] = {1/scale[0], 1/scale[1], 1/scale[2],
#if 1
                 alignment[0] / scale[1],
                 alignment[1] / scale[2],
                 alignment[2] / scale[2],
#else
                 alignment[0],
                 alignment[1],
                 alignment[2],
#endif
                 bias[0], bias[1], bias[2]};

   int i;
   for(i = 0; i<count; i++) {
      float dx = X[6] - points[i][0], dx2 = dx*dx;
      float dy = X[7] - points[i][1], dy2 = dy*dy;
      float dz = X[8] - points[i][2], dz2 = dz*dz;
      
#if 0
      J[i][0] = X[0]*dx2;
      J[i][1] = dy*(X[1]*dy + X[3]*dx);
      J[i][2] = dz*(X[2]*dz + X[4]*dx + X[5]*dy);
      
      J[i][3] = dx*(X[1]*dy + X[3]*dx);
      J[i][4] = dx*(X[2]*dz + X[4]*dx + X[5]*dy);
      J[i][5] = dy*(X[2]*dz + X[4]*dx + X[5]*dy);

      J[i][6] = X[4]*(X[2]*dz + X[5]*dy + X[4]*dx)
         + X[3]*(X[1]*dy + X[3]*dx) + X[0]*X[0]*dx;
      J[i][7] = X[5]*(X[2]*dz + X[5]*dy + X[4]*dx)
         + 2*X[1]*(X[1]*dy + X[3]*dx);
      J[i][8] = +X[2]*(X[2]*dz + X[5]*dy + X[4]*dx);
#else
      float f = (X[1]*dy + X[3]*dx);
      float g = (X[2]*dz + X[4]*dx + X[5]*dy);

      J[i][0] = X[0]*dx2;
      J[i][1] = dy*f;
      J[i][2] = dz*g;
      
      J[i][3] = dx*f;
      J[i][4] = dx*g;
      J[i][5] = dy*g;
      
      J[i][6] = X[4]*g + X[3]*f + X[0]*X[0]*dx;
      J[i][7] = X[5]*g + 2*X[1]*f;
      J[i][8] = X[2]*g;
#endif

      //   (a*(x-g))^2 + (b*(y-h) + d*(x-g))^2 + (c*(z-i) + e*(x-g) + f*(y-h))^2 = 1
      float x = X[0]*dx;
      float y = X[1]*dy + X[3]*dx;
      float z = X[2]*dz + X[4]*dx + X[5]*dy;
      R[i] = 1 - sqrt(x*x + y*y + z*z);
   }
   
   if(ApplyLeastSquares(X, J, R, 9, count))
      return 0;
   
   if(X[0] < 0 || X[1] < 0 || X[2] < 0 || isnan(X[0]))
      return 0;
   
   // Convert
   for(i = 0; i<3; i++) {
      scale[i] = 1 / X[i];
      bias[i] = X[i+6];
   }

#if 1
   alignment[0] = X[3] * scale[1];
   alignment[1] = X[4] * scale[2];
   alignment[2] = X[5] * scale[2];
#else
   alignment[0] = X[3];
   alignment[1] = X[4];
   alignment[2] = X[5];
#endif
   
   return 1;
}

void ApplyBestFit(float point[3], float n[3], float bias[3],
                  float scale[3], float *alignment)
{
   float d[3] = {point[0] - bias[0], point[1] - bias[1], point[2] - bias[2]};

   if(alignment) {
      d[2] += alignment[1] * d[0] + alignment[2] * d[1];
      d[1] += alignment[0] * d[0];
   }

   n[0] = d[0] / scale[0];
   n[1] = d[1] / scale[1];
   n[2] = d[2] / scale[2];
}

float CalcEllipseDeviation(float point[3], float bias[3], float scale[3], float *alignment)
{
   float n[3];
   ApplyBestFit(point, n, bias, scale, alignment);
   return fabs(1 - magnitude(n));
}

float CalcEllipseAvgDeviation(float (*points)[3], int count, float bias[3],
                       float scale[3], float *alignment)
{
    float devt = 0, dev;
    int i;
    for(i = 0; i<count; i++) {
       dev = CalcEllipseDeviation(points[i], bias, scale, alignment);
       devt += dev*dev;
    }

    devt /= count;
    return sqrt(devt);
}

/* calculate the best fit plane to points, the normal goes in
   N.  The plane is A*x + B*y + C*z = 1
*/
int CalcBestFitPlane(float (*points)[3], int count, float N[3])
{
    float R[count];

    N[0] = N[1] = N[2] = 0;

    int i;
    for(i = 0; i<count; i++)
        R[i] = 1;

    if(ApplyLeastSquares(N, points, R, 3, count))
       return 0;

    return 1;
}

float CalcDeviationPlane(float point[3], float norm[3])
{
   return (1 - fabs(dot(point, norm)))
      / sqrt(magnitude2(point) * magnitude(norm));
}

float CalcAvgDeviationPlane(float (*points)[3], int count, float norm[3])
{
   float devt = 0;
   int i;
   for(i = 0; i<count; i++) {
      float dev = CalcDeviationPlane(points[i], norm);
      devt += dev*dev;
   }

   devt /= count;
   return sqrt(devt);
}

/*
  r . p*q*p' = 1 or -1 (north or south hemisphere)

+r0*(2*p1*p3*q3+2*p0*p2*q3-2*p0*p3*q2+2*p1*p2*q2-p3^2*q1-p2^2*q1+p1^2*q1+p0^2*q1)
+r1*(2*p2*p3*q3-2*p0*p1*q3-p3^2*q2+p2^2*q2-p1^2*q2+p0^2*q2+2*p0*p3*q1+2*p1*p2*q1)
+r2*(p3^2*q3-p2^2*q3-p1^2*q3+p0^2*q3+2*p2*p3*q2+2*p0*p1*q2+2*p1*p3*q1-2*p0*p2*q1)
 */

float CalcDeviationQuaternion(float apoint[3], float mpoint[3], float Q[4],
                              int southhemisphere)
{
   float hemi = southhemisphere ? -1 : 1;
   float p[3] = {mpoint[0], mpoint[1], mpoint[2]};
   rotvecquat(p, Q);
   return hemi - dot(p, apoint);
}

int CalcBestFitQuaternion(float (*apoints)[3], float (*mpoints)[3], int count,
                          float Q[4], int southhemisphere)
{
    float J[count][4];
    float R[count];

    int i;
    for(i = 0; i<count; i++) {
       float p0 = Q[0], p1 = Q[1], p2 = Q[2], p3 = Q[3];
       float q0 = 0, q1 = mpoints[i][0], q2 = mpoints[i][1], q3 = mpoints[i][2];
       float r0 = apoints[i][0], r1 = apoints[i][1], r2 = apoints[i][2];

#if 0
       J[i][0] = 2*((+p0*q3+p1*q2-p2*q1)*r2+(-p1*q3+p0*q2+p3*q1)*r1+(p2*q3-p3*q2+p0*q1)*r0);
       J[i][1] = 2*((-p1*q3+p0*q2+p3*q1)*r2+(-p0*q3-p1*q2+p2*q1)*r1+(p3*q3+p2*q2+p1*q1)*r0);
       J[i][2] = 2*((-p2*q3+p3*q2-p0*q1)*r2+(+p3*q3+p2*q2+p1*q1)*r1+(p0*q3+p1*q2-p2*q1)*r0);
       J[i][3] = 2*((+p3*q3+p2*q2+p1*q1)*r2+(+p2*q3-p3*q2+p0*q1)*r1+(p1*q3-p0*q2-p3*q1)*r0);
#else
       float a =  p0*q3+p1*q2-p2*q1;
       float b = -p1*q3+p0*q2+p3*q1;
       float c =  p2*q3-p3*q2+p0*q1;
       float d =  p3*q3+p2*q2+p1*q1;
       J[i][0] = 2*( a*r2+b*r1+c*r0);
       J[i][1] = 2*( b*r2-a*r1+d*r0);
       J[i][2] = 2*(-c*r2+d*r1+a*r0);
       J[i][3] = 2*( d*r2+c*r1-b*r0);
#endif
       R[i] = CalcDeviationQuaternion(apoints[i], mpoints[i], Q, southhemisphere);
    }

    if(ApplyLeastSquares(Q, J, R, 4, count))
       return 0;

    if(isnan(Q[0]))
        return 0;

    return 1;
}

float CalcAvgDeviationQuaternion(float (*apoints)[3], float (*mpoints)[3], 
                                 int count, float Q[4],
                                 int southhemisphere)
{
    float devt = 0, dev;
    int i;
    for(i = 0; i<count; i++) {
       dev = CalcDeviationQuaternion(apoints[i], mpoints[i], Q, southhemisphere);
       devt += dev*dev;
    }

    devt /= count;
    return sqrt(devt);
}
