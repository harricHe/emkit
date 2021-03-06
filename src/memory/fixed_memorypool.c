#include "fixed_memorypool.h"
#include <string.h>

#define FIXED_MEMORYPOOL_SIGNATURE PACK('f','m','p','l')

typedef struct block {
	struct blockheader {
		bool_t using;
		struct block *next_free_blk;
	} header;
	uint8_t data[];
} block_t;
typedef struct blockheader blockheader_t;

typedef struct {
	uint32_t signature;
	block_t *free;
	size_t  ava_blks;
	size_t  blksz;
	size_t  blkcnt;
} base_t;


static base_t* get_object(void)
{
	static base_t objs[CONFIG_NUMOF_FIXEDMEMORYPOOL_HANDLES];
	static bool_t has_inited = 0;
	int32_t i;
	if (!has_inited) {
		memset(objs, 0, sizeof(objs));
		has_inited = 1;
	}
	for (i=0; i<CONFIG_NUMOF_FIXEDMEMORYPOOL_HANDLES; i++) {
		if (objs[i].signature == NULL_SIGNATURE)
			return &objs[i];
	}
	return NULL;
}


static void initialize_blocks(base_t *base, void *memory, size_t blksz, size_t blkcnt)
{
	block_t *blk;
	uint8_t *p;
	int32_t i, end = blkcnt -1;

	base->ava_blks = blkcnt;
	base->blksz  = blksz;
	base->blkcnt = blkcnt;
	base->free   = memory;
	p = (uint8_t*)base->free;

	for (i=0; i<end; i++) {
		blk = (block_t*)p;
		p  += blksz;
		blk->header.using = FALSE;
		blk->header.next_free_blk = (block_t*)p;
	}
	blk = (block_t*)p;
	blk->header.next_free_blk = NULL;
}


handle_t fixedmpool_create(void *memory, size_t blksz, size_t blkcnt)
{
	base_t *base;
	if (!memory) return NULL;
	if (!blkcnt) return NULL;
	if (blksz <= sizeof(blockheader_t)) return NULL;

	base = get_object();
	if (!base) return NULL;

	base->signature = FIXED_MEMORYPOOL_SIGNATURE;
	initialize_blocks(base, memory, blksz, blkcnt);

	return (handle_t)base;
}


error_t fixedmpool_destroy(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != FIXED_MEMORYPOOL_SIGNATURE)
		return -1;

	base->signature = NULL_SIGNATURE;
	return 0;
}


static void* allocate_block(base_t *base)
{
	block_t *blk;
	if (!base->free) {
		return NULL;
	}
	blk = base->free;
	base->free = blk->header.next_free_blk;
	blk->header.using = TRUE;
	blk->header.next_free_blk = NULL;
	base->ava_blks--;
	return (void*)blk->data;
}


static error_t free_block(base_t *base, void *data)
{
	block_t *blk = (block_t*)((uint8_t*)data - sizeof(blockheader_t));
	if (!blk->header.using)
		return -1;

	blk->header.using = FALSE;
	blk->header.next_free_blk = base->free;
	base->free = blk;
	base->ava_blks++;
	return 0;
}


void* fixedmpool_allocate(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return NULL;
	if (base->signature != FIXED_MEMORYPOOL_SIGNATURE)
		return NULL;

	return allocate_block(base);
}


error_t fixedmpool_free(handle_t hdl, void *block)
{
	base_t *base = (base_t*)hdl;
	if (!base) return -1;
	if (base->signature != FIXED_MEMORYPOOL_SIGNATURE)
		return -1;

	return free_block(base, block);
}


size_t fixedmpool_available_blocks(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (base->signature != FIXED_MEMORYPOOL_SIGNATURE)
		return 0;

	return base->ava_blks;
}


size_t fixedmpool_used_blocks(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (base->signature != FIXED_MEMORYPOOL_SIGNATURE)
		return 0;

	return base->blkcnt - base->ava_blks;
}


size_t fixedmpool_blockdata_size(handle_t hdl)
{
	base_t *base = (base_t*)hdl;
	if (!base) return 0;
	if (base->signature != FIXED_MEMORYPOOL_SIGNATURE)
		return 0;

	return base->blksz - sizeof(blockheader_t);
}

