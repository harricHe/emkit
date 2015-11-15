#include "unity_fixture.h"
#include "stack.h"

TEST_GROUP(stack);

TEST_SETUP(stack)
{
}

TEST_TEAR_DOWN(stack)
{
}

TEST(stack, create)
{
}

TEST(stack, destroy)
{
}

TEST(stack, push)
{
}

TEST(stack, pop)
{
}

TEST(stack, size)
{
}

TEST(stack, purge)
{
}

TEST_GROUP_RUNNER(stack)
{
	/* normal tests */
	RUN_TEST_CASE(stack, create);
	RUN_TEST_CASE(stack, destroy);
	RUN_TEST_CASE(stack, push);
	RUN_TEST_CASE(stack, pop);
	RUN_TEST_CASE(stack, size);
	RUN_TEST_CASE(stack, purge);
}

