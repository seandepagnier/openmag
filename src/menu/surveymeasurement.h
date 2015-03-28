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

#define SURVEYMEASUREMENT_DEFAULTNAME {' ', ' ', 'A'}

enum SurveyMeasurementHeader {NEWNAME = 1, BACKSIGHT=2, DISTANCE=4, LRUD=8};

struct surveyname {
   char name[3];
   uint16_t num;
};

struct surveymeasurement {
   uint8_t header;
   struct surveyname from, to;
   float incline, azimuth, bincline, bazimuth;
   float distance, lrud[4];
};

void surveymeasurements_clear(void);
uint16_t surveymeasurement_read(uint16_t pos, struct surveymeasurement *measurement);
void surveymeasurement_delete(struct surveyname *from,
                              struct surveyname *to);
int surveymeasurement_store(struct surveymeasurement *measurement);
void surveymeasurement_stats(float *totaldist, int *freespace);
int surveymeasurement_find(struct surveymeasurement *m);

int surveymeasurement_namecmp(struct surveyname *x, struct surveyname *y);
