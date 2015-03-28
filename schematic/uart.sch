v 20080706 1
C 1500 800 1 0 0 connector4-2.sym
{
T 2200 2900 5 10 1 1 0 6 1
refdes=CONN14
T 1800 2850 5 10 0 0 0 0 1
device=CONNECTOR_4
T 1800 3050 5 10 0 0 0 0 1
footprint=CONNECTOR 4 1
}
C 1400 900 1 0 0 gnd-1.sym
C 1300 2400 1 0 0 vcc-1.sym
C 100 1900 1 0 0 input-2.sym
{
T 100 2100 5 10 0 0 0 0 1
net=TX:1
T 600 2000 5 10 1 1 0 7 1
value=TX
}
C 1500 1700 1 180 0 output-2.sym
{
T 600 1500 5 10 0 0 180 0 1
net=RX:1
T 1300 1000 5 10 0 0 180 0 1
device=none
T 600 1600 5 10 1 1 180 1 1
value=RX
}
