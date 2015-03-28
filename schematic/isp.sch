v 20080706 1
C 2600 1700 1 0 0 output-2.sym
{
T 3500 1900 5 10 0 0 0 0 1
net=RESET:1
T 2800 2400 5 10 0 0 0 0 1
device=none
T 4100 1800 5 10 1 1 180 1 1
value=RESET
}
C 2600 500 1 0 0 output-2.sym
{
T 3500 700 5 10 0 0 0 0 1
net=SCK:1
T 2800 1200 5 10 0 0 0 0 1
device=none
T 3500 600 5 10 1 1 0 1 1
value=SCK
}
C 600 100 1 0 0 avrisp.sym
{
T 1795 2100 5 10 1 1 0 0 1
refdes=CONN13
T 1000 12750 5 10 0 0 0 0 1
footprint=CONNECTOR 3 2
}
C 600 900 1 270 0 gnd-1.sym
C 900 1400 1 90 0 vcc-1.sym
C 2600 1300 1 0 0 output-2.sym
{
T 3500 1500 5 10 0 0 0 0 1
net=MOSI:1
T 2800 2000 5 10 0 0 0 0 1
device=none
T 3500 1400 5 10 1 1 0 1 1
value=MOSI
}
C 4000 1100 1 180 0 input-2.sym
{
T 4000 900 5 10 0 0 180 0 1
net=MISO:1
T 3400 400 5 10 0 0 180 0 1
device=none
T 3500 1000 5 10 1 1 180 7 1
value=MISO
}
