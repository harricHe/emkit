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

/* blocking I/O */
typedef error_t (*putc_func_t)(char c);
typedef char (*getc_func_t)(void);
typedef error_t (*shexe_func_t)(char *line);

handle_t shell_create(void *memory, size_t size,
		putc_func_t putc,
		getc_func_t getc,
		shexe_func_t exec);
error_t shell_destroy(handle_t hdl);
error_t shell_start(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SHELL_H__ */


