v 20080706 1
C 1000 1000 1 0 0 input-2.sym
{
T 1000 1200 5 10 0 0 0 0 1
net=BATT+:1
T 1500 1100 5 10 1 1 0 7 1
value=BATT+
}
C 2400 1000 1 0 0 output-2.sym
{
T 3300 1200 5 10 0 0 0 0 1
net=BATTVCC:1
T 3300 1100 5 10 1 1 0 1 1
value=BATTVCC
}