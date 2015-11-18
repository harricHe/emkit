#include "queue.h"
#include <string.h>

#define QUEUE_SIGNATURE PACK('q','u','e','u')

typedef struct {
	uint32_t signeture;
	uint8_t *buffer_start;
	uint8_t *buffer_end;
	uint8_t *qp;
	size_t  qsz;
	size_t  qcnt;
	size_t  used;
} base_t;

static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_QUEUE_HANDLES];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_QUEUE_HANDLES; i++) {
		if (objs[i].signeture == NULL_SIGNATURE) {
			return &objs[i];
		}
	}
	return NULL;
}


handle_t queue_create(void *memory, size_t qsz, size_t qcnt)
{
	base_t *base;
	if (!memory) return NULL;
	if (!qsz)  return NULL;
	if (!qcnt) return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signeture = QUEUE_SIGNATURE;
	base->buffer_start = memory;
	base->buffer_end   = base->buffer_start + (qsz * qcnt);
	base->qp = base->buffer_start;
	base->qsz  = qsz;
	base->qcnt = qcnt;
	base->used = 0;

	return (handle_t)base;
}

error_t queue_destroy(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signeture != QUEUE_SIGNATURE)
		return -1;

	base->signeture = NULL_SIGNATURE;
	return 0;
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


