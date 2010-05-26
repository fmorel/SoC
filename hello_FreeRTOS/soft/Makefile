CC_PREFIX    = lm32-elf-
CC           = $(CC_PREFIX)gcc

TARGET_ARCH  = -mmultiply-enabled -mbarrel-shift-enabled
DEBUG_CFLAGS = -ggdb
LDFLAGS      = -Tsoclib.ld
CFLAGS       = -Wall -ggdb -O2 $(DEBUG_CFLAGS) 
CPPFLAGS     = -I.

SOFT_IMAGE = soft.elf

FREERTOS_DIR = ../../FreeRTOS/Source
FREERTOS_INCDIR = $(FREERTOS_DIR)/include
FREERTOS_PLATDIR = $(FREERTOS_DIR)/portable/GCC/LM32
FREERTOS_MEMDIR = $(FREERTOS_DIR)/portable/MemMang

VPATH = $(FREERTOS_DIR) $(FREERTOS_PLATDIR) $(FREERTOS_MEMDIR)

FREERTOS_OBJS = queue.o list.o tasks.o Interrupts.o port.o portasm.o heap_1.o crt0bootloader.o
FREERTOS_OPTS = -DLM32 -I$(FREERTOS_INCDIR)
CFLAGS += $(FREERTOS_OPTS)

PRINTF_DIR = ../../my_printf/

VPATH += $(PRINTF_DIR)
CFLAGS += -I$(PRINTF_DIR)


TIMER_DIR = ../wb_timer
CPPFLAGS += -I$(TIMER_DIR)

OBJS  =  main.o  my_printf.o \
        $(FREERTOS_OBJS)

.PHONY: all clean

all:$(SOFT_IMAGE)

$(SOFT_IMAGE):$(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $^
clean:
	rm -f $(OBJS) $(SOFT_IMAGE)
