#include "unity_fixture.h"
#include "priority_queue.h"

TEST_GROUP(priority_queue);

#define PRIORITY_NUM (3)
#define BLOCK_SIZE   (4)
#define BLOCK_COUNT  (4)
#define POOL_SIZE   ((PRIORITY_NUM) * (BLOCK_SIZE) * (BLOCK_COUNT))
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];

TEST_SETUP(priority_queue)
{
}

TEST_TEAR_DOWN(priority_queue)
{
	priqueue_destroy(s_handle);
}

TEST(priority_queue, create)
{
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT( s_handle );
}

TEST(priority_queue, destroy)
{
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT( s_handle );
	TEST_ASSERT_UNLESS( priqueue_destroy(s_handle) );
	TEST_ASSERT_NULL( priqueue_get(s_handle) );
}

TEST(priority_queue, write)
{
	uint32_t pri_i, blk_i;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT( priqueue_write(s_handle, pri_i, &pri_i) );
	}
}

TEST(priority_queue, read)
{
	uint32_t pri_i, blk_i;
	uint32_t data;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_read(s_handle, &data) );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	TEST_ASSERT( priqueue_read(s_handle, &data) );

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t apri = PRIORITY_NUM - pri_i - 1;
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, apri, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_read(s_handle, &data) );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	TEST_ASSERT( priqueue_read(s_handle, &data) );
}

TEST(priority_queue, read_with_priority)
{
	uint32_t pri_i, blk_i;
	uint32_t data;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_read_with_priority(s_handle, pri_i, &data) );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT( priqueue_read_with_priority(s_handle, pri_i, &data) );
	}

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t apri = PRIORITY_NUM - pri_i - 1;
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, apri, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_read_with_priority(s_handle, pri_i, &data) );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT( priqueue_read_with_priority(s_handle, pri_i, &data) );
	}
}

TEST(priority_queue, get)
{
	uint32_t pri_i, blk_i;
	uint32_t data;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			uint32_t *p = priqueue_get(s_handle);
			TEST_ASSERT_NOT_NULL( p );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	TEST_ASSERT_NULL( priqueue_get(s_handle) );

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t apri = PRIORITY_NUM - pri_i - 1;
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, apri, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			uint32_t *p = priqueue_get(s_handle);
			TEST_ASSERT_NOT_NULL( p );
			TEST_ASSERT_EQUAL_PTR( &data, p );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	TEST_ASSERT_NULL( priqueue_get(s_handle) );
}

TEST(priority_queue, get_with_priority)
{
	uint32_t pri_i, blk_i;
	uint32_t data;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			uint32_t *p = priqueue_get_with_priority(s_handle, pri_i);
			TEST_ASSERT_NOT_NULL( p );
			TEST_ASSERT_EQUAL_PTR( &data, p );
			TEST_ASSERT_EQUAL_UINT32( expect, data );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_NULL( priqueue_get_with_priority(s_handle, pri_i) );
	}

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t apri = PRIORITY_NUM - pri_i - 1;
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, apri, &data) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			uint32_t expect = (BLOCK_COUNT * pri_i)  + blk_i;
			uint32_t *p = priqueue_get_with_priority(s_handle, pri_i);
			TEST_ASSERT_NOT_NULL( p );
			TEST_ASSERT_EQUAL_UINT32( expect, *p );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_NULL( priqueue_get_with_priority(s_handle, pri_i) );
	}
}

TEST(priority_queue, size)
{
	uint32_t pri_i, blk_i;
	uint32_t using[PRIORITY_NUM] = { 0 };
	uint32_t data;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_used(s_handle, pri_i) );
	}

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
			using[pri_i]++;
			TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using[pri_i], priqueue_available(s_handle, pri_i) );
			TEST_ASSERT_EQUAL_UINT32( using[pri_i], priqueue_used(s_handle, pri_i) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_used(s_handle, pri_i) );
	}

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			TEST_ASSERT_UNLESS( priqueue_read(s_handle, &data) );
			using[pri_i]--;
			TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using[pri_i], priqueue_available(s_handle, pri_i) );
			TEST_ASSERT_EQUAL_UINT32( using[pri_i], priqueue_used(s_handle, pri_i) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_used(s_handle, pri_i) );
	}
}

TEST(priority_queue, purge)
{
	uint32_t pri_i, blk_i;
	uint32_t using[PRIORITY_NUM] = { 0 };
	uint32_t data;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_used(s_handle, pri_i) );
	}

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			data = (BLOCK_COUNT * pri_i)  + blk_i;
			TEST_ASSERT_UNLESS( priqueue_write(s_handle, pri_i, &data) );
			using[pri_i]++;
			TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using[pri_i], priqueue_available(s_handle, pri_i) );
			TEST_ASSERT_EQUAL_UINT32( using[pri_i], priqueue_used(s_handle, pri_i) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_used(s_handle, pri_i) );
	}

	TEST_ASSERT_UNLESS( priqueue_purge(s_handle) );

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		using[pri_i] = 0;
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_used(s_handle, pri_i) );
	}

	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		for (blk_i=0; blk_i<BLOCK_COUNT; blk_i++) {
			TEST_ASSERT_UNLESS( priqueue_read(s_handle, &data) );
			using[pri_i]--;
			TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT - using[pri_i], priqueue_available(s_handle, pri_i) );
			TEST_ASSERT_EQUAL_UINT32( using[pri_i], priqueue_used(s_handle, pri_i) );
		}
	}
	for (pri_i=0; pri_i<PRIORITY_NUM; pri_i++) {
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_capacity(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( BLOCK_COUNT, priqueue_available(s_handle, pri_i) );
		TEST_ASSERT_EQUAL_UINT32( 0, priqueue_used(s_handle, pri_i) );
	}
}

TEST(priority_queue, illegal_priority)
{
	uint32_t illegal_pri = PRIORITY_NUM;
	uint32_t data = 0x00;
	s_handle = priqueue_create(s_pool, PRIORITY_NUM, BLOCK_SIZE, BLOCK_COUNT);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT( priqueue_write(s_handle, illegal_pri, &data) );
	TEST_ASSERT( priqueue_read_with_priority(s_handle, illegal_pri, &data) );
	TEST_ASSERT_NULL( priqueue_get_with_priority(s_handle, illegal_pri) );
	TEST_ASSERT_UNLESS( priqueue_available(s_handle, illegal_pri) );
	TEST_ASSERT_UNLESS( priqueue_used(s_handle, illegal_pri) );
	TEST_ASSERT_UNLESS( priqueue_capacity(s_handle, illegal_pri) );
}


TEST_GROUP_RUNNER(priority_queue)
{
	/* normal tests */
	//RUN_TEST_CASE(priority_queue, create);
	//RUN_TEST_CASE(priority_queue, destroy);
	//RUN_TEST_CASE(priority_queue, write);
	//RUN_TEST_CASE(priority_queue, read);
	//RUN_TEST_CASE(priority_queue, read_with_priority);
	//RUN_TEST_CASE(priority_queue, get);
	//RUN_TEST_CASE(priority_queue, get_with_priority);
	//RUN_TEST_CASE(priority_queue, size);
	//RUN_TEST_CASE(priority_queue, purge);

	/* normal tests */
	//RUN_TEST_CASE(priority_queue, illegal_priority);
}


