#include "shell.h"
#include <string.h>

#define SHELL_SIGNATURE PACK('s','h','e','l')
#define ASCII_BS 0x08
#define SHELL_DEFAULT_PROMPT "$>"

typedef struct {
	uint32_t signature;
	char prompt[SHELL_PROMPT_LEN];
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
	strncpy(base->prompt, SHELL_DEFAULT_PROMPT, SHELL_PROMPT_LEN-1);
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

error_t shell_set_prompt(handle_t hdl, const char *prompt)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != SHELL_SIGNATURE)
		return -1;
	if (!prompt) return -1;
	strncpy(base->prompt, prompt, SHELL_PROMPT_LEN-1);
	return 0;
}


static size_t write_line(const base_t *base, const char *msg) {
	const char *p = msg;
	if (base->putc_func) {
		while(*p) {
			base->putc_func(*(p++));
		}
	}
	return 0;
}

#if 0
static void echoback(const base_t *base, char c) {
	if (base->putc_func) {
		base->putc_func(c);
	}
}

static void backspace(const base_t *base)
{
	echoback(base, ASCII_BS);
	echoback(base, ' ');
	echoback(base, ASCII_BS);
}
#endif

static size_t read_line(const base_t *base)
{
	return 0;
}


error_t shell_start(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != SHELL_SIGNATURE)
		return -1;

	while (1) {
		size_t rlen;

		if (base->signature == NULL_SIGNATURE) {
			break;
		}

		write_line(base, base->prompt);
		rlen = read_line(base);
		if (rlen == 0) {
			continue;
		}
	}

	return -1;
}

