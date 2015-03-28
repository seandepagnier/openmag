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

#include "dataparser.h"
#include "attribute.h"
#include "accessor.h"
#include "operator.h"

struct accessor_data {
    struct attribute *attributes;
    struct accessor_data *next;
    char filename[128];
    int line;
} *accessors, *accessorstail;

int accessor_count;

void add_accessor(char *start)
{
    struct accessor_data *data = malloc(sizeof *data);
    data->attributes = parseattributes(start + 14);
    data->next = NULL;
    strcpy(data->filename, filename);
    data->line = line;
    if(accessorstail)
        accessorstail->next = data;
    else
        accessors = data;
    accessorstail = data;
}

void print_accessors(FILE *file)
{
    struct accessor_data *acc;
    for(acc = accessors; acc; acc = acc->next) {
        match_operations(acc->attributes);

        fprintf(file, "/* from %s:%d */\n", acc->filename, acc->line);
        fprintf(file, "const char *data_accessor_"NUM_ATTR"(int op, "
                "const char *data_param) {\n");
        print_visible_operation_list(file);
        fprintf(file, "    switch (op) {\n");
        print_operations(file, acc->attributes);
        fprintf(file, "        default:\n            "
                "return DATA_UNIMPLEMENTED;\n    }\n"
                "    return 0;\n}\n\n");

        /* check which attributes were not used */
        struct attribute *a;
        for(a = acc->attributes; a; a = a->next)
            if(!a->usecount) {
                fprintf(stderr, "%s:%d: warning: attribute %s", acc->filename,
                       acc->line, a->name);
                if(a->value)
                    fprintf(stderr, "=%s", a->value);
                fprintf(stderr, " not matched with any operator\n");
            }
    }
}

void print_accessor_header(FILE *file)
{
    fprintf(file, "extern data_accessor data_accessors[%d] PROGMEM;\n\n",
            accessor_count + 1);
}

void print_accessor_table(FILE *file)
{
    fprintf(file, "data_accessor data_accessors[] PROGMEM = {");
    int i;
    for(i = 0; i<accessor_count; i++)
        fprintf(file, "data_accessor_%d,\n"
                "                                          ", i);
    fprintf(file, "0};\n\n");
}

static void free_accessors(struct accessor_data *data)
{
    if(!data)
        return;
    free_accessors(data->next);
    free_attributes(data->attributes);
    free(data);
}

void flush_accessors(void)
{
    free_accessors(accessors);
    accessors = accessorstail = NULL;
}
