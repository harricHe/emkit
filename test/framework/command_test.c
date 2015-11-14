#include "unity_fixture.h"
#include "command.h"

TEST_GROUP(command);

#define COMMAND_COUNT (4)
#define POOL_SIZE     (COMMAND_UNIT_SIZE * COMMAND_COUNT)
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];

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

TEST_GROUP_RUNNER(command)
{
	RUN_TEST_CASE(command, create);
}

