#include "unity_fixture.h"
#include "stack.h"
#include <stdio.h>

TEST_GROUP(stack);

#define POOL_SIZE (32)
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];

TEST_SETUP(stack)
{
}

TEST_TEAR_DOWN(stack)
{
	stack_destroy(s_handle);
}

TEST(stack, create)
{
	size_t unitsz = 4;
	size_t count = POOL_SIZE / unitsz;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT( s_handle );
}

TEST(stack, destroy)
{
	size_t unitsz = 4;
	size_t count = POOL_SIZE / unitsz;
	uint32_t data;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( stack_destroy(s_handle) );
	TEST_ASSERT_NULL( stack_pop(s_handle, &data) );
}

TEST(stack, push_pop1)
{
	size_t unitsz = 1;
	size_t count = POOL_SIZE / unitsz;
	uint8_t data = 0;
	uint8_t i;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );

	// push
	for (i=0; i<count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
	}
	// overflow
	TEST_ASSERT( stack_push(s_handle, &data) );

	// pop
	for (i=count; i>0; i--) {
		TEST_ASSERT_EQUAL_PTR( &data, stack_pop(s_handle, &data) );
		TEST_ASSERT_EQUAL_UINT8( i-1, data );
	}
	// underflow
	TEST_ASSERT_EQUAL_PTR( NULL, stack_pop(s_handle, &data) );
}

TEST(stack, push_pop2)
{
	size_t unitsz = 2;
	size_t count = POOL_SIZE / unitsz;
	uint16_t data;
	uint16_t i;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (i=0; i<(int32_t)count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
	}
	// overflow
	TEST_ASSERT( stack_push(s_handle, &data) );

	// pop
	for (i=count; i>0; i--) {
		TEST_ASSERT_EQUAL_PTR( &data, stack_pop(s_handle, &data) );
		TEST_ASSERT_EQUAL_UINT16( i-1, data );
	}
	// underflow
	TEST_ASSERT_EQUAL_PTR( NULL, stack_pop(s_handle, &data) );
}

TEST(stack, push_pop4)
{
	size_t unitsz = 4;
	size_t count = POOL_SIZE / unitsz;
	uint32_t data;
	uint32_t i;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (i=0; i<count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
	}
	// overflow
	TEST_ASSERT( stack_push(s_handle, &data) );

	// pop
	for (i=count; i>0; i--) {
		TEST_ASSERT_EQUAL_PTR( &data, stack_pop(s_handle, &data) );
		TEST_ASSERT_EQUAL_UINT32( i-1, data );
	}
	// underflow
	TEST_ASSERT_EQUAL_PTR( NULL, stack_pop(s_handle, &data) );
}

TEST(stack, push_pop8)
{
	size_t unitsz = 4;
	size_t count = POOL_SIZE / unitsz;
	uint64_t data;
	int32_t i;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );
	for (i=0; i<(int32_t)count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
	}
	// overflow
	TEST_ASSERT( stack_push(s_handle, &data) );

	// pop
	for (i=count; i>0; i--) {
		TEST_ASSERT_EQUAL_PTR( &data, stack_pop(s_handle, &data) );
		TEST_ASSERT_EQUAL_UINT8( i-1, data );
	}
	// underflow
	TEST_ASSERT_EQUAL_PTR( NULL, stack_pop(s_handle, &data) );
}

TEST(stack, size)
{
	size_t unitsz = 1;
	size_t count = POOL_SIZE / unitsz;
	uint8_t data = 0;
	uint8_t i;
	size_t using = 0;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( count, stack_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( count, stack_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, stack_used(s_handle) );

	// push
	for (i=0; i<(int32_t)count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
		using++;
		TEST_ASSERT_EQUAL_UINT32( count - using, stack_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, stack_used(s_handle) );
	}

	// pop
	for (i=count; i>0; i--) {
		TEST_ASSERT_EQUAL_PTR( &data, stack_pop(s_handle, &data) );
		using--;
		TEST_ASSERT_EQUAL_UINT8( i-1, data );
		TEST_ASSERT_EQUAL_UINT32( count - using, stack_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, stack_used(s_handle) );
	}
}

TEST(stack, purge)
{
	size_t unitsz = 1;
	size_t count = POOL_SIZE / unitsz;
	uint8_t i;
	size_t using;
	s_handle = stack_create(s_pool, unitsz, count);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( count, stack_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( count, stack_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, stack_used(s_handle) );

	// push
	using = 0;
	for (i=0; i<count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
		using++;
		TEST_ASSERT_EQUAL_UINT32( count - using, stack_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, stack_used(s_handle) );
	}

	// purge
	TEST_ASSERT_UNLESS( stack_purge(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( count, stack_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, stack_used(s_handle) );

	// push
	using = 0;
	for (i=0; i<count; i++) {
		TEST_ASSERT_UNLESS( stack_push(s_handle, &i) );
		using++;
		TEST_ASSERT_EQUAL_UINT32( count - using, stack_available(s_handle) );
		TEST_ASSERT_EQUAL_UINT32( using, stack_used(s_handle) );
	}
}

TEST_GROUP_RUNNER(stack)
{
	/* normal tests */
	RUN_TEST_CASE(stack, create);
	RUN_TEST_CASE(stack, destroy);
	RUN_TEST_CASE(stack, push_pop1);
	RUN_TEST_CASE(stack, push_pop2);
	RUN_TEST_CASE(stack, push_pop4);
	RUN_TEST_CASE(stack, push_pop8);
	RUN_TEST_CASE(stack, size);
	RUN_TEST_CASE(stack, purge);
}

