PROGNAME = calibrationtest
SRCS = calibrationtest.c

CFLAGS = -g

LDFLAGS += -llinalg -lcalibration -lm

INCPREFIX = ../../

include $(INCPREFIX)make/datagen.mk
include $(INCPREFIX)make/app.mk
include $(INCPREFIX)make/host.mk

plotdata: calibrationtest Makefile
	./calibrationtest < log2 > $@

plotcmd: plotdata
	echo "plot 'plotdata'" > $@

plot: plotcmd
	gnuplot -persist plotcmd

plotall: calibrationtest Makefile
	./calibrationtest < $(INCPREFIX)bin/rdata2/logl > plotl
	./calibrationtest < $(INCPREFIX)bin/rdata2/logr > plotr
	./calibrationtest < $(INCPREFIX)bin/rdata2/logu > plotu
	./calibrationtest < $(INCPREFIX)bin/rdata2/logd > plotd
	echo "plot 'plotl', 'plotr', 'plotu', 'plotd'" > plotallcmd
	#gnuplot -persist plotallcmd
