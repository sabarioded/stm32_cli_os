#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <stdint.h>

/* ============================================================================
   System Clock Configuration
   ============================================================================ */
#define SYSCLK_HZ          80000000UL  /* 80 MHz system clock */

/* ============================================================================
   Task Stack Allocation Mode
   ============================================================================
   Choose between static (compile-time) or dynamic (runtime) stack allocation.
   
   STATIC MODE (TASK_ALLOC_STATIC):
   ---------------------------------
   - Task stacks are embedded in the task_t structure
   - All stacks allocated at compile time in .bss section
   - Memory usage: ~60 KB for task_list[58] (1032 bytes per task)
   - Pros: Simple, predictable, no allocation failures
   - Cons: Wastes memory if not all tasks used, large .bss section
   - Best for: Systems with fixed number of tasks, plenty of RAM
   
   DYNAMIC MODE (TASK_ALLOC_DYNAMIC):
   -----------------------------------
   - Task stacks allocated from heap at runtime via heap_malloc()
   - Task structures are small (~16 bytes), stacks allocated on demand
   - Memory usage: ~928 bytes for task_list[58] + 1020 bytes per active task
   - Heap available: ~92 KB in SRAM1
   - Pros: Memory efficient, flexible, can create/destroy tasks dynamically
   - Cons: Can fail if heap exhausted, slightly more complex
   - Best for: Systems with variable task count, limited RAM
   
   RECOMMENDATION: Use DYNAMIC mode unless you have a specific reason not to.
   It's more memory efficient and allows runtime flexibility.
============================================================================ */
#define TASK_ALLOC_STATIC      0
#define TASK_ALLOC_DYNAMIC     1

/* Choose allocation mode: */
#define TASK_STACK_ALLOC_MODE  TASK_ALLOC_DYNAMIC  /* Recommended for most applications */

/* ============================================================================
   Memory Layout Summary (STM32L476RG - 96 KB SRAM1)
   ============================================================================
   
   STATIC MODE:
   ------------
   .bss (task stacks):  ~60 KB  (58 tasks * 1032 bytes)
   .bss (other):        ~4 KB   (globals, buffers)
   Heap:                ~32 KB  (mostly unused)
   
   DYNAMIC MODE:
   -------------
   .bss (task structs): ~1 KB   (58 tasks * 16 bytes)
   .bss (other):        ~4 KB   (globals, buffers)
   Heap:                ~92 KB  (task stacks allocated from here)
     - Idle task:       1020 bytes (always present)
     - Per task:        1020 bytes (allocated on task_create)
     - Available:       ~92 KB total
     - Max tasks:       min(58, 92KB/1KB) = 58 tasks (theoretically ~90 if increased MAX_TASKS)
   
============================================================================ */

/* ============================================================================
   CLI Configuration
   ============================================================================ */
#define CLI_MAX_LINE_LEN       128    /* Maximum command line length */
#define CLI_MAX_ARGS           16     /* Maximum number of command arguments */
#define CLI_MAX_CMDS           32     /* Maximum number of registered commands */

/* ============================================================================
   UART Configuration
   ============================================================================ */
#define UART_BAUD_DEFAULT      115200 /* Default UART baud rate */
#define UART_RX_BUFFER_SIZE    256    /* UART receive buffer size */
#define UART_TX_BUFFER_SIZE    512    /* UART transmit buffer size */

/* ============================================================================
   Scheduler Configuration
   ============================================================================ */
#define MAX_TASKS              58     /* Maximum number of tasks */
#define SYSTICK_FREQ_HZ        1000   /* SysTick interrupt frequency (1 kHz = 1ms tick) */

/* Stack overflow detection */
#define STACK_CANARY           0xDEADBEEF  /* Magic value at stack bottom */

/* Garbage collection */
#define GARBAGE_COLLECTION_TICKS 1000U  /* Run GC every 1000 ticks (1 second at 1kHz) */

/* ============================================================================
   Interrupt Priorities (ARM Cortex-M)
   ============================================================================
   Lower numbers = higher priority
   STM32L4 has 4 bits = 16 priority levels (0-15)
   
   Priority 0:  Highest (critical hard faults)
   Priority 5:  High-priority peripherals (timers, critical I/O)
   Priority 10: Normal peripherals (UART, SPI, I2C)
   Priority 14: SysTick (lower than most peripherals)
   Priority 15: PendSV (lowest - for context switching)
============================================================================ */
#define MAX_SYSCALL_PRIORITY   5   /* Highest priority that can call RTOS functions */
#define SYSTICK_PRIORITY       14  /* SysTick priority (lower than peripherals) */
#define PENDSV_PRIORITY        15  /* PendSV priority (lowest for context switch) */

/* ============================================================================
   Stack Size Configuration
   ============================================================================ */
#define STACK_SIZE_IN_WORDS      255    /* Default: 255 words = 1020 bytes */
#define STACK_SIZE_BYTES         (STACK_SIZE_IN_WORDS * sizeof(uint32_t))

/* Dynamic allocation limits */
#define STACK_MIN_SIZE_BYTES     512    /* Minimum stack size (128 words) */
#define STACK_MAX_SIZE_BYTES     8192   /* Maximum stack size (2048 words) */

/* Common stack sizes (for convenience) */
#define STACK_SIZE_512B         512    /* For simple tasks */
#define STACK_SIZE_1KB          1024   /* For normal tasks (default) */
#define STACK_SIZE_2KB          2048   /* For tasks with deep call stacks */
#define STACK_SIZE_4KB          4096   /* For tasks with large local variables */

/* ============================================================================
   Debug Configuration
   ============================================================================ */
/* #define DEBUG */  /* Uncomment to enable debug features */

#ifdef DEBUG
    #define DEBUG_STACK_OVERFLOW_CHECK  1  /* Enable stack overflow checking */
    #define DEBUG_HEAP_STATS            1  /* Enable heap statistics */
    #define DEBUG_TASK_STATS            1  /* Enable task statistics */
#else
    #define DEBUG_STACK_OVERFLOW_CHECK  0
    #define DEBUG_HEAP_STATS            0
    #define DEBUG_TASK_STATS            0
#endif

/* ============================================================================
   Compile-Time Validation
   ============================================================================ */
#if (TASK_STACK_ALLOC_MODE != TASK_ALLOC_STATIC) && (TASK_STACK_ALLOC_MODE != TASK_ALLOC_DYNAMIC)
    #error "TASK_STACK_ALLOC_MODE must be either TASK_ALLOC_STATIC or TASK_ALLOC_DYNAMIC"
#endif

/* Verify stack size is reasonable */
#if STACK_SIZE_IN_WORDS < 64
    #warning "Stack size very small - may cause overflow"
#endif

#if STACK_SIZE_IN_WORDS > 1024
    #warning "Stack size very large - may waste memory"
#endif

/* Verify MAX_TASKS is reasonable */
#if MAX_TASKS < 2
    #error "MAX_TASKS must be at least 2 (for idle + 1 user task)"
#endif

#if (TASK_STACK_ALLOC_MODE == TASK_ALLOC_STATIC) && (MAX_TASKS > 58)
    #warning "With static allocation, many tasks will consume a lot of .bss memory"
#endif

#if (TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC) && (MAX_TASKS * STACK_SIZE_IN_WORDS * 4 > 90000)
    #warning "Total stack size exceeds available heap - not all tasks can be created"
#endif

#endif /* PROJECT_CONFIG_H */