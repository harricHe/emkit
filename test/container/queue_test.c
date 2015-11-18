#include "unity_fixture.h"
#include "queue.h"

TEST_GROUP(queue);

#define BLOCK_SIZE  (4)
#define BLOCK_COUNT (4)
#define POOL_SIZE   ((BLOCK_SIZE) * (BLOCK_COUNT))
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];

TEST_SETUP(queue)
{
}

TEST_TEAR_DOWN(queue)
{
	queue_destroy(s_handle);
}

TEST(queue, create)
{
	s_handle = queue_create(s_pool, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT( s_handle );
}

TEST(queue, destroy)
{
	s_handle = queue_create(s_pool, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( queue_destroy(s_handle) );
	TEST_ASSERT_NULL( queue_get(s_handle) );
}

TEST(queue, write_read)
{
	uint32_t i, data;
	s_handle = queue_create(s_pool, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_write(s_handle, &i) );
	}
	TEST_ASSERT( queue_write(s_handle, &i) );

	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_read(s_handle, &data) );
		TEST_ASSERT_EQUAL_UINT32( i, data );
	}
	TEST_ASSERT( queue_read(s_handle, &data) );
}

TEST(queue, get)
{
	uint32_t i;
	uint32_t *data;
	s_handle = queue_create(s_pool, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_write(s_handle, &i) );
	}
	TEST_ASSERT( queue_write(s_handle, &i) );

	for (i=0; i<BLOCK_COUNT; i++) {
		data = queue_get(s_handle);
		TEST_ASSERT_EQUAL_UINT32( i, *data );
	}
	TEST_ASSERT_NULL( queue_get(s_handle) );
}

TEST(queue, size)
{
	uint32_t i;
	uint32_t using = 0;
	uint32_t *data;
	s_handle = queue_create(s_pool, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_used(s_handle) );

	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_write(s_handle, &i) );
		using++;
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using, queue_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, queue_used(s_handle) );
	}
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_used(s_handle) );

	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_read(s_handle, &data) );
		using--;
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using, queue_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, queue_used(s_handle) );
	}
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_used(s_handle) );
}

TEST(queue, purge)
{
	uint32_t i;
	uint32_t using = 0;
	s_handle = queue_create(s_pool, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_used(s_handle) );

	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_write(s_handle, &i) );
		using++;
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using, queue_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, queue_used(s_handle) );
	}
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_used(s_handle) );

	TEST_ASSERT_UNLESS( queue_purge(s_handle) );
	using = 0;
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_used(s_handle) );

	for (i=0; i<BLOCK_COUNT; i++) {
		TEST_ASSERT_UNLESS( queue_write(s_handle, &i) );
		using++;
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using, queue_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, queue_used(s_handle) );
	}
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, queue_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, queue_used(s_handle) );
}

TEST_GROUP_RUNNER(queue)
{
	/* normal tests */
	RUN_TEST_CASE(queue, create);
	RUN_TEST_CASE(queue, destroy);
	RUN_TEST_CASE(queue, write_read);
	RUN_TEST_CASE(queue, get);
	RUN_TEST_CASE(queue, size);
	RUN_TEST_CASE(queue, purge);
}


