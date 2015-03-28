v 20070818 1
C 2200 400 0 0 0 sca3000.sym
{
T 5000 4200 5 10 1 1 0 0 1
refdes=U5
T 2700 4200 5 10 1 1 0 0 1
device=SCA3000
}
N 2000 1000 2200 1000 4
N 5700 3000 6200 3000 4
N 6200 2600 5700 2600 4
N 2200 2600 1900 2600 4
N 1900 2600 1900 2200 4
N 1900 2200 2200 2200 4
C 6100 2300 1 0 0 gnd-1.sym
C 1800 1900 1 0 0 gnd-1.sym
N 2100 200 2100 1800 4
C 1500 1400 1 90 0 capacitor-1.sym
{
T 1000 1600 5 10 1 1 90 0 1
refdes=C3
T 600 1600 5 10 0 0 90 0 1
symversion=0.1
T 1500 1500 5 10 1 1 90 0 1
value=1uF
T 1500 1400 5 10 0 0 0 0 1
footprint=603
}
C 7100 2300 1 90 0 capacitor-1.sym
{
T 6600 2500 5 10 1 1 90 0 1
refdes=C4
T 6200 2500 5 10 0 0 90 0 1
symversion=0.1
T 7100 2300 5 10 1 1 90 0 1
value=1uF
T 7100 2300 5 10 0 0 0 0 1
footprint=603
}
N 1300 2300 1900 2300 4
N 1300 1400 2100 1400 4
N 6200 3200 6900 3200 4
N 6900 2300 6900 2200 4
N 6200 2600 6200 3200 4
N 2200 3400 2100 3400 4
N 5700 2200 6900 2200 4
N 5800 2200 5800 200 4
N 2100 200 6100 200 4
C 600 900 1 0 0 input-2.sym
{
T 600 1100 5 10 0 0 0 0 1
net=SCASS:1
T 1200 1600 5 10 0 0 0 0 1
device=none
T 1100 1000 5 10 1 1 0 7 1
value=SCASS
}
C 700 3300 1 0 0 input-2.sym
{
T 700 3500 5 10 0 0 0 0 1
net=SCARESET:1
T 1300 4000 5 10 0 0 0 0 1
device=none
T 1200 3400 5 10 1 1 0 7 1
value=SCARESET
}
N 2200 3000 2100 3000 4
C 2100 3100 1 180 0 output-2.sym
{
T 1200 3000 5 10 1 1 180 1 1
value=SCAINT
T 1200 2900 5 10 0 0 180 0 1
net=SCAINT:1
T 1900 2400 5 10 0 0 180 0 1
device=none
}
C 7300 1100 1 180 0 input-2.sym
{
T 7300 900 5 10 0 0 180 0 1
net=SCK:1
T 6700 400 5 10 0 0 180 0 1
device=none
T 6800 1000 5 10 1 1 180 7 1
value=SCK
}
C 7300 1900 1 180 0 input-2.sym
{
T 7300 1700 5 10 0 0 180 0 1
net=MOSI:1
T 6700 1200 5 10 0 0 180 0 1
device=none
T 6800 1800 5 10 1 1 180 7 1
value=MOSI
}
C 5900 1300 1 0 0 output-2.sym
{
T 6800 1500 5 10 0 0 0 0 1
net=MISO:1
T 6100 2000 5 10 0 0 0 0 1
device=none
T 6800 1400 5 10 1 1 0 1 1
value=MISO
}
N 5700 1800 5900 1800 4
N 5700 1400 5900 1400 4
N 5700 1000 5900 1000 4
N 2200 1800 2100 1800 4
N 2100 1400 2200 1400 4
C 5900 200 1 0 0 vcc-1.sym
