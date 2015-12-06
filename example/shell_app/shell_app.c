#include "shell.h"
#include "text.h"
#include "command.h"
#include <stdio.h>
#include <string.h>

static handle_t cmd_hdl;
static char shell_work_memory[128];
static char command_work_memory[128];

static error_t myputc(char c) {
	/* FIXME */
	return 0;
}

static char mygetc(void) {
	/* FIXME */
	return 0;
}

static error_t myexecute(const char *line) {
	char workline[128];
	char *argv[8];
	int32_t argc = 0;
	char *p;
	const char separators[] = " ";
	size_t len;

	strncpy(workline, line, sizeof(workline));
	p = argv[0] = workline;
	len = strlen(line);

	for (argc=1; argc<8; ) {
		argv[argc++] = p = text_separate(p, len - (p - workline), separators);
		if (!p) {
			break;
		}
	}

	return command_execute(cmd_hdl, argc, (const char**)argv);
}

static void myhook(error_t err, const char *line) {
}


static error_t cmd_hello(int32_t argc, const char **argv) {
	/* FIXME */
	return 0;
}


static error_t cmd_goodbye(int32_t argc, const char **argv) {
	/* FIXME */
	return 0;
}


int main(int argc, char **argv) {
	handle_t sh_hdl;

	command_init(myputc);
	sh_hdl = shell_create(shell_work_memory, sizeof(shell_work_memory),
			myputc, mygetc, myexecute, myhook);
	if (!sh_hdl) {
		fprintf(stderr, "Shell Handle is NULL!\n");
		return -1;
	}
	cmd_hdl = command_create(command_work_memory, sizeof(command_work_memory));
	if (!cmd_hdl) {
		fprintf(stderr, "Command Handle is NULL!\n");
		return -1;
	}

	command_add(cmd_hdl, "hello", cmd_hello, NULL);
	command_add(cmd_hdl, "goodbye", cmd_goodbye, NULL);

	shell_start(sh_hdl);

	return 0;
}


