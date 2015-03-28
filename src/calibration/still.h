/* Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
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

#define NUMSTILLPOINTS    64
extern float stillpoints[NUMSTILLPOINTS][6];
extern uint8_t numstillpoints;
extern uint8_t jointstillpoints;

enum StillState {NOTSTILL, STILLSETTLED, STILL};
enum PointType {ACCEL, MAG};

int still_count_stillpoints(int sensor);
void still_accumulate(struct sensor_typ *s, int32_t *datain);
void still_force_update(float a[3], float m[3]);
void still_poll(void);
void still_calibrate_accel(void);
void still_calibrate_mag(void);
int still_cal_done(float olddev, float dev);
float still_runpoints(int (*makerow)(float *H, float *Z,
                                     const float *X, int i),
                      float *X, int m, int n);

extern volatile uint8_t stillcalibrateaccel, stillcalibratemag;
extern uint8_t calibrationdebugging, calibrationfastdebugging;
extern uint8_t still_update;
