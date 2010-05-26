#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#define TIMER_STATUS   0x00
#define TIMER_CONTROL  0x04
#define TIMER_PERIOD   0x08
#define TIMER_SNAPSHOT 0x0c

#define TIMER_CONTROL_INT_BIT_MASK   1 << 0
#define TIMER_CONTROL_CONT_BIT_MASK  1 << 1
#define TIMER_CONTROL_START_BIT_MASK 1 << 2
#define TIMER_CONTROL_STOP_BIT_MASK  1 << 3

#define TIMER_BASE 0xA0000600
#define TIMER_SIZE 0x00000100
#define TIMER_IRQ  1

typedef struct {
  volatile uint32_t Status;
  volatile uint32_t Control;
  volatile uint32_t Period;
  volatile uint32_t Snapshot;
} Timer_t ;

#endif
