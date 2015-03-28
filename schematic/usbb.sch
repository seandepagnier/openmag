v 20080706 1
C 1300 400 1 0 0 gnd-1.sym
C 1400 900 1 0 0 io-1.sym
{
T 2300 1100 5 10 0 0 0 0 1
net=D+:1
T 2300 1000 5 10 1 1 0 1 1
value=D+
}
C 1400 1200 1 0 0 io-1.sym
{
T 2300 1400 5 10 0 0 0 0 1
net=D-:1
T 2300 1300 5 10 1 1 0 1 1
value=D-
}
C 1400 1500 1 0 0 output-2.sym
{
T 2300 1700 5 10 0 0 0 0 1
net=USBVBus:1
T 3200 1600 5 10 1 1 180 1 1
value=USBVBus
}
C 100 500 1 0 0 usb.sym
{
T 800 1900 5 10 1 1 0 0 1
refdes=CONN11
T 200 1900 5 10 1 1 0 0 1
device=USB
T 500 13150 5 10 0 0 0 0 1
footprint=conn_usb2
}
