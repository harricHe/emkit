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
	TEST_ASSERT( s_handle );
}

TEST(fixed_memorypool, destroy)
{
	s_handle = fixedmpool_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_UNLESS( fixedmpool_destroy(s_handle) );
	TEST_ASSERT_NULL( fixedmpool_allocate(s_handle) );
}

TEST_GROUP_RUNNER(fixed_memorypool)
{
    RUN_TEST_CASE(fixed_memorypool, create);
    RUN_TEST_CASE(fixed_memorypool, destroy);
}

