
SIM_BINARY = simulation.x
TEST_BINARY = test.x

PLATFORM_DESC    =  platform_desc
TEST_DESC        =  test_desc
SOCLIB_INCLUDES  = -I. -Isrc
SOCLIB_CC_ARGS   = -j4 $(SOCLIB_INCLUDES)

SOCLIB_CC = soclib-cc

SOCLIB = $(shell $(SOCLIB_CC) --getpath)
export SOCLIB

.PHONY: all soft simlog soft_clean $(SIM_BINARY) test_submodules test test_self $(TEST_BINARY)
