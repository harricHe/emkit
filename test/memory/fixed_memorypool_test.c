#include "unity_fixture.h"
#include "fixed_memorypool.h"

TEST_GROUP(fixed_memorypool);

static handle_t s_handle = NULL;
static uint8_t s_pool[64];
static size_t s_blocksize = 16;
static size_t s_blockcount = 4;

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
	uint8_t *p[s_blockcount];
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

TEST_GROUP_RUNNER(fixed_memorypool)
{
    RUN_TEST_CASE(fixed_memorypool, create);
    RUN_TEST_CASE(fixed_memorypool, destroy);
    RUN_TEST_CASE(fixed_memorypool, blockdata_size);
    RUN_TEST_CASE(fixed_memorypool, allocate);
}

