#include "memorypool.h"

#define MEMORYPOOL_SIGNATURE PACK('m','e','m','p')

handle_t mempool_create(void *memory, size_t memsz)
{
	return NULL;
}

error_t mempool_destroy(handle_t hdl)
{
	return -1;
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


