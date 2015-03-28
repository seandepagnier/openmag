/* Copyright (C) 2007, 2009 Sean D'Epagnier <sean@depagnier.com>
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

/* recursive algorithms */
#define ApplyRecursiveLeastSquares(P, H, R, Z, X, m, n) \
  _ApplyRecursiveLeastSquares((float *)(P), (const float *)(H), \
                              (const float *)(R), Z, X, m, n)
int _ApplyRecursiveLeastSquares(float *P, const float *H, const float *R,
                                const float Z[], float X[], int m, int n);

void SetupFitSphere(float H[4], float Z[1], const float X[4],
                    const float x[3]);
void SetupFitThermalSphere(float H[7], float Z[1], const float X[7],
                           const float x[3], float t);
void ApplyRotatedEllipsoid(const float X[9], float x[3], float *b);
void SetupFitRotatedEllipsoid(float *H, float Z[1], const float X[9],
                              const float x[3]);
void ApplyRotatedEllipsoid3rdOrder(const float X[15], float x[3], float *b, float *r);
void SetupFitRotatedEllipsoid3rdOrder(float *H, float Z[1], const float X[15],
                                      const float x[3]);
void ApplyTransformationMatrix(const float X[13], float x[3], float *b);
void SetupFitTransformationMatrix_m(float *H, float *Z, const float X[13],
                                    const float x[3]);
void SetupFitTransformationMatrix_g(float *H, float *Z, const float X[13],
                                    const float x[3], const float g[3]);

void ApplyQuaternion(float *x, const float X[3], float *q);
void SetupFitQuaternion(float H[4], float Z[1], const float X[4],
                        const float g[3], const float m[3]);
float CalcDeviation(const float R[], int n);
float CalcDeviationCallback(const float X[], int (*makerow)(float *H, float *Z,
					   const float *X, int i), int n);

#define ApplyLeastSquares(X, J, R, m, n) \
  _ApplyLeastSquares(X, (const float *)(J), R, m, n)
int _ApplyLeastSquares(float X[], const float *j, const float R[], int m, int n);
int ApplyLeastSquaresCallback(float X[], int (*makerow)(float *H, float *Z,
                                        const float *X, int i), int m, int n);

float AlignAxis(float alignment[4], float (*points)[3], int count);
float AlignAxes(float alignment[4], float bias[3],
                float (*points)[3], int count,
                int *axis, int axiscount);

float QuaternionAlignAxes(float alignment[4], float (*points)[3], int count,
                          int *axis, int axiscount);
float QuaternionRebias(float bias[3], float (*points)[3], int count,
                       int *axis, int axiscount);

void InitialEstimateSphere(float (*points)[3], int count, float bias[3],
                           float scale[1]);
int CalcBestFitSphere(float (*points)[3], int count, float bias[3],
                      float scale[1]);

void InitialEstimateEllipse(float (*points)[3], int count, float bias[3],
                            float scale[3]);
int CalcBestFitEllipse(float (*points)[3], int count, float bias[3],
                       float scale[3]);

int CalcBestFitRotatedEllipse(float (*points)[3], int count, float bias[3],
                              float alignment[3], float scale[3]);

void ApplyBestFit(float point[3], float n[3], float bias[3],
                  float scale[3], float *alignment);
float CalcEllipseDeviation(float point[3], float bias[3], float scale[3],
                    float *alignment);
float CalcEllipseAvgDeviation(float (*points)[3], int count, float bias[3],
                       float scale[3], float *alignment);

int CalcBestFitPlane(float (*points)[3], int count, float N[3]);
float CalcDeviationPlane(float point[3], float norm[3]);
float CalcAvgDeviationPlane(float (*points)[3], int count, float norm[3]);

float CalcDeviationQuaternion(float apoint[3], float mpoint[3], float Q[4],
                              int southhemisphere);
int CalcBestFitQuaternion(float (*apoints)[3], float (*mpoints)[3], int count,
                          float Q[4], int southhemisphere);
float CalcAvgDeviationQuaternion(float (*apoints)[3], float (*mpoints)[3], 
                                 int count, float Q[4],
                                 int southhemisphere);
