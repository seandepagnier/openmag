v 20070818 1
C 8500 11900 1 270 0 input-2.sym
{
T 8700 11900 5 10 0 0 270 0 1
net=BATTV:1
T 8600 11400 5 10 1 1 270 7 1
value=BATTV
}
C 4700 3100 0 0 0 AT90USB.sym
{
T 9700 7000 5 10 1 1 0 6 1
refdes=U11
T 5100 15550 5 10 0 0 0 0 1
device=at90usb
T 5100 15750 5 10 0 0 0 0 1
footprint=TQFP64_14
}
C 9800 1900 1 0 0 gnd-1.sym
C 10200 3300 1 180 0 crystal-1.sym
{
T 10000 2800 5 10 0 0 180 0 1
device=CRYSTAL
T 10000 3000 5 10 1 1 180 0 1
refdes=U7
T 10000 2600 5 10 0 0 180 0 1
symversion=0.1
T 10200 3300 5 10 0 0 90 0 1
footprint=CS20XTAL
}
C 9700 2200 1 90 0 capacitor-1.sym
{
T 9200 2700 5 10 1 1 90 0 1
value=22pF
T 9200 2400 5 10 1 1 90 0 1
refdes=C5
T 8800 2400 5 10 0 0 90 0 1
footprint=603
T 8800 2400 5 10 0 0 90 0 1
symversion=0.1
}
C 10400 2200 1 90 0 capacitor-1.sym
{
T 9700 2400 5 10 0 0 90 0 1
device=CAPACITOR
T 9900 2400 5 10 1 1 90 0 1
refdes=C6
T 9500 2400 5 10 0 0 90 0 1
footprint=603
T 9500 2400 5 10 0 0 90 0 1
symversion=0.1
}
N 9500 3100 9500 3400 4
N 9500 3400 9700 3400 4
N 10200 3100 10200 3400 4
N 10200 3400 10100 3400 4
N 10200 2200 9500 2200 4
N 11700 1000 11700 3400 4
C 11300 400 1 0 0 gnd-1.sym
N 1700 5900 4900 5900 4
N 8500 3400 8500 1300 4
N 8500 1300 1200 1300 4
C 2800 4000 1 0 0 gnd-1.sym
C 1300 2600 1 270 1 connector5-1.sym
{
T 2800 4400 5 10 0 0 270 6 1
device=CONNECTOR_5
T 3000 2700 5 10 1 1 270 6 1
refdes=CONN2
T 1300 2600 5 10 0 0 180 0 1
footprint=CONNECTOR 5 1
}
C 300 5000 1 0 0 input-2.sym
{
T 300 5200 5 10 0 0 0 0 1
net=MISO:1
T 900 5700 5 10 0 0 0 0 1
device=none
T 800 5100 5 10 1 1 0 7 1
value=MISO
}
C 1700 5600 1 180 0 output-2.sym
{
T 800 5400 5 10 0 0 180 0 1
net=MOSI:1
T 1500 4900 5 10 0 0 180 0 1
device=none
T 800 5500 5 10 1 1 180 1 1
value=MOSI
}
N 1700 5500 4900 5500 4
N 1700 5100 4900 5100 4
C 14700 5800 1 0 0 output-2.sym
{
T 15600 6000 5 10 0 0 0 0 1
net=SCASS:1
T 14900 6500 5 10 0 0 0 0 1
device=none
T 15600 5900 5 10 1 1 0 1 1
value=SCASS
}
C 14700 5400 1 0 0 output-2.sym
{
T 15600 5600 5 10 0 0 0 0 1
net=HMCSS:1
T 14900 6100 5 10 0 0 0 0 1
device=none
T 15600 5500 5 10 1 1 0 1 1
value=HMCSS
}
C 4800 4400 1 180 0 output-2.sym
{
T 3900 4200 5 10 0 0 180 0 1
net=HMCSR+:1
T 4600 3700 5 10 0 0 180 0 1
device=none
T 3900 4300 5 10 1 1 180 1 1
value=HMCSR+
}
N 2400 4300 2400 5900 4
N 2100 4300 2100 5100 4
N 1800 4300 1800 5500 4
N 2700 4300 2900 4300 4
N 1200 1300 1200 4300 4
N 1200 4300 1500 4300 4
N 4900 7100 4800 7100 4
C 4700 7600 1 0 0 gnd-1.sym
C 5000 7500 1 180 0 vcc-1.sym
N 4800 7500 4900 7500 4
N 4800 7900 4900 7900 4
C 3400 7000 1 0 0 input-2.sym
{
T 3400 7200 5 10 0 0 0 0 1
net=USBVBus:1
T 4000 7700 5 10 0 0 0 0 1
device=none
T 3900 7100 5 10 1 1 0 7 1
value=USBVBus
}
C 4800 8400 1 180 0 io-1.sym
{
T 3900 8200 5 10 0 0 180 0 1
net=D+:1
T 3900 8300 5 10 1 1 180 1 1
value=D+
}
C 4800 8800 1 180 0 io-1.sym
{
T 3900 8600 5 10 0 0 180 0 1
net=D-:1
T 3900 8700 5 10 1 1 180 1 1
value=D-
}
N 4800 8700 4900 8700 4
N 4800 8300 4900 8300 4
C 1700 6000 1 180 0 output-2.sym
{
T 800 5800 5 10 0 0 180 0 1
net=SCK:1
T 1500 5300 5 10 0 0 180 0 1
device=none
T 800 5900 5 10 1 1 180 1 1
value=SCK
}
C 3400 9800 1 0 0 input-2.sym
{
T 3400 10000 5 10 0 0 0 0 1
net=SCAINT:1
T 4000 10500 5 10 0 0 0 0 1
device=none
T 3900 9900 5 10 1 1 0 7 1
value=SCAINT
}
N 4800 9900 4900 9900 4
C 7200 10500 1 0 0 vcc-1.sym
N 8600 10400 8600 10500 4
N 7400 10400 7400 10500 4
N 13200 10400 14700 10400 4
N 14700 10700 12900 10700 4
N 12900 10700 12900 10400 4
N 12500 10400 12500 11100 4
N 12500 11100 14700 11100 4
N 14600 9900 14700 9900 4
N 14600 9500 14700 9500 4
N 14600 9100 14700 9100 4
N 14600 8700 14700 8700 4
N 14600 8300 14700 8300 4
N 14600 5100 14700 5100 4
N 14600 4700 14700 4700 4
C 16900 8700 1 180 1 connector2-2.sym
{
T 18500 7400 5 10 1 1 180 0 1
refdes=Bootloader1
T 17200 7450 5 10 0 0 180 6 1
device=CONNECTOR_2
T 17200 7250 5 10 0 0 180 6 1
footprint=CONNECTOR 2 1
}
C 16600 8000 1 0 0 gnd-1.sym
N 14700 10300 14700 10400 4
C 14700 11000 1 0 0 io-1.sym
{
T 15600 11100 5 10 1 1 0 1 1
value=LCD DB0
T 15600 11200 5 10 0 0 0 0 1
net=LCDDB0:1
}
C 14700 10600 1 0 0 io-1.sym
{
T 15600 10800 5 10 0 0 0 0 1
net=LCDDB1:1
T 15600 10700 5 10 1 1 0 1 1
value=LCD DB1
}
C 14700 10200 1 0 0 io-1.sym
{
T 15600 10400 5 10 0 0 0 0 1
net=LCDDB2:1
T 15600 10300 5 10 1 1 0 1 1
value=LCD DB2
}
C 14700 9800 1 0 0 io-1.sym
{
T 15600 10000 5 10 0 0 0 0 1
net=LCDDB3:1
T 15600 9900 5 10 1 1 0 1 1
value=LCD DB3
}
C 14700 9400 1 0 0 io-1.sym
{
T 15600 9600 5 10 0 0 0 0 1
net=LCDDB4:1
T 15600 9500 5 10 1 1 0 1 1
value=LCD DB4
}
C 14700 9000 1 0 0 io-1.sym
{
T 15600 9200 5 10 0 0 0 0 1
net=LCDDB5:1
T 15600 9100 5 10 1 1 0 1 1
value=LCD DB5
}
C 14700 8600 1 0 0 io-1.sym
{
T 15600 8800 5 10 0 0 0 0 1
net=LCDDB6:1
T 15600 8700 5 10 1 1 0 1 1
value=LCD DB6
}
C 14700 8200 1 0 0 io-1.sym
{
T 15600 8400 5 10 0 0 0 0 1
net=LCDDB7:1
T 15600 8300 5 10 1 1 0 1 1
value=LCD DB7
}
C 14700 3800 1 0 0 output-2.sym
{
T 15600 4000 5 10 0 0 0 0 1
net=LCDWR:1
T 14900 4500 5 10 0 0 0 0 1
device=none
T 15600 3900 5 10 1 1 0 1 1
value=LCDWR
}
C 14700 4200 1 0 0 output-2.sym
{
T 15600 4400 5 10 0 0 0 0 1
net=LCDRD:1
T 14900 4900 5 10 0 0 0 0 1
device=none
T 15600 4300 5 10 1 1 0 1 1
value=LCDRD
}
N 14600 4300 14700 4300 4
N 14600 3900 14700 3900 4
C 14700 4600 1 0 0 output-2.sym
{
T 15600 4800 5 10 0 0 0 0 1
net=LCDRS:1
T 14900 5300 5 10 0 0 0 0 1
device=none
T 15600 4700 5 10 1 1 0 1 1
value=LCDRS
}
C 14700 5000 1 0 0 passive-1.sym
{
T 15600 5100 5 10 1 1 0 1 1
value=NC
}
N 4800 6700 4900 6700 4
N 14600 7900 16900 7900 4
C 3400 9400 1 0 0 input-2.sym
{
T 3400 9600 5 10 0 0 0 0 1
net=KEYPAD0:1
T 3900 9500 5 10 1 1 0 7 1
value=KEYPAD0
}
C 8200 1900 1 90 0 input-2.sym
{
T 8000 1900 5 10 0 0 90 0 1
net=KEYPAD1:1
T 8100 2400 5 10 1 1 90 7 1
value=KEYPAD1
}
C 7800 1900 1 90 0 input-2.sym
{
T 7600 1900 5 10 0 0 90 0 1
net=KEYPAD2:1
T 7700 2400 5 10 1 1 90 7 1
value=KEYPAD2
}
C 7400 1900 1 90 0 input-2.sym
{
T 7200 1900 5 10 0 0 90 0 1
net=KEYPAD3:1
T 7300 2400 5 10 1 1 90 7 1
value=KEYPAD3
}
C 10100 11900 1 270 0 input-2.sym
{
T 10300 11900 5 10 0 0 270 0 1
net=KEYPAD4:1
T 10200 11400 5 10 1 1 270 7 1
value=KEYPAD4
}
C 10500 11900 1 270 0 input-2.sym
{
T 10700 11900 5 10 0 0 270 0 1
net=KEYPAD5:1
T 10600 11400 5 10 1 1 270 7 1
value=KEYPAD5
}
C 10900 11900 1 270 0 input-2.sym
{
T 11100 11900 5 10 0 0 270 0 1
net=KEYPAD6:1
T 11000 11400 5 10 1 1 270 7 1
value=KEYPAD6
}
C 11300 11900 1 270 0 input-2.sym
{
T 11500 11900 5 10 0 0 270 0 1
net=KEYPAD7:1
T 11400 11400 5 10 1 1 270 7 1
value=KEYPAD7
}
N 4800 9500 4900 9500 4
N 7300 3300 7300 3400 4
N 7700 3300 7700 3400 4
N 8100 3400 8100 3300 4
N 11300 1000 11700 1000 4
N 11300 700 11400 700 4
C 16600 7600 1 180 0 input-2.sym
{
T 16600 7400 5 10 0 0 180 0 1
net=STRAP0:1
T 16100 7500 5 10 1 1 180 7 1
value=STRAP0
}
N 16700 8300 16900 8300 4
C 16600 7200 1 180 0 input-2.sym
{
T 16600 7000 5 10 0 0 180 0 1
net=STRAP1:1
T 16100 7100 5 10 1 1 180 7 1
value=STRAP1
}
C 16600 6800 1 180 0 input-2.sym
{
T 16600 6600 5 10 0 0 180 0 1
net=STRAP2:1
T 16100 6700 5 10 1 1 180 7 1
value=STRAP2
}
N 14600 6300 14700 6300 4
C 12100 1900 1 90 0 input-2.sym
{
T 11900 1900 5 10 0 0 90 0 1
net=REGERR:1
T 11400 2500 5 10 0 0 90 0 1
device=none
T 12000 2400 5 10 1 1 90 7 1
value=RegERR
}
C 4800 4800 1 180 0 output-2.sym
{
T 3900 4600 5 10 0 0 180 0 1
net=SCARESET:1
T 4600 4100 5 10 0 0 180 0 1
device=none
T 3900 4700 5 10 1 1 180 1 1
value=SCARESET
}
C 12500 1900 1 90 0 input-2.sym
{
T 12300 1900 5 10 0 0 90 0 1
net=BATTCHG:1
T 12400 2400 5 10 1 1 90 7 1
value=BATTCHG
}
C 12700 3300 1 270 0 output-2.sym
{
T 12900 2400 5 10 0 0 270 0 1
net=BATTSELV:1
T 12800 1400 5 10 1 1 90 1 1
value=BATTSELV
}
C 14700 6200 1 0 0 output-2.sym
{
T 15600 6400 5 10 0 0 0 0 1
net=LCDRESET:1
T 14900 6900 5 10 0 0 0 0 1
device=none
T 15600 6300 5 10 1 1 0 1 1
value=LCDRESET
}
C 3400 6600 1 0 0 input-2.sym
{
T 3400 6800 5 10 0 0 0 0 1
net=USBID:1
T 3900 6700 5 10 1 1 0 7 1
value=USBID
}
C 9600 500 1 0 0 connector3-1.sym
{
T 11400 1400 5 10 0 0 0 0 1
device=CONNECTOR_3
T 9600 1600 5 10 1 1 0 0 1
refdes=Serial1
T 9600 500 5 10 0 0 0 0 1
footprint=CONNECTOR 3 1
}
N 11300 1300 11300 3400 4
C 9100 10500 1 90 0 output-2.sym
{
T 8900 11400 5 10 0 0 90 0 1
net=LASER:1
T 9000 12100 5 10 1 1 270 1 1
value=LASER
}
C 9500 10500 1 90 0 io-1.sym
{
T 9400 12000 5 10 1 1 270 1 1
value=LCDBL
T 9300 11400 5 10 0 0 90 0 1
net=LCDBL:1
}
C 4800 4000 1 180 0 output-2.sym
{
T 3900 3800 5 10 0 0 180 0 1
net=HMCSR-:1
T 4600 3300 5 10 0 0 180 0 1
device=none
T 3900 3900 5 10 1 1 180 1 1
value=HMCSR-
}
N 4800 4700 4900 4700 4
N 4800 4300 4900 4300 4
N 4800 3900 4900 3900 4
N 14600 5900 14700 5900 4
N 14600 5500 14700 5500 4
N 10200 10500 10200 10400 4
N 10600 10400 10600 10500 4
N 11000 10500 11000 10400 4
N 11400 10500 11400 10400 4
N 9000 10500 9000 10400 4
N 9400 10400 9400 10500 4
N 12800 3300 12800 3400 4
N 12000 3300 12000 3400 4
N 12400 3300 12400 3400 4
C 14700 6400 1 0 0 gnd-1.sym
N 14800 6700 14600 6700 4
N 14600 7100 14800 7100 4
N 14600 7500 14800 7500 4
N 14800 6700 14800 7500 4
