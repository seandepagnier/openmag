#include <stdio.h>
#include <stdlib.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/databools.h>
#include <data/dataenums.h>
#include <data/datastrings.h>
#include <data/dataarrays.h>

#define NUM_ACCEL_POINTS 20
float accelpoints[NUM_ACCEL_POINTS][3] EEMEM;
DATA_ACCESSOR(name=accelpoints type=float mem=eeprom arraylen=3
              count=NUM_ACCEL_POINTS varname=accelpoints)

static const char val1[] PROGMEM = "test1";
static const char val2[] PROGMEM = "test2";
const char *const enumtest_vals[] PROGMEM = {val1, val2, 0};
uint8_t enumtest;
DATA_ACCESSOR(name=enumtest type=enumerant mem=sram varname=enumtest
              enumvalues=enumtest_vals writable)

uint8_t enbG;
DATA_ACCESSOR(name=enbG mem=sram type=bool varname=enbG writable)

#include <avrdata.h>

uint8_t x; DATA_ACCESSOR(name=x type=uint8_t mem=sram varname=x writable)
float df[10]; DATA_ACCESSOR(name=data type=float mem=sram varname=df arraylen=10)

DATA_ACCESSOR(name=testdir dirname=testdir) // directory

uint16_t y; DATA_ACCESSOR(name=y type=uint16_t mem=sram varname=y dir=testdir writable)
char hello[20]; DATA_ACCESSOR(name=hello type=string mem=sram varname=hello arraylen=20 writable)

float f; DATA_ACCESSOR(name=floatingpoint type=float mem=sram varname=f writable)

int main(void)
{
   DATA_OUTPUT(DATA_PROMPT);
   for(;;) {
        char buffer[1000];

        if(!fgets(buffer, sizeof buffer, stdin))
            break;

        buffer[strlen(buffer)-1] = '\0';
        data_process_interactive_cmd(buffer);
    }
    puts("");
    return(0);
}
