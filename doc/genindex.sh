#!/bin/bash
# genindex.sh -- generate a index page for documentation linking to multiple formats
# 
# Copyright (C) 2008 Sean D'Epagnier
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, you can either send email to this
# program's maintainer or write to: The Free Software Foundation,
# Inc.; 59 Temple Place, Suite 330; Boston, MA 02111-1307, USA.
#

# Usage: $prog DOCNAMES

prog="`basename \"$0\"`"

DOCS+="<ul>"
DOCS+="</ul>"


for NAME in $@; do
# get the title out
TITLE=`grep '@title ' $NAME.texi | sed 's!.*@title !!g'`
DESCRIPTION=`grep '@c genindex description: ' $NAME.texi | sed 's!@c genindex description: !!g'`
DOCS+="<li>$TITLE --  $DESCRIPTION (<a href=\"doc/$NAME.html\">html</a> <a href=\"doc/$NAME/index.html\">html-split</a> <a href="doc/$NAME.pdf">pdf</a> <a href="doc/$NAME.txt"> text </a>)"
done

curdate="`date '+%B %d, %Y'`"
sed \
 -e "s!%%DATE%%!$curdate!g" \
 -e "s!%%DOCS%%!$DOCS!g" \
 -e "s!%%SCRIPTNAME%%!$prog!g" \
index.html.in > index.html
