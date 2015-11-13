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
	ringbuf_t *base = (ringbuf_t*)hdl;
	if (!base) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;

	base->signeture = NULL_SIGNATURE;
	return 0;
}

error_t ringbuf_write(handle_t hdl, const void *data, size_t size)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	const uint8_t *p = (const uint8_t*)data;
	size_t taillen;
	if (!base) return -1;
	if (!data) return -1;
	if (!size) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;

	taillen = base->buffer_end - base->wp;
	if (taillen > size) {
		memcpy(base->wp, p, size);
		base->wp += size;
	} else {
		size_t headlen = size - taillen;
		memcpy(base->wp, p, taillen);
		memcpy(base->buffer_start, &p[taillen], headlen);
		base->wp = base->buffer_start + headlen;
	}

	if (size > (base->capacity - base->used)) {
		base->rp = base->wp;
		base->used = base->capacity;
	} else {
		base->used += size;
	}
	return 0;
}

size_t ringbuf_read(handle_t hdl, void *data, size_t size)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	uint8_t *p = (uint8_t*)data;
	size_t taillen;
	size_t asize;
	if (!base) return -1;
	if (!data) return -1;
	if (!size) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;

	taillen = base->buffer_end - base->rp;
	asize = ((base->capacity - base->used) > size)
		? size : (base->capacity - base->used);
	if (base->rp < base->wp) {
		memcpy(p, base->rp, asize);
		base->rp += asize;
	} else {
		size_t headlen = size - taillen;
		memcpy(p, base->rp, taillen);
		memcpy(&p[taillen], base->buffer_start, headlen);
		base->rp = base->buffer_start + headlen;
	}

	base->used -= asize;
	return 0;
}

size_t ringbuf_read_to(handle_t hdl, void *data, size_t size, uint8_t token)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	uint8_t *p;
	size_t asize;
	size_t headlen;
	size_t taillen;
	int32_t i;
	if (!base) return -1;
	if (!data) return -1;
	if (!size) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;

	asize = ((base->capacity - base->used) > size)
		? size : (base->capacity - base->used);

	taillen = base->buffer_end - base->rp; 
	p = base->rp;
	for (i=0; i<(int32_t)taillen; i++) {
		if (p[i] == token) {
			return ringbuf_read(hdl, data, i+1);
		}
		asize--;
		if (!asize) {
			/* not found */
			return ringbuf_read(hdl, data, size);
		}
	}

	headlen = size - taillen;
	p = base->buffer_start;
	for (i=0; i<(int32_t)headlen; i++) {
		if (p[i] == token) {
			return ringbuf_read(hdl, data, taillen+i+1);
		}
		asize--;
		if (asize) {
			return ringbuf_read(hdl, data, size);
		}
	}

	return 0;
}

size_t ringbuf_available(handle_t hdl)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	if (!base) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;
	return base->capacity - base->used;
}

size_t ringbuf_used(handle_t hdl)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	if (!base) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;
	return base->used;
}

size_t ringbuf_capacity(handle_t hdl)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	if (!base) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;
	return base->capacity;
}

error_t ringbuf_purge(handle_t hdl)
{
	ringbuf_t *base = (ringbuf_t*)hdl;
	if (!base) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;
	base->rp = base->buffer_start;
	base->wp = base->buffer_start;
	base->used = 0;
	return 0;
}

