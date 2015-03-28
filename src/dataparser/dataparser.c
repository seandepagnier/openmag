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

// this program generates a table of operations for each accessor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataparser.h"
#include "attribute.h"
#include "accessor.h"
#include "operator.h"

char filename[128];
int line;

static FILE *safeopen(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if(!file)
        die2("Failed to open: %s with mode: %s\n", filename, mode);
    return file;
}

static void compile(void)
{
    line = 0;
    for(;;) {
        line++;
        char buffer[1024];
        if(!fgets(buffer, sizeof buffer, stdin))
            break;
        int len = strlen(buffer);
        if(buffer[len-1] != '\n')
            die("line greater than '%d' bytes, aborting\n", sizeof buffer);

        buffer[len-1] = '\0';

        /* since we typically work with preprocessed files,
           look for lines with "# num str params..." */
        if(buffer[0] == '#') {
            char *endptr;
            line = strtol(buffer + 1, &endptr, 10) - 1;
            if(endptr == buffer + 1)
                die("invalid input line starting with %d");
            
            char *f = strchr(endptr, '"');
            if(f) {
                char *endf = strchr(f + 1, '"');
                if(endf) {
                    *endf = '\0';
                    strncpy(filename, f+1, sizeof filename);
                }
            }
        }
            
        /* search for \data tags */
        char *start = buffer, *end;
        while((end = strstr(start, "\\data_end"))) {
            *end = '\0';
            char *begin;
            if((begin = strstr(start, "\\data_accessor")))
                add_accessor(begin);
            if((begin = strstr(start, "\\data_operator")))
                add_operator(begin);
            start = end + 1;
        }
    }
}

static void addfile(const char *filename, FILE *output)
{
    /* put the whole file in a buffer */
    FILE *input = safeopen(filename, "r");
    fseek(input, 0, SEEK_END);
    long len = ftell(input);

    fseek(input, 0, SEEK_SET);
    char *buffer = malloc(len+1);
    buffer[len] = '\0';
    if(fread(buffer, len, 1, input) == -1)
        die("Unable to read file: %s\n", filename);

    /* go through the file seaching for NUM_ATTR enumerating it */
    char *start = strstr(buffer, "data_accessor_"NUM_ATTR), *end = 0;
    if(start) 
        fwrite(buffer, start - buffer, 1, output);
    while(start && end < buffer + len) {
        if(!(end = strstr(start + 1, "data_accessor_"NUM_ATTR)))
            end = buffer + len;

        char num[] = NUM_ATTR;
        int c = 0;

        char *p = start;
        while(p < end) {
            if(*p == num[c]) {
                if(++c == strlen(num)) {
                    fprintf(output, "%d", accessor_count);
                    c = 0;
                }
            } else {
               int i;
               for(i=0; i<c; i++)
                  fputc(num[i], output);
               c = 0;

               fputc(*p, output);
            }
            p++;
        }

        start = end;
        accessor_count++;
    }

    /* go through the file seaching for operators, and add them to the list */
    char *op = buffer - 1;
    const char query[] = "case DATA_OPERATOR_";
    while((op = strstr(op + 1, query))) {
        char *s = op + sizeof query - 1;
        char *e = strchr(s, ':');
        char name[e - s + 1];
        memcpy(name, s, e - s);
        name[e - s] = '\0';
        add_unique_operator_name(name);
    }
    free(buffer);
}

int main(int argc, char *argv[])
{
    /* no args means compile mode, read from stdin, write to stdout */
    if(argc == 1) {
        compile();
        print_accessors(stdout);
        return 0;
    }

    /* link mode, files specified on command line from previous compile output
       to get linked together into datagen.c and datagen.h */
    FILE *source = safeopen("datagen.c", "w");
    FILE *header = safeopen("datagen.h", "w");
    if(!source || !header)
        die2("failed to open datagen.c and/or datagen.h for writing\n");

    /* static header */
    fprintf(source, "/* Generated file.\n   Created by dataparser contains "
            "reflection data */\n\n");

    fprintf(source, "#define COMPILE_DATAGEN\n");
    fprintf(source, "#include <data/data.h>\n");
    fprintf(source, "#include \"datagen.h\"\n\n");

    /* process each file */
    int i;
    for(i = 1; i<argc; i++)
        addfile(argv[i], source);

    /* tables */
    print_operator_table(source);
    print_accessor_table(source);

    /* write header file */
    fprintf(header, "/* Generated file.\n   To be included if direct access "
            "to data reflection data is required */\n\n");

    print_operator_header(header);
    print_accessor_header(header);

    return 0;
}
