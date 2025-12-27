#include "app_commands.h"
#include "cli.h"
#include "scheduler.h"
#include "stm32_alloc.h"
#include <string.h>

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
#include "utils.h"  /* For string to number conversion */
#endif

/* Forward declarations */
static int cmd_heap_stats_handler(int argc, char **argv);
static int cmd_task_list_handler(int argc, char **argv);

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

/* Command handlers */
static int cmd_heap_stats_handler(int argc, char **argv)
{
    (void)argc;
    (void)argv;

#if TASK_STACK_ALLOC_MODE == TASK_ALLOC_DYNAMIC
    heap_stats_t stats;
    if (stm32_allocator_dump_stats(&stats) == 0) {
        cli_printf("Heap Statistics:\r\n");
        cli_printf("  Total size:     %u bytes\r\n", (unsigned int)stats.total_size);
        cli_printf("  Used:           %u bytes\r\n", (unsigned int)stats.used_size);
        cli_printf("  Free:           %u bytes\r\n", (unsigned int)stats.free_size);
        cli_printf("  Largest block:  %u bytes\r\n", (unsigned int)stats.largest_free_block);
        cli_printf("  Allocated blocks: %u\r\n", (unsigned int)stats.allocated_blocks);
        cli_printf("  Free fragments:   %u\r\n", (unsigned int)stats.free_blocks);
        
        if (stats.total_size > 0) {
            unsigned int percent = (stats.used_size * 100) / stats.total_size;
            cli_printf("  Usage:           %u%%\r\n", percent);
        }
        
        /* Check integrity */
        if (stm32_allocator_check_integrity() == 0) {
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
                case TASK_ZOMBIE:   state_str = "ZOMBIE"; break;
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

/* Register all commands */
void app_commands_register_all(void)
{
    cli_register_command(&heap_stats_cmd);
    cli_register_command(&task_list_cmd);
}

