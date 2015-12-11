#include "memorypool.h"
#include <string.h>

#define MEMORYPOOL_SIGNATURE PACK('m','e','m','p')

typedef struct memhead {
	struct memhead *prev;
	struct memhead *next;
	size_t  size;
	uint8_t area[];
} memhead_t;

typedef struct {
	uint32_t signature;
	uint8_t *pool;
	size_t  poolsz;
} base_t;

static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_MEMORYPOOLS];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_MEMORYPOOLS; i++) {
		if (objs[i].signature == NULL_SIGNATURE)
			return &objs[i];
	}
	return NULL;
}

handle_t mempool_create(void *memory, size_t memsz)
{
	base_t *base;
	if (!memory) return NULL;
	if (!memsz) return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signature = MEMORYPOOL_SIGNATURE;

	return (handle_t)base;
}

error_t mempool_destroy(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != MEMORYPOOL_SIGNATURE)
		return -1;

	base->signature = NULL_SIGNATURE;
	return 0;
}

void* mempool_allocate(handle_t hdl, size_t size)
{
	return NULL;
}

error_t mempool_free(handle_t hdl, void *block)
{
	return -1;
}

size_t mempool_available(handle_t hdl)
{
	return 0;
}

size_t mempool_used(handle_t hdl)
{
	return 0;
}

size_t mempool_capacity(handle_t hdl)
{
	return 0;
}


