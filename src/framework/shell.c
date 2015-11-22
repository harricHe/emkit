#include "shell.h"
#include <string.h>

#define SHELL_SIGNATURE PACK('s','h','e','l')

typedef struct {
	uint32_t signature;
	putc_func_t  putc_func;
	getc_func_t  getc_func;
	shexe_func_t exec_func;
	void  *line;
	size_t len;
} base_t;


static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_SHELL_HANDLES];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_SHELL_HANDLES; i++) {
		if (objs[i].signature == NULL_SIGNATURE)
			return &objs[i];
	}
	return NULL;
}



handle_t shell_create(void *memory, size_t size,
		putc_func_t putc,
		getc_func_t getc,
		shexe_func_t exec)
{
	base_t *base;
	if (!memory) return NULL;
	if (!size) return NULL;
	if (!putc) return NULL;
	if (!getc) return NULL;
	if (!exec) return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signature = SHELL_SIGNATURE;
	base->putc_func = putc;
	base->getc_func = getc;
	base->exec_func = exec;
	base->line = memory;
	base->len  = size;

	return (handle_t)base;
}


error_t shell_destroy(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != SHELL_SIGNATURE)
		return -1;

	base->signature = NULL_SIGNATURE;
	return 0;
}


error_t shell_start(handle_t hdl)
{
	return -1;
}

