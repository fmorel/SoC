#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION                1
#define configUSE_IDLE_HOOK                 0
#define configUSE_TICK_HOOK                 0
#define configCPU_CLOCK_HZ                  100000000
#define configTICK_RATE_HZ                  500
#define configMAX_PRIORITIES                5
#define configMINIMAL_STACK_SIZE            128
#define configTOTAL_HEAP_SIZE               10240
#define configMAX_TASK_NAME_LEN             16
#define configUSE_TRACE_FACILITY            0
#define configUSE_16_BIT_TICKS              0
#define configIDLE_SHOULD_YIELD             1
#define configUSE_MUTEXES                   0
#define configUSE_RECURSIVE_MUTEXES         0
#define configUSE_COUNTING_SEMAPHORES       0
#define configUSE_ALTERNATIVE_API           0
#define configCHECK_FOR_STACK_OVERFLOW      0
#define configQUEUE_REGISTRY_SIZE           10
#define configGENERATE_RUN_TIME_STATS       0

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vResumeFromISR              1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_uxTaskGetStackHighWaterMark 0

#define configUSE_CO_ROUTINES               0 
#define configMAX_CO_ROUTINE_PRIORITIES     1

//#define configKERNEL_INTERRUPT_PRIORITY			[dependent of processor]
//#define configMAX_SYSCALL_INTERRUPT_PRIORITY	[dependent on processor and application]

#endif /* FREERTOS_CONFIG_H */

