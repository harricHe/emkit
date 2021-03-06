/* This framework's responsibility is following.
 * - echoback support
 * - backspace support
 * - command history suppoort
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_SHELL_HANDLES (1)
#define SHELL_PROMPT_LEN (4)

typedef error_t (*shexe_func_t)(const char *line);
typedef void (*post_hook_t)(error_t err, const char *line);

handle_t shell_create(void *memory, size_t size,
		putc_func_t putc,
		getc_func_t getc,
		shexe_func_t exec,
		post_hook_t hook);

error_t shell_destroy(handle_t hdl);
error_t shell_set_prompt(handle_t hdl, const char *prompt);
error_t shell_start(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SHELL_H__ */


