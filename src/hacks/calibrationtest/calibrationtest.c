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

#include <stdint.h>
#include <math.h>

#include <calibration/calibrationserver.h>
#include <calibration/calc.h>

#include <linalg/vector.h>

#define AZMODE

int main(int argc, char *argv[])
{
#if 1
  accelX[0] = -149.1;
  accelX[1] = -4.6;
  accelX[2] = 123.0;
  accelX[3] = 1328.9;

  magX[0] = 1.0 / 50100;
  magX[1] =  .992;
  magX[2] = 1.05;
  magX[3] = -0.106;
  magX[4] =  0.00694;
  magX[5] = -0.0736;
  magX[6] = -7040;
  magX[7] = -9120;
  magX[8] = -15360;

#if 0 /* l d */
  magX[6] = -6585;
  magX[7] = -9120;
  magX[8] = -15430;
#endif
#if 0 /* u r */
  magX[6] = -7000;
  magX[7] = -9820;
  magX[8] = -15150;
#endif
#if 0 /* s (d and r) */
  magX[6] = -6010;
  magX[7] = -8750;
  magX[8] = -15150;
#endif

  magalignX[0] = 0.002;
  magalignX[1] = -.003;
  magalignX[2] = 0.021;

#else
  accelX[0] = 198.3;
  accelX[1] = 60;
  accelX[2] = -42.5;
  accelX[3] = 1327.1;

  magX[0] = 1.0 / 50400;
  magX[1] =  .974;
  magX[2] = 1.05;
  magX[3] = -0.11;
  magX[4] =  0.0558;
  magX[5] = -0.0679;

  magX[6] = 38500;
  magX[7] = 19600;
  magX[8] = -29800;

#if 0 /* r */
  magX[6] = 37200;
  magX[7] = 18300;
  magX[8] = -29700;
#endif
#if 0 /* u */
  magX[6] = 37540;
  magX[7] = 20360;
  magX[8] = -29550;
#endif
#if 0 /* l */
  magX[6] = 37580;
  magX[7] = 20250;
  magX[8] = -30070;
#endif
#if 1 /* d */
  magX[6] = 37410;
  magX[7] = 21060;
  magX[8] = -30520;
#endif

  magalignX[0] = 0.0020;
  magalignX[1] = 0.000453;
  magalignX[2] = 0.0384;

#endif

  accellaseralignment[1] = 0.00;
  accellaseralignment[2] = -0.013;
  accellaseralignment[3] = -.01;

  accellaseralignment[0] = sqrt(1 - magnitude2(accellaseralignment+1));
  memcpy(maglaseralignment, accellaseralignment, sizeof accellaseralignment);

  float corrazimuth = 0, total=0, totalm=0;
  char buffer[1024];
  static float asave[72];
  int i=0;
  while(fgets(buffer, sizeof buffer, stdin)) {
    if(sscanf(buffer, "rawaccel: %ld %ld %ld\n",
              accel.filtered + 0,
              accel.filtered + 1,
              accel.filtered + 2) == 3) {
      int i;
      for(i = 0; i<3; i++)
        accel.filtered[i] <<= 4;
    }

    if(sscanf(buffer, "rawmag: %ld %ld %ld\n",
              mag.filtered + 0,
              mag.filtered + 1,
              mag.filtered + 2) == 3) {

      calc_laser_incline_azimuth();

      float a[3];
      calc_get_calibrated_accel(a);

      float diff = azimuth - corrazimuth;
      if(diff < -50)
        diff += 360;
      if(diff > 50)
        diff -= 360;

      float m = 1-calibration_get_mag_magnitude();
      totalm += m*m;

      total += diff;
      asave[i++] = diff;

#ifdef AZMODE
      corrazimuth += 5;
#endif
    }
  }

  float dev = 0;
  corrazimuth = 0;
  int j = 0;
  for(j=0; j<i; j++) {
    float d = asave[j] - total / i;

    printf("%.2f %.2f %.2f\n", corrazimuth, d, asave[j]);

#ifdef AZMODE
      corrazimuth += 5;
#else
      corrazimuth += 1;
#endif

    dev += d*d;
  }

  fprintf(stderr, "mag dev: %.5f\n", sqrt(totalm / 72));
  fprintf(stderr, "dev: %.5f\n", sqrt(dev / 72));

  return 0;
}
