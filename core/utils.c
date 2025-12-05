#include "utils.h"



/*********** Function definitions ***********/
int wait_for_flag_set(volatile uint32_t *reg, uint32_t mask, uint32_t max_iter)
{
    uint32_t i = max_iter;
    while(((*reg) & mask) == 0U) {
        if (i-- == 0U) return -1;
    }
    return 0;
}

int wait_for_flag_clear(volatile uint32_t *reg, uint32_t mask, uint32_t max_iter)
{
    uint32_t i = max_iter;
    while(((*reg) & mask) != 0U) {
        if (i-- == 0U) return -1;
    }
    return 0;
}

int wait_for_reg_mask_eq(volatile uint32_t *reg, uint32_t mask, uint32_t expected, uint32_t max_iter)
{
    uint32_t i = max_iter;
    while(((*reg) & mask) != expected) {
        if (i-- == 0U) return -1;
    }
    return 0;
}
