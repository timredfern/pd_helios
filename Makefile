# Makefile to build class 'counter' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = helios


#shared.sources = sdk/HeliosDac.cpp libusb/os/darwin_usb.h libusb/os/darwin_usb.c 
# input source file (class name == source file basename)
helios.class.sources = src/sdk/HeliosDac.cpp src/pd_helios.cpp src/helios.cpp
# all extra files to be included in binary distribution of the library
#datafiles = 

ldlibs = libusb-1.0.0.dylib #libHeliosDacAPI.dylib  
#ldlibs = -lHeliosDacAPI -lusb-1.0.0
#ldflags = -lusb-1.0.0
	
# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=src/pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
