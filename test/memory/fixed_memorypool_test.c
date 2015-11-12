#include "unity_fixture.h"
#include "fixed_memorypool.h"

TEST_GROUP(fixed_memorypool);

#define BLOCK_SIZE  (24)
#define BLOCK_COUNT (4)
#define POOL_SIZE   ((BLOCK_SIZE) * (BLOCK_COUNT))
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];
static size_t s_blocksize = BLOCK_SIZE;
static size_t s_blockcount = BLOCK_COUNT;

TEST_SETUP(fixed_memorypool)
{
}

TEST_TEAR_DOWN(fixed_memorypool)
{
	fixedmpool_destroy(s_handle);
}

TEST(fixed_memorypool, create)
{
	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );
}

TEST(fixed_memorypool, destroy)
{
	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( fixedmpool_destroy(s_handle) );
	TEST_ASSERT_NULL( fixedmpool_allocate(s_handle) );
}

TEST(fixed_memorypool, blockdata_size)
{
	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT( fixedmpool_blockdata_size(s_handle) > 0 );
}

TEST(fixed_memorypool, allocate)
{
	uint8_t *p[BLOCK_COUNT];
	size_t datasize, headersize;
	int i, j;

	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	datasize = fixedmpool_blockdata_size(s_handle);
	headersize = s_blocksize - datasize;

	for (i=0; i<(int)s_blockcount; i++) {
		p[i] = fixedmpool_allocate(s_handle);
		TEST_ASSERT_NOT_NULL( p[i] );
	}
	TEST_ASSERT_NULL( fixedmpool_allocate(s_handle) );

	for (i=0; i<(int)s_blockcount; i++) {
		for (j=i+1; j<(int)s_blockcount; j++) {
			TEST_ASSERT_NOT_EQUAL( p[i], p[j] );
		}
	}

	for (i=0; i<(int)s_blockcount; i++) {
		uint8_t offset = (uint8_t)(i << 4);
		for (j=0; j<(int)datasize; j++) {
			p[i][j] = offset + (uint8_t)j;
		}
	}
	for (i=0; i<(int)s_blockcount; i++) {
		uint8_t offset = (uint8_t)(i << 4);
		for (j=0; j<(int)datasize; j++) {
			TEST_ASSERT_EQUAL_UINT8( offset + (uint8_t)j, s_pool[(s_blocksize*i) + (headersize+j)] );
		}
	}
}

TEST(fixed_memorypool, free)
{
	uint8_t *p[BLOCK_COUNT];
	uint8_t *q[BLOCK_COUNT];
	size_t datasize, headersize;
	int i;

	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	datasize = fixedmpool_blockdata_size(s_handle);
	headersize = s_blocksize - datasize;

	for (i=0; i<(int)s_blockcount; i++) {
		p[i] = q[i] = fixedmpool_allocate(s_handle);
		/* 
		 *  p[0] = q[0]
		 *  p[1] = q[1]
		 *  p[2] = q[2]
		 *  p[3] = q[3]
		 */
		TEST_ASSERT_NOT_NULL( p[i] );
	}

	TEST_ASSERT_UNLESS( fixedmpool_free(s_handle, p[0]) );
	p[0] = fixedmpool_allocate(s_handle);
	TEST_ASSERT_EQUAL_PTR( p[0], q[0] );

	{
		int random_idxs[BLOCK_COUNT] = { 3, 0, 1, 2 };
		/* 
		 *  p[0] = q[3]
		 *  p[1] = q[0]
		 *  p[2] = q[1]
		 *  p[3] = q[2]
		 */
		for (i=0; i<(int)s_blockcount; i++) {
			int idx = random_idxs[i];
			TEST_ASSERT_UNLESS( fixedmpool_free(s_handle, p[idx]) );
		}
		for (i=0; i<(int)s_blockcount; i++) {
			/* LIFO */
			int idx = random_idxs[s_blockcount - 1 - i];
			p[i] = fixedmpool_allocate(s_handle);
			TEST_ASSERT_EQUAL_PTR( p[i], q[idx] );
		}
	}

	TEST_ASSERT_UNLESS( fixedmpool_free(s_handle, p[0]) );
	TEST_ASSERT( fixedmpool_free(s_handle, p[0]) );
}

TEST(fixed_memorypool, size)
{
	uint8_t *p[BLOCK_COUNT];
	int i, using = 0;

	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedmpool_available_blocks(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, fixedmpool_used_blocks(s_handle) );

	for (i=0; i<(int)s_blockcount; i++) {
		p[i] = fixedmpool_allocate(s_handle);
		TEST_ASSERT_NOT_NULL( p[i] );
		using++;
		TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedmpool_available_blocks(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, fixedmpool_used_blocks(s_handle) );
	}
	TEST_ASSERT_NULL( fixedmpool_allocate(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedmpool_available_blocks(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( using, fixedmpool_used_blocks(s_handle) );

	{
		int random_idxs[BLOCK_COUNT] = { 3, 1, 2, 0 };
		for (i=0; i<(int)s_blockcount; i++) {
			int idx = random_idxs[i];
			TEST_ASSERT_UNLESS( fixedmpool_free(s_handle, p[idx]) );
			using--;
			TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedmpool_available_blocks(s_handle) );
			TEST_ASSERT_EQUAL_UINT32( using, fixedmpool_used_blocks(s_handle) );
		}
	}
	TEST_ASSERT( fixedmpool_free(s_handle, p[0]) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedmpool_available_blocks(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( using, fixedmpool_used_blocks(s_handle) );
}


TEST_GROUP_RUNNER(fixed_memorypool)
{
    RUN_TEST_CASE(fixed_memorypool, create);
    RUN_TEST_CASE(fixed_memorypool, destroy);
    RUN_TEST_CASE(fixed_memorypool, blockdata_size);
    RUN_TEST_CASE(fixed_memorypool, allocate);
    RUN_TEST_CASE(fixed_memorypool, free);
    RUN_TEST_CASE(fixed_memorypool, size);
}

