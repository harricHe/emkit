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
} base_t;


static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_RINGBUFFER_HANDLES];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_RINGBUFFER_HANDLES; i++) {
		if (objs[i].signeture == NULL_SIGNATURE) {
			return &objs[i];
		}
	}
	return NULL;
}

handle_t ringbuf_create(void *memory, size_t size)
{
	base_t* base;
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
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;

	base->signeture = NULL_SIGNATURE;
	return 0;
}

static error_t rb_write(base_t *base, const void *data, size_t size)
{
	size_t taillen = base->buffer_end - base->wp;
	const uint8_t *p = (const uint8_t*)data;

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
	return size;
}

size_t ringbuf_write(handle_t hdl, const void *data, size_t size)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (!data) return 0;
	if (!size) return 0;
	if (!size > base->capacity) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;

	return rb_write(base, data, size);
}

static size_t rb_read(base_t *base, void *data, size_t size)
{
	uint8_t *p = (uint8_t*)data;
	size_t taillen;
	size_t asize;

	taillen = base->buffer_end - base->rp;
	asize = (base->used > size) ? size : base->used;
	if (base->rp < base->wp) {
		memcpy(p, base->rp, asize);
		base->rp += asize;
	} else {
		size_t headlen = asize - taillen;
		memcpy(p, base->rp, taillen);
		memcpy(&p[taillen], base->buffer_start, headlen);
		base->rp = base->buffer_start + headlen;
	}

	base->used -= asize;
	return asize;
}

size_t ringbuf_read(handle_t hdl, void *data, size_t size)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (!data) return 0;
	if (!size) return 0;
	if (!size > base->capacity) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;

	return rb_read(base, data, size);
}

size_t ringbuf_read_to(handle_t hdl, void *data, size_t size, uint8_t token)
{
	base_t *base = (base_t*)hdl;
	uint8_t *p;
	size_t asize;
	size_t headlen;
	size_t taillen;
	if (!base) return 0;
	if (!data) return 0;
	if (!size) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;

	asize = (base->used > size) ? size : base->used;

	taillen = base->buffer_end - base->rp;
	if (taillen > asize) {
		taillen = asize;
	}
	headlen = asize - taillen;

	if (taillen) {
		p = memchr(base->rp, token, taillen);
		if (p) {
			return rb_read(base, data, (p - base->rp) + 1);
		}
	}

	if (headlen) {
		p = memchr(base->buffer_start, token, headlen);
		if (p) {
			return rb_read(base, data, (p - base->buffer_start) + 1);
		}
	}

	/* not found */
	return rb_read(base, data, size);
}

size_t ringbuf_available(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;
	return base->capacity - base->used;
}

size_t ringbuf_used(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;
	return base->used;
}

size_t ringbuf_capacity(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return 0;
	return base->capacity;
}

error_t ringbuf_purge(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signeture != RINGBUFFER_SIGNATURE)
		return -1;
	base->rp = base->buffer_start;
	base->wp = base->buffer_start;
	base->used = 0;
	return 0;
}

