LIBNAME=menu
SRCS = compass.c keys.c pedometer.c selection.c survey.c calibration.c gaussmeter.c menu.c reader.c settings.c plots.c readertext.c entry.c surveymeasurement.c stats.c lynnsurvey.c anemometer.c calculator.c spherepainter.c vectorview.c eval.c surveyplot.c

LDFLAGS = -llinalg -lgfx -lglut -lcalibration

MULTILIBDATAGEN=1

all install: rp readertext.h
clean: cleanreadertext

.PHONY: rp

rp:
	$(MAKE) -C readerparser

$(SRCS): readertext.h

include ../make/multilib.mk

cleanreadertext:
	rm -f readertext.h readertext.c
	$(MAKE) -C readerparser clean

readertext.h: readerdata readerparser/readerparser
	touch readertext.do readertext.avr.do
	readerparser/readerparser < readerdata
