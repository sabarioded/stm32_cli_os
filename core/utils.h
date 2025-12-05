#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int wait_for_flag_set(volatile uint32_t *reg, uint32_t mask, uint32_t max_iter);
int wait_for_flag_clear(volatile uint32_t *reg, uint32_t mask, uint32_t max_iter);
int wait_for_reg_mask_eq(volatile uint32_t *reg, uint32_t mask, uint32_t expected, uint32_t max_iter);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */