/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
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

/*
  routines to access accessors defined by DATA_ACCESSOR
*/

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/datautil.h>

#include <avrdata.h>

#include "databasictypes_funcs.h"
#include "dataenums_funcs.h"
#include "databools_funcs.h"
#include "datastrings_funcs.h"
#include "dataquaternions_funcs.h"
#include "dataarrays_funcs.h"

#ifndef DATAGEN
#include "datagen.h"
#endif

/* root directory accessor */
DATA_ACCESSOR(dir=root dirname=root)

/* operators for root directory */
DATA_OPERATOR(allops, dirname=root,
              int c = 0;
              char *x;
              while((x = pgm_read_ptr(data_operators+c++)))
                 DATA_OUTPUT(PGM_STR_FMT" ", x);
              DATA_OUTPUT("\n");
)

static const char data_root[] PROGMEM = "root";
const char data_extraparammsg[] PROGMEM = "This operation requires a parameter\n";

#ifdef __AVR__
static int strcmp_P_P(const char *s1, const char *s2)
{
    char t1 = pgm_read_byte(s1), t2 = pgm_read_byte(s2);
    if(t1 != t2 || !t1)
        return (t1 - t2);
    return strcmp_P_P(s1 + 1, s2 + 1);
}
#else
#define strcmp_P_P strcmp
#endif

static int findop(const char *op)
{
    int i;
    for(i=0; ; i++) {
        const char *pgm_op = pgm_read_ptr(data_operators + i);

        if(!pgm_op) {
            DATA_OUTPUT("<failed to find op: %s>\n", op);
            return 0;
        }

        if(!strcmp_P(op, pgm_op))
            return i + 1;
    }
}

static const char *getdir(data_accessor func)
{
    const char *val = func(DATA_OPERATOR__dir, NULL);
    if(val == DATA_UNIMPLEMENTED)
        return data_root;
    return val;
}

static int issamedir(data_accessor func, const char *dirname)
{
    return !strcmp_P_P(dirname, getdir(func));
}

static data_accessor find_acc_1(const char *name, data_accessor d)
{
    char *nextname = strchr(name, '/');
    if(nextname) {
        *nextname = '\0';
        nextname++;
    }

    /* make sure it's in this directory */
    const char *dirname = d(DATA_OPERATOR__dirname, NULL);
    if(dirname == DATA_UNIMPLEMENTED) {
        DATA_OUTPUT("<not a directory: "PGM_STR_FMT">\n",
                    d(DATA_OPERATOR__name, NULL));
        return NULL;
    }

    int acc = 0;
    data_accessor func;
    while((func = pgm_read_ptr(data_accessors+acc))) {
        const char *n = func(DATA_OPERATOR__name, NULL);

        if(n != DATA_UNIMPLEMENTED && !strcmp_P_P(getdir(func), dirname))
           if(!strcmp_P(name, n)) {
               if(nextname && *nextname)
                   return find_acc_1(nextname, func);

               return func; /* found it */
           }

        acc++;
    }
    return NULL;
}

static data_accessor find_acc(const char *name, data_accessor d)
{
    /* put data on the stack since find_acc_1 modifies it */
    char n[strlen(name) + 1];
    strcpy(n, name);
    return find_acc_1(n, d);
}

static data_accessor find_root(void)
{
    int acc = 0;
    for(;;) {
        data_accessor func = pgm_read_ptr(data_accessors+acc);

        if(!func)
            break;

        const char *dirname = func(DATA_OPERATOR__dirname, NULL);
        if(dirname != DATA_UNIMPLEMENTED && !strcmp_P_P(data_root, dirname))
            return func; /* found it */
        acc++;
    }

    DATA_OUTPUT("severe, error! cannot find root data node\n");
    return NULL;
}

void data_show_operations(const uint8_t ops[])
{
    int i = 0;
    uint8_t x;
    while((x = pgm_read_byte(ops + i++))) {
        const char *op = pgm_read_ptr(data_operators + x - 1);
        DATA_OUTPUT(PGM_STR_FMT" ", op);
    }
    DATA_OUTPUT("\n");
}

void data_list_names(const char *dirname)
{
    int acc = 0;
    for(;;) {
        data_accessor func = pgm_read_ptr(data_accessors+acc);
        if(!func)
            break;

        if(issamedir(func, dirname)) {
            const char *name = func(DATA_OPERATOR__name, NULL);
            if(name != DATA_UNIMPLEMENTED) {
               DATA_OUTPUT(PGM_STR_FMT, name);
                if(func(DATA_OPERATOR__dirname, NULL) != DATA_UNIMPLEMENTED)
                    DATA_OUTPUT("/");
                DATA_OUTPUT(" ");
            }
        }
        acc++;
    }    
    DATA_OUTPUT("\n");
}

void data_process_interactive_cmd(char *cmd)
{
    DATA_OUTPUT_CHAR(DATA_START_CHAR);
    data_process_cmd(cmd);
    DATA_OUTPUT(DATA_PROMPT);
}

void data_process_cmd(char *cmd)
{
    data_accessor func = find_root();

    char cmd2[128];
    strcpy(cmd2, cmd);
    char *op, *name, *param;
    data_split_cmd(cmd, &op, &name, &param);

    if(*name) {
        /* try to find the accessor by name in the accessors */
        if(!(func = find_acc(name, func))) {
            /* can't find it */
            DATA_OUTPUT("<unknown accessor: %s>\n", name);
            return;
        }
    }

    /* special case of empty operator, do nothing */
    if(!*op)
        return;

    int opind = findop(op);
    if(!opind)
       return;

    /* call it! */
    const char *val = func(opind, param);
    if(val == DATA_UNIMPLEMENTED) 
        DATA_OUTPUT("<unimplemented>\n");
    else
       if(val) /* known issue: a longer string here can cause problems
                  with bit-banged usb */
          DATA_OUTPUT("<operation returned unexpectedly>\n");
}
