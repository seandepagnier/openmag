#
#             MyUSB Library
#     Copyright (C) Dean Camera, 2008.
#              
#  dean [at] fourwalledcubicle [dot] com
#      www.fourwalledcubicle.com
#

# Makefile to build the MyUSB library and Demos.

# Call with "make all" to rebuild everything, "make clean" to clean everything,
# "make clean_list" to remove all intermediatary files but preserve any binaries
# and "make doxygen" to document the project with DoxyGen (if installed).

# It is suggested that for the master build, the --quiet switch is passed to make,
# to remove all the commands from the output. This gives a much easier to read
# report of the entire build process.

all:

doxygen:
	@echo Generating library documentation.
	@echo
	doxygen doxygen.conf
	@echo
	@echo Generation of library documentation complete.

%:
	@echo Executing \"make $@\" on all MyUSB library elements.
	@echo
	make -C MyUSB/ $@
	make -C Demos/ $@
	make -C Projects/ $@
	make -C Bootloaders/ $@
	@echo
	@echo MyUSB \"make $@\" operation complete.