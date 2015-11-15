#ifndef __STACK_H__
#define __STACK_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_STACK_HANDLES (2)

handle_t stack_create(void *memory, size_t unitsz, size_t cnt);
error_t stack_destroy(handle_t hdl);
error_t stack_push(handle_t hdl, const void *data);
void*  stack_pop(handle_t hdl, void *data);
size_t stack_available(handle_t hdl);
size_t stack_used(handle_t hdl);
size_t stack_capacity(handle_t hdl);
error_t stack_purge(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STACK_H__ */


