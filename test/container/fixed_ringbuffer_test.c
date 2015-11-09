#include "unity_fixture.h"
#include "fixed_ringbuffer.h"

TEST_GROUP(fixed_ringbuffer);

static handle_t s_handle = NULL;
static uint8_t s_pool[64];
static size_t s_blocksize = 16;
static size_t s_blockcount = 16;

TEST_SETUP(fixed_ringbuffer)
{
}

TEST_TEAR_DOWN(fixed_ringbuffer)
{
	fixedringbuf_destroy(s_handle);
}

TEST(fixed_ringbuffer, create)
{
	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT( s_handle );
}

TEST(fixed_ringbuffer, destroy)
{
	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_UNLESS( fixedringbuf_destroy(s_handle) );
	TEST_ASSERT_NULL( fixedringbuf_get(s_handle) );
}

TEST_GROUP_RUNNER(fixed_ringbuffer)
{
    RUN_TEST_CASE(fixed_ringbuffer, create);
    RUN_TEST_CASE(fixed_ringbuffer, destroy);
}

