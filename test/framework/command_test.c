#include "unity_fixture.h"
#include "command.h"

TEST_GROUP(command);

#define COMMAND_COUNT (4)
#define POOL_SIZE     (COMMAND_UNIT_SIZE * COMMAND_COUNT)
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];

static error_t blankfunc(int32_t argc, const char **argv)
{
	return 0;
}

TEST_SETUP(command)
{
}

TEST_TEAR_DOWN(command)
{
	command_destroy(s_handle);
}

TEST(command, create)
{
	s_handle = command_create(s_pool, POOL_SIZE);
	TEST_ASSERT_NOT_NULL( s_handle );
}

TEST(command, destroy)
{
	s_handle = command_create(s_pool, POOL_SIZE);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( command_destroy(s_handle) );
	TEST_ASSERT( command_add(s_handle, "test", blankfunc) );
}

TEST_GROUP_RUNNER(command)
{
	RUN_TEST_CASE(command, create);
	RUN_TEST_CASE(command, destroy);
}

