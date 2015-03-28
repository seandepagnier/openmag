v 20080706 1
C 1300 300 1 0 0 gnd-1.sym
N 1400 600 1400 700 4
C 1400 1500 1 0 0 resistor-1.sym
{
T 1400 1700 5 10 1 1 0 0 1
refdes=R4
T 1400 1500 5 10 0 0 0 0 1
footprint=603
T 1800 1700 5 10 1 1 0 0 1
value=22
}
C 1400 1200 1 0 0 resistor-1.sym
{
T 1400 1400 5 10 1 1 0 0 1
refdes=R5
T 1400 1200 5 10 0 0 0 0 1
footprint=603
T 1800 1400 5 10 1 1 0 0 1
value=22
}
C 2300 1200 1 0 0 io-1.sym
{
T 3200 1400 5 10 0 0 0 0 1
net=D+:1
T 3200 1300 5 10 1 1 0 1 1
value=D+
}
C 2300 1500 1 0 0 io-1.sym
{
T 3200 1700 5 10 0 0 0 0 1
net=D-:1
T 3200 1600 5 10 1 1 0 1 1
value=D-
}
C 2300 1800 1 0 0 output-2.sym
{
T 3200 2000 5 10 0 0 0 0 1
net=USBVBus:1
T 4000 1900 5 10 1 1 180 1 1
value=USBVBus
}
N 2300 1900 1400 1900 4
C 100 600 1 0 0 usbmini.sym
{
T 800 2200 5 10 1 1 0 0 1
refdes=CONN12
T 200 2200 5 10 1 1 0 0 1
device=USB
T 500 13250 5 10 0 0 0 0 1
footprint=usbminibrotated
}
C 2300 900 1 0 0 output-2.sym
{
T 3200 1100 5 10 0 0 0 0 1
net=USBID:1
T 3800 1000 5 10 1 1 180 1 1
value=USBID
}
N 1400 1000 2300 1000 4
