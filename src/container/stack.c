#include "stack.h"
#include <string.h>

#define STACK_SIGNATURE PACK('s','t','a','k')

typedef struct {
	uint32_t signeture;
	uint8_t *buffer_start;
	uint8_t *buffer_end;
	uint8_t *sp;
	size_t  unitsz;
} base_t;


static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_STACK_HANDLES];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_STACK_HANDLES; i++) {
		if (objs[i].signeture == NULL_SIGNATURE) {
			return &objs[i];
		}
	}
	return NULL;
}


handle_t stack_create(void *memory, size_t unitsz, size_t cnt)
{
	base_t *base;
	if (!memory) return NULL;
	if (!unitsz) return NULL;
	if (!cnt)    return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signeture = STACK_SIGNATURE;
	base->buffer_start = memory;
	base->buffer_end   = base->buffer_start + (unitsz * cnt);
	base->sp = base->buffer_start;
	base->unitsz = unitsz;

	return (handle_t)base;
}

error_t stack_destroy(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signeture != STACK_SIGNATURE)
		return -1;

	base->signeture = NULL_SIGNATURE;
	return 0;
}

error_t stack_push(handle_t hdl, const void *data)
{
	return -1;
}

void* stack_pop(handle_t hdl, void *data)
{
	return NULL;
}

size_t stack_available(handle_t hdl)
{
	return 0;
}

size_t stack_used(handle_t hdl)
{
	return 0;
}

size_t stack_capacity(handle_t hdl)
{
	return 0;
}

error_t stack_purge(handle_t hdl)
{
	return -1;
}

