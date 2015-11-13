#include "ringbuffer.h"
#include <string.h>

#define RINGBUFFER_SIGNATURE PACK('r','b','u','f')

typedef struct {
	uint32_t signeture;
	uint8_t *buffer_start;
	uint8_t *buffer_end;
	uint8_t *rp;
	uint8_t *wp;
	size_t  capacity;
	size_t  used;
} ringbuf_t;


static ringbuf_t* get_object(void)
{
	static ringbuf_t objs[CONFIG_NUMOF_RINGBUFFERS];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_RINGBUFFERS; i++) {
		if (objs[i].signeture == NULL_SIGNATURE) {
			return &objs[i];
		}
	}
	return NULL;
}


static inline uint8_t* step_pointer(const ringbuf_t *base, const uint8_t *p, size_t len)
{
	uint8_t *ep = (uint8_t*)(p + len);
	return (ep > base->buffer_end)
		? base->buffer_start + (ep - base->buffer_end)
		: ep;
}


handle_t ringbuf_create(void *memory, size_t size)
{
	ringbuf_t* base;
	if (!memory) return NULL;
	if (!size)   return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signeture = RINGBUFFER_SIGNATURE;
	base->buffer_start = memory;
	base->buffer_end = base->buffer_start + size;
	base->rp = base->buffer_start;
	base->wp = base->buffer_start;
	base->capacity = size;
	base->used = 0;

	return (handle_t)base;
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

