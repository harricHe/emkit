#include "unity_fixture.h"
#include "memorypool.h"

TEST_GROUP(memorypool);

TEST_SETUP(memorypool)
{
}

TEST_TEAR_DOWN(memorypool)
{
}

TEST(memorypool, create)
{
	// handle_t mempool_create(void *memory, size_t memsz);
}

TEST(memorypool, destroy)
{
	// error_t mempool_destroy(handle_t hdl);
}

TEST(memorypool, allocate)
{
	// void* mempool_allocate(handle_t hdl, size_t size);
}

TEST(memorypool, free)
{
	// error_t mempool_free(handle_t hdl, void *block);
}

TEST(memorypool, size)
{
	// size_t mempool_available(handle_t hdl);
	// size_t mempool_used(handle_t hdl);
	// size_t mempool_capacity(handle_t hdl);
}

TEST_GROUP_RUNNER(memorypool)
{
	RUN_TEST_CASE(memorypool, create);
	RUN_TEST_CASE(memorypool, destroy);
	RUN_TEST_CASE(memorypool, allocate);
	RUN_TEST_CASE(memorypool, free);
	RUN_TEST_CASE(memorypool, size);
}

