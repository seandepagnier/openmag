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

const char *getattribute(struct attribute *attribute, const char *name, int use)
{
    struct attribute *p;
    for(p = attribute; p; p = p->next)
        if(!strcmp(name, p->name)) {
            p->usecount += use;
            return p->value;
        }
    return (const char*)-1;
}

static struct attribute *newattribute(const char *name, const char *value)
{
    struct attribute *p = malloc(sizeof *p);
    p->name = malloc(strlen(name) + 1);
    strcpy(p->name, name);
    if(value) {
        p->value = malloc(strlen(value) + 1);
        strcpy(p->value, value);
    } else
        p->value = NULL;
    p->usecount = 0;
    p->next = NULL;

    return p;
}

enum {FULL, DUMPING, EATING, CHOMPING, QUOTED_CHOMPING};
struct attribute *parseattributes(char *start)
{
    struct attribute *p = NULL, *q;

    /* split into words */
    char *c=start, *name, *value;
    int state = DUMPING;
    for(;;) {
        switch(state) {
        case FULL:
            {
                struct attribute *r = newattribute(name, value);
                if(p)
                    q->next = r;
                else
                    p = r;
                q = r;
            }
            if(!*c)
                return p;
            state = DUMPING;
        case DUMPING:
            if(*c == ' ')
                break;
            if(!*c)
                return p;
            name = c;
            value = NULL;
            state = EATING;
            break;
        case EATING:
            switch(*c) {
            case '=':
                *c = '\0';
                value = c+1;
                state = CHOMPING;
                break;
            case '"':
                die("Invalid character: '%c'\n", *c);
                break;
            case ' ':
                *c = '\0';
                state = FULL;
                break;
            }
            break;
        case CHOMPING:
            switch(*c) {
            case '"':
                state = QUOTED_CHOMPING;
                break;
            case ' ':
                *c = '\0';
                state = FULL;
                break;
            }
            break;
        case QUOTED_CHOMPING:
            if(*c == '"')
                state = CHOMPING;
            break;
        }

        c++;
        if(!*c)
            state = FULL;
    }
}

void free_attributes(struct attribute *attrib)
{
    if(!attrib)
        return;
    free_attributes(attrib->next);
    free(attrib->name);
    free(attrib->value);
    free(attrib);
}
