#include "app_commands.h"
#include "cli.h"
#include "scheduler.h"
#include "heap.h"
#include <string.h>

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
#include "utils.h"  /* For string to number conversion */
#endif

/* Forward declarations */
static int cmd_heap_stats_handler(int argc, char **argv);
static int cmd_task_list_handler(int argc, char **argv);
#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
static int cmd_heap_test_handler(int argc, char **argv);
#endif

/* Command definitions */
static const cli_command_t heap_stats_cmd = {
    .name = "heap",
    .help = "Show heap statistics (dynamic mode only)",
    .handler = cmd_heap_stats_handler
};

static const cli_command_t task_list_cmd = {
    .name = "tasks",
    .help = "List all tasks",
    .handler = cmd_task_list_handler
};

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
static const cli_command_t heap_test_cmd = {
    .name = "heaptest",
    .help = "Test heap allocator: heaptest <size>",
    .handler = cmd_heap_test_handler
};
#endif

/* Command handlers */

static int cmd_heap_stats_handler(int argc, char **argv)
{
    (void)argc;
    (void)argv;

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
    heap_stats_t stats;
    if (heap_get_stats(&stats) == 0) {
        cli_printf("Heap Statistics:\r\n");
        cli_printf("  Total size:     %u bytes\r\n", (unsigned int)stats.total_size);
        cli_printf("  Used:           %u bytes\r\n", (unsigned int)stats.used_size);
        cli_printf("  Free:           %u bytes\r\n", (unsigned int)stats.free_size);
        cli_printf("  Largest block:  %u bytes\r\n", (unsigned int)stats.largest_free_block);
        cli_printf("  Allocated blocks: %u\r\n", (unsigned int)stats.block_count);
        cli_printf("  Free fragments:   %u\r\n", (unsigned int)stats.fragment_count);
        
        if (stats.total_size > 0) {
            unsigned int percent = (stats.used_size * 100) / stats.total_size;
            cli_printf("  Usage:           %u%%\r\n", percent);
        }
        
        /* Check integrity */
        if (heap_check_integrity() == 0) {
            cli_printf("  Status:          OK\r\n");
        } else {
            cli_printf("  Status:          CORRUPTED!\r\n");
        }
    } else {
        cli_printf("Heap not initialized\r\n");
    }
#else
    cli_printf("Heap statistics only available in dynamic allocation mode\r\n");
    cli_printf("Current mode: STATIC (stacks embedded in task_list[])\r\n");
#endif

    return 0;
}

static int cmd_task_list_handler(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    extern task_t task_list[MAX_TASKS];

    cli_printf("Task List:\r\n");
    cli_printf("ID   State      Stack Location\r\n");
    cli_printf("---  ---------  --------------\r\n");

    /* Count active tasks */
    uint32_t count = 0;
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        if (task_list[i].state != TASK_UNUSED) {
            const char *state_str;
            switch (task_list[i].state) {
                case TASK_READY:    state_str = "READY"; break;
                case TASK_RUNNING:  state_str = "RUNNING"; break;
                case TASK_BLOCKED:  state_str = "BLOCKED"; break;
                default:            state_str = "UNKNOWN"; break;
            }

            cli_printf("%-3u  %-9s  ", task_list[i].task_id, state_str);

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_STATIC
            cli_printf("Static (embedded)\r\n");
#else
            if (task_list[i].stack_ptr != NULL) {
                cli_printf("0x%08x (heap)\r\n", (unsigned int)task_list[i].stack_ptr);
            } else {
                cli_printf("NULL\r\n");
            }
#endif
            count++;
        }
    }

    cli_printf("\r\nTotal tasks: %u\r\n", (unsigned int)count);

    return 0;
}

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
/* Simple string to unsigned int converter */
static unsigned int str_to_uint(const char *str)
{
    unsigned int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

static int cmd_heap_test_handler(int argc, char **argv)
{
    if (argc < 2) {
        cli_printf("Usage: heaptest <size>\r\n");
        cli_printf("  Allocates and frees memory of specified size\r\n");
        return -1;
    }

    unsigned int size = str_to_uint(argv[1]);
    if (size == 0) {
        cli_printf("Invalid size: %s\r\n", argv[1]);
        return -1;
    }

    cli_printf("Testing heap with %u bytes...\r\n", size);

    void *ptr = heap_malloc(size);
    if (ptr == NULL) {
        cli_printf("Allocation FAILED\r\n");
        return -1;
    }

    cli_printf("Allocated at: 0x%08x\r\n", (unsigned int)ptr);

    heap_stats_t stats;
    if (heap_get_stats(&stats) == 0) {
        cli_printf("Free after alloc: %u bytes\r\n", (unsigned int)stats.free_size);
    }

    heap_free(ptr);
    cli_printf("Freed successfully\r\n");

    if (heap_get_stats(&stats) == 0) {
        cli_printf("Free after free:  %u bytes\r\n", (unsigned int)stats.free_size);
    }

    return 0;
}
#endif

/* Register all commands */
void app_commands_register_all(void)
{
    cli_register_command(&heap_stats_cmd);
    cli_register_command(&task_list_cmd);
#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
    cli_register_command(&heap_test_cmd);
#endif
}

