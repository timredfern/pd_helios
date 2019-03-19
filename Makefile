# Makefile to build class 'helios' for Pure Data.

lib.name = helios

helios.class.sources = src/sdk/HeliosDac.cpp src/pd_helios.cpp src/helios.cpp

uname := $(shell uname)

ifeq ($(findstring $(uname), Linux GNU GNU/kFreeBSD), $(uname))
  system = Linux
endif

ifeq ($(uname), Darwin)
  system = Darwin
endif

ifeq ($(filter MINGW% MSYS%, $(uname)), $(uname))
  system = Windows
endif

ifeq ($(system), Darwin)
	ldlibs =  libusb-1.0.0.dylib
else
	ldlibs = -lusb-1.0
endif

ldflags = -Wl,-rpath,./

include src/pd-lib-builder/Makefile.pdlibbuilder
