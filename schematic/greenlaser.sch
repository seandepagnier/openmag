v 20080706 1
C 700 2100 1 0 0 input-2.sym
{
T 700 2300 5 10 0 0 0 0 1
net=BATTVCC:1
T 2300 2400 5 10 1 1 0 7 1
value=BATTVCC
}
C 1400 1700 1 270 0 gnd-1.sym
N 2100 2200 4200 2200 4
C 1700 0 1 0 0 irf7509.sym
{
T 3400 1900 5 10 1 1 0 0 1
refdes=U24
T 2100 1900 5 10 1 1 0 0 1
device=IRF7509
T 2100 12650 5 10 0 0 0 0 1
footprint=MSOP8
T 2100 12450 5 10 0 0 0 0 1
device=Atmega64
}
C 300 1100 1 0 0 input-2.sym
{
T 300 1300 5 10 0 0 0 0 1
net=GREENLASER:1
T 1600 1000 5 10 1 1 0 7 1
value=GREENLASER
}
C 4200 1400 1 0 0 connector2-2.sym
{
T 4900 2700 5 10 1 1 0 6 1
refdes=GREENLASER1
T 4500 2650 5 10 0 0 0 0 1
device=CONNECTOR_2
T 4500 2850 5 10 0 0 0 0 1
footprint=CONNECTOR 2 1
}
N 4200 1200 4200 1800 4
