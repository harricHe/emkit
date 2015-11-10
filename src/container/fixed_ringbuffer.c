#include "fixed_ringbuffer.h"
#include <string.h>

#define FIXED_RINGBUFFER_SIGNATURE PACK('f','r','b','f')

typedef struct {
	uint32_t signeture;
	uint8_t *buffer_start;
	uint8_t *buffer_end;
	uint8_t *rp;
	uint8_t *wp;
	size_t  blksz;
	size_t  blkcnt;
	size_t  used;
} fixringbuf_t;


static fixringbuf_t* get_object(void)
{
	static fixringbuf_t objs[CONFIG_NUMOF_FIXEDRINGBUFFERS];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_FIXEDRINGBUFFERS; i++) {
		if (objs[i].signeture == NULL_SIGNATURE) {
			return &objs[i];
		}
	}
	return NULL;
}


static inline uint8_t* step_pointer(const fixringbuf_t *base, const uint8_t *p)
{
	p += base->blksz;
	if (p >= base->buffer_end) {
		p = base->buffer_start;
	}
	return (uint8_t*)p;
}

handle_t fixedringbuf_create(void *memory, size_t blksz, size_t blkcnt)
{
	fixringbuf_t *base;
	if (!memory) return NULL;
	if (!blksz)  return NULL;
	if (!blkcnt) return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signeture = FIXED_RINGBUFFER_SIGNATURE;
	base->buffer_start = memory;
	base->buffer_end   = (uint8_t*)base->buffer_start + (blksz * blkcnt);
	base->rp = base->buffer_start;
	base->wp = base->buffer_start;
	base->blksz  = blksz;
	base->blkcnt = blkcnt;
	base->used   = 0;

	return (handle_t)base;
}


error_t fixedringbuf_destroy(handle_t hdl)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base) return -1;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return -1;

	base->signeture = NULL_SIGNATURE;
	return 0;
}


error_t fixedringbuf_write(handle_t hdl, const void *block)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base)  return -1;
	if (!block) return -1;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return -1;

	memcpy(base->wp, block, base->blksz);
	base->wp = step_pointer(base, base->wp);
	base->used++;
	if (base->used > base->blkcnt) {
		/* over flow */
		base->rp = base->wp;
		base->used--;
	}
	return 0;
}


error_t fixedringbuf_read(handle_t hdl, void *block)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base)  return -1;
	if (!block) return -1;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return -1;
	if (!base->used) return -1;
	memcpy(block, base->rp, base->blksz);
	base->rp = step_pointer(base, base->rp);
	base->used--;
	return 0;
}


void* fixedringbuf_get(handle_t hdl)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	void *p;
	if (!base) return NULL;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return NULL;
	if (!base->used) return NULL;
	p = base->rp;
	base->rp = step_pointer(base, base->rp);
	base->used--;
	return p;
}


size_t fixedringbuf_available(handle_t hdl)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base) return 0;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return 0;
	return base->blkcnt - base->used;
}


size_t fixedringbuf_used(handle_t hdl)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base) return 0;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return 0;
	return base->used;
}


size_t fixedringbuf_capacity(handle_t hdl)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base) return 0;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return 0;
	return base->blkcnt;
}


error_t fixedringbuf_purge(handle_t hdl)
{
	fixringbuf_t *base = (fixringbuf_t*)hdl;
	if (!base) return -1;
	if (base->signeture != FIXED_RINGBUFFER_SIGNATURE)
		return -1;
	base->rp = base->buffer_start;
	base->wp = base->buffer_start;
	base->used = 0;
	return 0;
}

