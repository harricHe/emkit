#include "command.h"

handle_t command_create(void *memory, size_t size)
{
	return NULL;
}

error_t command_destroy(handle_t hdl)
{
	return -1;
}

error_t command_add(handle_t hdl, const char *cmd, commandfunc_t func)
{
	return -1;
}

error_t command_remove(handle_t hdl, const char *cmd)
{
	return -1;
}

error_t command_execute(handle_t hdl, int argc, const char **argv)
{
	return -1;
}

error_t command_execute_line(handle_t hdl, const char *line)
{
	return -1;
}

