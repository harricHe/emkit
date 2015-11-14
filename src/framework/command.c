#include "command.h"
#include <string.h>

#define COMMAND_SIGNATURE PACK('c','m','n','d')

typedef struct command {
	char name[COMMAND_NAME_SIZE];
	commandfunc_t func;
} command_t;

typedef struct {
	uint32_t signature;
	command_t *cmdlist;
	size_t cmdnum;
	size_t cmdcap;
} base_t;

// size check when pre-compile
extern int dummy[(COMMAND_UNIT_SIZE == sizeof(command_t)) ? 1 : -1];

static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_COMMAND_HANDLES];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_COMMAND_HANDLES; i++) {
		if (objs[i].signature == NULL_SIGNATURE)
			return &objs[i];
	}
	return NULL;
}


static void initialize_handle(base_t *base, void *memory, size_t size)
{
	memset(memory, 0, size);
	base->cmdlist = memory;
	base->cmdnum  = 0;
	base->cmdcap  = size / sizeof(command_t);
}


handle_t command_create(void *memory, size_t size)
{
	base_t *base;
	if (!memory) return NULL;
	if (!size) return NULL;
	if (size <= sizeof(command_t)) return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signature = COMMAND_SIGNATURE;
	initialize_handle(base, memory, size);

	return (handle_t)base;
}

error_t command_destroy(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != COMMAND_SIGNATURE)
		return -1;

	base->signature = NULL_SIGNATURE;
	return 0;
}

static error_t cmd_add(base_t *base, const char *cmdname, commandfunc_t func)
{
	int32_t i;
	command_t *cmd = base->cmdlist;

	for (i=0; i<(int32_t)base->cmdcap; i++) {
		if (!strcmp(cmd[i].name, cmdname)) {
			// already added
			return -1;
		}
	}

	for (i=0; i<(int32_t)base->cmdcap; i++) {
		if (!cmd[i].func) {
			strncpy(cmd[i].name, cmdname, COMMAND_NAME_SIZE);
			cmd[i].func = func;
			base->cmdnum++;
			return 0;
		}
	}

	return -1;
}

error_t command_add(handle_t hdl, const char *cmdname, commandfunc_t func)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (!cmdname)  return -1;
	if (!*cmdname) return -1;
	if (!func) return -1;
	if (base->cmdnum == base->cmdcap)
		return -1;
	if (base->signature != COMMAND_SIGNATURE)
		return -1;

	return cmd_add(base, cmdname, func);
}

static error_t cmd_remove(base_t *base, const char *cmdname)
{
	int32_t i;
	command_t *cmd = base->cmdlist;

	for (i=0; i<(int32_t)base->cmdcap; i++) {
		if (!strcmp(cmd[i].name, cmdname)) {
			cmd[i].name[0] = 0;
			cmd[i].func = NULL;
			base->cmdnum--;
			return 0;
		}
	}

	return -1;
}

error_t command_remove(handle_t hdl, const char *cmdname)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (!cmdname) return -1;
	if (!*cmdname) return -1;
	if (base->cmdnum == 0)
		return -1;
	if (base->signature != COMMAND_SIGNATURE)
		return -1;

	return cmd_remove(base, cmdname);
}

static error_t cmd_execute(base_t *base, int32_t argc, const char **argv)
{
	int32_t i;
	command_t *cmd = base->cmdlist;

	for (i=0; i<(int32_t)base->cmdcap; i++) {
		if (!strcmp(cmd[i].name, argv[0])) {
			if (!cmd[i].func) {
				return -1;
			}
			return cmd[i].func(argc, argv);
		}
	}

	return -1;
}

error_t command_execute(handle_t hdl, int32_t argc, const char **argv)
{
	base_t *base = (base_t*)hdl;
	if (!argc) return -1;
	if (!argv) return -1;
	if (base->signature != COMMAND_SIGNATURE)
		return -1;

	return cmd_execute(base, argc, argv);
}

