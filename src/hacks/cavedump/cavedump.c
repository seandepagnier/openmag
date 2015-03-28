#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#include <poll.h>

#include <hosteeprom.h>
#include <eedata.h>

/* assume stdin is an eeprom dump "make dumpeeprom" */

int main(void)
{
   readeedatafromfile(0);

   const char str[2][8] = {"accels:", "mag:   "};
   int i, j;

   for(i = 0; i<2; i++) {
      puts(str[i]);
      float x[3];
      readbias(x, i);
      printf("bias: %f %f %f\n", x[0], x[1], x[2]);
      readscale(x, i);
      printf("scale: %f %f %f\n", x[0], x[1], x[2]);

      float y[MAXPOINTS][3];
      readpoints(y, i);
      for(j = 0; j<MAXPOINTS; j++)
         printf("point: %f %f %f\n", y[j][0], y[j][1], y[j][2]);
      printf("\n\n");
   }

   return 0;
}
