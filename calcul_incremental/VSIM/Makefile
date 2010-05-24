SOCLIB_PATH = /comelec/softs/opt/www.soclib.fr/distrib/soclib

SOCLIB = $(shell $(SOCLIB_CC) --getpath)
export SOCLIB

.PHONY: all sim clean

all: lib.done system.done

clean:
	rm -f *.done
	rm -rf work
	rm -f transcript  vsim.wlf


lib.done:
	vlib work
	@touch $@

calcul_incremental.done : ../calcul_incremental.cpp
	sccom -DSOCLIB_MODULE_DEBUG=1 `sdl-config --cflags` -ggdb -DSOCLIB_MODE_DEBUG -Wall  -DSOCLIB -I$(SOCLIB_PATH)/soclib/lib/include -I$(SOCLIB_PATH)/soclib/communication/wishbone/caba/source/include/  -c ../calcul_incremental.cpp
	@touch $@

top.done : ../top.cpp
	sccom `sdl-config --cflags` -O2 -DSOCLIB_MODE_RELEASE -Wall  -DSOCLIB -I$(SOCLIB_PATH)/soclib/lib/include  -c  ../top.cpp
	@touch $@

system.done : calcul_incremental.done  top.done
	sccom -link -lpng `sdl-config --libs`
	@touch $@


sim:system.done
	vsim -novopt sc_main
