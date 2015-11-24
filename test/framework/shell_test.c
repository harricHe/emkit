#include "unity_fixture.h"
#include "shell.h"

TEST_GROUP(shell);

TEST_SETUP(shell)
{
}

TEST_TEAR_DOWN(shell)
{
}

TEST(shell, create)
{
}

TEST(shell, destroy)
{
}

TEST(shell, start)
{
}

TEST_GROUP_RUNNER(shell)
{
	RUN_TEST_CASE(shell, create);
	RUN_TEST_CASE(shell, destroy);
	RUN_TEST_CASE(shell, start);
}
