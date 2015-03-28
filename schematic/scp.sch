v 20080706 1
C 800 600 1 0 0 capacitor-1.sym
{
T 800 900 5 10 1 1 0 0 1
refdes=C21
T 1000 1500 5 10 0 0 0 0 1
symversion=0.1
T 900 600 5 10 1 1 0 0 1
value=1uF
T 800 600 5 10 0 0 270 0 1
footprint=603
}
C 5900 500 1 180 0 input-2.sym
{
T 5900 300 5 10 0 0 180 0 1
net=SCK:1
T 5300 -200 5 10 0 0 180 0 1
device=none
T 5400 400 5 10 1 1 180 7 1
value=SCK
}
C 5900 900 1 180 0 input-2.sym
{
T 5900 700 5 10 0 0 180 0 1
net=MOSI:1
T 5300 200 5 10 0 0 180 0 1
device=none
T 5400 800 5 10 1 1 180 7 1
value=MOSI
}
C 4500 1100 1 0 0 output-2.sym
{
T 5400 1300 5 10 0 0 0 0 1
net=MISO:1
T 4700 1800 5 10 0 0 0 0 1
device=none
T 5400 1200 5 10 1 1 0 1 1
value=MISO
}
C 1700 200 1 0 0 scp1000.sym
{
T 3900 3600 5 10 1 1 0 0 1
refdes=U22
T 2200 3600 5 10 1 1 0 0 1
device=SCP1000
T 2100 12850 5 10 0 0 0 0 1
footprint=scp1000
}
C 5900 1700 1 180 0 input-2.sym
{
T 5900 1500 5 10 0 0 180 0 1
net=SCPDRDY:1
T 5300 1000 5 10 0 0 180 0 1
device=none
T 5400 1600 5 10 1 1 180 7 1
value=SCPSS
}
C 1400 500 1 270 0 gnd-1.sym
C 1400 1300 1 270 0 gnd-1.sym
C 1500 1600 1 0 0 vcc-1.sym
C 4800 2700 1 90 0 gnd-1.sym
N 4500 2400 4500 3200 4
C 4300 2000 1 0 0 vcc-1.sym
C 1700 2500 1 180 0 output-2.sym
{
T 800 2300 5 10 0 0 180 0 1
net=SCPDRDY:1
T 1500 1800 5 10 0 0 180 0 1
device=none
T 1500 2600 5 10 1 1 180 1 1
value=SCPDRDY
}
C 1400 3100 1 270 0 gnd-1.sym
N 1700 3200 1700 2800 4
C 1400 2100 1 270 0 gnd-1.sym
C 500 900 1 270 0 gnd-1.sym
C 800 1400 1 0 0 capacitor-1.sym
{
T 800 1700 5 10 1 1 0 0 1
refdes=C22
T 1000 2300 5 10 0 0 0 0 1
symversion=0.1
T 900 1400 5 10 1 1 0 0 1
value=1uF
T 800 1400 5 10 0 0 270 0 1
footprint=603
}
C 500 1700 1 270 0 gnd-1.sym
C 4500 1800 1 0 0 capacitor-1.sym
{
T 5100 2100 5 10 1 1 0 0 1
refdes=C23
T 4700 2700 5 10 0 0 0 0 1
symversion=0.1
T 4600 1800 5 10 1 1 0 0 1
value=1uF
T 4500 1800 5 10 0 0 270 0 1
footprint=603
}
C 5700 1900 1 90 0 gnd-1.sym
