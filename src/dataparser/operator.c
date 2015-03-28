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

#include <regex.h>

#include "dataparser.h"
#include "attribute.h"
#include "operator.h"

struct operator_list
{
    struct attribute *attributes;
    char *cmds;
    struct operator_list *next;
};

struct operator_data
{
    char *name;
    struct operator_list *current_best_match;
    struct operator_list *operators;
    struct operator_data *next;
} *operators, *operatorstail ;

static struct operator_data *getop(const char *op)
{
    struct operator_data *p;
    for(p = operators; p; p=p->next)
        if(!strcmp(p->name, op))
           return p;

    /* if not found add it! */
    struct operator_data **data = operatorstail ? &operatorstail->next : &operators;
    *data = operatorstail = malloc(sizeof *operatorstail);
    (*data)->next = NULL;

    (*data)->operators = NULL;
    (*data)->name = malloc(strlen(op) + 1);

    strcpy((*data)->name, op);

    return *data;
}

void add_operator(char *start)
{
    /* pull out operator */
    char *op = start + 14;
    while(*op == ' ')
        op++;
    
    char *attributes = strstr(op, "\\attributes");
    if(!attributes)
        die("\\attributes tag not found after data_operator\n");
    attributes[-1] = '\0';
    attributes += 11;

    char *cmds = strstr(attributes, "\\cmds");
    if(!cmds)
        die("\\cmds tag not found after \\data_operator\n");
    cmds[-1] = '\0';
    cmds += 5;

    struct operator_data *dataop = getop(op);

    struct operator_list *data = malloc(sizeof *data);
    data->attributes = parseattributes(attributes);

    data->cmds = malloc(strlen(cmds) + 1);
    strcpy(data->cmds, cmds);

    data->next = dataop->operators;
    dataop->operators = data;
}

void add_unique_operator_name(const char *name)
{
    getop(name);
}

static int match(const char *string, char *pattern)
{
    int status;
    regex_t re;
    char error[256];

    if ((status = regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB)) != 0) {
        regerror(status, &re, error, sizeof error);
        die2("regcomp failed: %s\n", error);
    }

    status = regexec(&re, string, (size_t) 0, NULL, 0);

    regfree(&re);

    return status;
}

/* for each operator match the best fit with the given attributes,
   and store the information internally for print_visible_operation_list
   and print_operations */
void match_operations(struct attribute *attributes)
{
    struct operator_data *d;
    for(d = operators; d; d = d->next) {
        struct operator_list *l;
        struct operator_list *bestmatch = NULL;
        int points = -1;
        for(l = d->operators; l; l=l->next) {
            struct attribute *a;
            int p = 0;
            for(a = l->attributes; a; a = a->next) {
                const char *value = getattribute(attributes, a->name, 0);
                if(value == (const char*)-1
                   || (!value && a->value)) {
                    p = -1;
                    break; /* no match */
                }

                if(!a->value)
                    p += 256; /* compatible operator */

                else
                    if(match(value, a->value)) {
                        p = -1;
                        break; /* no match */
                    } else
                        p += 257; /* exact match */
            }

            if(p > points) {
                points = p;
                bestmatch = l;
            }
        }

        d->current_best_match = bestmatch;
    }
}

void print_visible_operation_list(FILE *file)
{
    fprintf(file, "    static const uint8_t implemented_ops[] PROGMEM = {");
    struct operator_data *d;
    for(d = operators; d; d = d->next) {
        if(!d->current_best_match)
            continue;
        if(d->name[0] != '_') /* don't make internal accessors visible here */
            fprintf(file, "DATA_OPERATOR_%s, ", d->name);
    }
    fprintf(file, "0};\n");
}

/* print all of the operations we can find matches for the given attributes */
void print_operations(FILE *file, struct attribute *attributes)
{
    struct operator_data *d;
    for(d = operators; d; d = d->next) {
        struct operator_list *bestmatch = d->current_best_match;
        if(!bestmatch)
            continue;

        fprintf(file, "        case DATA_OPERATOR_%s:\n        {\n", d->name);
        fprintf(file, "#define DATA_MACRO_"NUM_ATTR"_%s(", d->name);
        struct attribute *a;
        int notfirst = 0;
        for(a = bestmatch->attributes; a; a = a->next) {
            const char *x = getattribute(attributes, a->name, 0);
            if(x == (const char*)-1)
                die2("fatal: invalid match\n");
            if(x) {
                if(notfirst++)
                    fprintf(file, ", ");
                fprintf(file, "%s", a->name);
            }
        }
        fprintf(file, ") %s\n", bestmatch->cmds);
        fprintf(file, "        DATA_MACRO_"NUM_ATTR"_%s(", d->name);
        notfirst = 0;
        for(a = bestmatch->attributes; a; a = a->next) {
            const char *x = getattribute(attributes, a->name, 1);
            if(x) {
                if(notfirst++)
                    fprintf(file, ", ");
                fprintf(file, "%s", x);
            }
        }
        fprintf(file, ")\n");
        fprintf(file, "        } break;\n");
    }
}

void print_operator_header(FILE *file)
{
    /* calculate count */
    struct operator_data *d;
    int count = 0;
    for(d = operators; d; d = d->next)
        count++;

    fprintf(file, "extern const char * const\ndata_operators[%d] PROGMEM;\n\n",
            count + 1);

    /* put the enum out there */
    fprintf(file, "enum {");
    int cc=1, hc = -1;
    for(d = operators; d; d = d->next) {
        int c = d->name[0] == '_' ? hc-- : cc++;
        fprintf(file, "\n      DATA_OPERATOR_%s = %d,", d->name, c);
    }

    if(!operators)
        fprintf(file, "NO_OPERATORS");

    fprintf(file, "};\n\n");
}

void print_operator_table(FILE *file)
{
    int count = 0;
    struct operator_data *d;
    for(d = operators; d; d = d->next) {
        if(d->name[0] == '_')
            continue;

        char buf[2*strlen(d->name)];
        char *x = buf, *y = d->name;
        while(*y) {
            if(*y == '"' || *y == '\\') {
                *x = '\\';
                x++;
            }
            *x = *y;
            x++;
            y++;
        }
        *x = '\0';

        fprintf(file, "static const char data_operator_%d[] PROGMEM"
                "= \"%s\";\n", count, buf);
        count++;
    }

    fprintf(file, "\nconst char *const\ndata_operators[] PROGMEM = {\n");

    int i;
    for(i = 0; i<count; i++)
        fprintf(file, "data_operator_%d,\n", i);

    fprintf(file, "0};\n\n");
}
