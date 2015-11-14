#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_COMMAND_HANDLES (2)
#define COMMAND_UNIT_SIZE (16)
#define COMMAND_NAME_SIZE (8)

typedef error_t (*commandfunc_t)(int32_t argc, const char **argv);

handle_t command_create(void *memory, size_t size);
error_t command_destroy(handle_t hdl);
error_t command_add(handle_t hdl, const char *cmd, commandfunc_t func);
error_t command_remove(handle_t hdl, const char *cmd);
error_t command_execute(handle_t hdl, int32_t argc, const char **argv);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMAND_H__ */

