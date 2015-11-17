#include "queue.h"

handle_t queue_create(void *memory, size_t qsz, size_t qcnt)
{
	return NULL;
}

error_t queue_destroy(handle_t hdl)
{
	return -1;
}

error_t queue_write(handle_t hdl, const void *data)
{
	return -1;
}

error_t queue_read(handle_t hdl, void *data)
{
	return -1;
}

void* queue_get(handle_t hdl)
{
	return NULL;
}

size_t queue_available(handle_t hdl)
{
	return 0;
}

size_t queue_used(handle_t hdl)
{
	return 0;
}

size_t queue_capacity(handle_t hdl)
{
	return 0;
}

error_t queue_purge(handle_t hdl)
{
	return -1;
}


