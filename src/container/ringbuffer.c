#include "ringbuffer.h"

#define FIXED_RINGBUFFER_SIGNATURE PACK('r','b','u','f')

handle_t ringbuf_create(void *memory, size_t size)
{
	return NULL;
}

error_t ringbuf_destroy(handle_t hdl)
{
	return -1;
}

error_t ringbuf_write(handle_t hdl, const void *data, size_t size)
{
	return -1;
}

size_t ringbuf_read(handle_t hdl, void *data, size_t size)
{
	return 0;
}

size_t ringbuf_read_to(handle_t hdl, void *data, size_t size, uint8_t token)
{
	return 0;
}

size_t ringbuf_available(handle_t hdl)
{
	return 0;
}

size_t ringbuf_used(handle_t hdl)
{
	return 0;
}

size_t ringbuf_capacity(handle_t hdl)
{
	return 0;
}

error_t ringbuf_purge(handle_t hdl)
{
	return -1;
}

