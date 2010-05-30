CC_PREFIX    = lm32-elf-
CC           = $(CC_PREFIX)gcc

TARGET_ARCH  = -mmultiply-enabled -mbarrel-shift-enabled
DEBUG_CFLAGS = -ggdb
LDFLAGS      = -Tsoclib.ld
CFLAGS       = -Wall -ggdb -O2 $(DEBUG_CFLAGS) 
CPPFLAGS     = -I. -Isrc

SOFT_IMAGE = soft.elf

OBJS  =  main.o lm32_irq.o

ROOT_DIR = ..
CFLAGS+= -I$(ROOT_DIR)

.PHONY: all clean

all:$(SOFT_IMAGE)

$(SOFT_IMAGE):$(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $^
clean:
	rm -f $(OBJS) $(SOFT_IMAGE)
