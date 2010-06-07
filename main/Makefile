include $(BASE_DIR)/variables.inc
SOFT       = soft/soft.elf
OUTPUT_LOG = simu.log

SUBMODULES = increment video_in video_out

all: test_soclib $(SIM_BINARY) soft

soft:
	$(MAKE) -C soft

test_soclib:
	@test -z "$(SOCLIB)" && (\
	echo "You must have soclib-cc in your $$PATH" ; exit 1 ) || exit 0
	@test ! -z "$(SOCLIB)"



simlog: $(SIM_BINARY) soft
	set -o pipefail ; SOCLIB_TTY=TERM ./$(SIM_BINARY) $(SIMULATION_ARGS) < /dev/null 2>&1 | tee $(OUTPUT_LOG)


soft_clean:
	$(MAKE) -C soft clean

BASE_DIR = $(CURDIR)
export BASE_DIR

include $(BASE_DIR)/rules.inc
