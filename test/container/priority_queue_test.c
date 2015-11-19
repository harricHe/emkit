#include "unity_fixture.h"
#include "priority_queue.h"

TEST_GROUP(priority_queue);

TEST_SETUP(priority_queue)
{
}

TEST_TEAR_DOWN(priority_queue)
{
}

TEST(priority_queue, create)
{
}

TEST(priority_queue, destroy)
{
}

TEST(priority_queue, write)
{
}

TEST(priority_queue, read)
{
}

TEST(priority_queue, read_with_priority)
{
}

TEST(priority_queue, get)
{
}

TEST(priority_queue, get_with_priority)
{
}

TEST(priority_queue, size)
{
}

TEST(priority_queue, purge)
{
}

TEST_GROUP_RUNNER(priority_queue)
{
	/* normal tests */
	RUN_TEST_CASE(priority_queue, create);
	RUN_TEST_CASE(priority_queue, destroy);
	RUN_TEST_CASE(priority_queue, write);
	RUN_TEST_CASE(priority_queue, read);
	RUN_TEST_CASE(priority_queue, read_with_priority);
	RUN_TEST_CASE(priority_queue, get);
	RUN_TEST_CASE(priority_queue, get_with_priority);
	RUN_TEST_CASE(priority_queue, size);
	RUN_TEST_CASE(priority_queue, purge);
}


