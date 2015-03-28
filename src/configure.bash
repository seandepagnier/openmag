# Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

CFG=make/config.mk

echo "# configuration for building openmag" > $CFG
echo >> $CFG

echo "# have host by default" >> $CFG
echo "HAVE_HOST ?= 1" >> $CFG
echo >> $CFG

echo "# set to disable dynamic libraries" >> $CFG
if [ `uname` == "CYGWIN" ]; then
  echo "STATICLIBS = 1" >> $CFG
else
  echo "#STATICLIBS = 1" >> $CFG
fi
echo >> $CFG

echo "# set to enable building for avr" >> $CFG
which avr-gcc &> /dev/null
if (( $? )) ; then
  echo "#HAVE_AVR = 1" >> $CFG
else
  echo "HAVE_AVR = 1" >> $CFG
fi
echo >> $CFG

echo "# set to enable qt" >> $CFG
which qmake &> /dev/null
if (( $? )) ; then
  echo "#HAVE_QT = 1" >> $CFG
else
  echo "HAVE_QT = 1" >> $CFG
fi
echo >> $CFG

echo "# set to enable ccache" >> $CFG
which ccache &> /dev/null
if (( $? )) ; then
  echo "#USE_CCACHE = 1" >> $CFG
else
  echo "USE_CCACHE = 1" >> $CFG
fi
echo >> $CFG

echo "wrote make/config.mk"
