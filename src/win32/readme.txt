This is the readme for release 1 of the dataclient and dataviewer for windows.

This folder contains both applications as well as required dll files and drivers needed.

The dataviewer and dataclient make it possible to communicate with devices.
Using them is covered in the user's guide.

By default the device may not be recognized as a serial device. This means that the dataviewer
will not find it.  To force it into this mode hold button 2 down during powerup (plugging it in)
The first time you do this, windows will prompt for a driver, specify the location of this folder,
the driver is the file OpenMag.inf

I have observed the following bugs which only occur under windows.  If you switch to a different
operating system, you won't have these problems.

1. The device cannot function as both mouse and serial port at the same time.
   On windows xp it will reset the machine if you try.
2. On some windows xp machines, if you plug in two devices both configured as a mouse,
   the machine will reset.
3. On some windows xp machines, if you plug in two devices both configured for serial,
   only the first one will work.
4. The dataviewer is about 20 times slower to populate than on linux or freebsd.
5. Some windows vista machines fail to install the driver complaining that it can't find the specified file.. doesn't specify what file it can't find.
