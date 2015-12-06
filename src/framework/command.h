#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_COMMAND_HANDLES (2)
// TODO: this size depends platform
#define COMMAND_UNIT_SIZE (24)

typedef error_t (*command_func_t)(int32_t argc, const char **argv);

error_t command_init(putc_func_t putc);
handle_t command_create(void *memory, size_t size);
error_t command_destroy(handle_t hdl);
error_t command_add(handle_t hdl, const char *cmd, command_func_t func, const char *description);
error_t command_remove(handle_t hdl, const char *cmd);
error_t command_execute(handle_t hdl, int32_t argc, const char **argv);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMAND_H__ */

