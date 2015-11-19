#include "priority_queue.h"

handle_t priqueue_create(void *memory, uint32_t pricnt, size_t qsz, size_t qcnt)
{
	return NULL;
}

error_t priqueue_destroy(handle_t hdl)
{
	return -1;
}

error_t priqueue_write(handle_t hdl, uint32_t pri, const void *data)
{
	return -1;
}

error_t priqueue_read(handle_t hdl, void *data)
{
	return -1;
}

error_t priqueue_read_with_priority(handle_t hdl, uint32_t pri, void *data)
{
	return -1;
}

void* priqueue_get(handle_t hdl)
{
	return NULL;
}

void* priqueue_get_with_priority(handle_t hdl, uint32_t pri)
{
	return NULL;
}

size_t priqueue_available(handle_t hdl, uint32_t pri)
{
	return 0;
}

size_t priqueue_used(handle_t hdl, uint32_t pri)
{
	return 0;
}

size_t priqueue_capacity(handle_t hdl, uint32_t pri)
{
	return 0;
}

error_t priqueue_purge(handle_t hdl)
{
	return -1;
}


