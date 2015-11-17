#include "unity_fixture.h"
#include "queue.h"
#include <stdio.h>

TEST_GROUP(queue);

static handle_t s_handle = NULL;

TEST_SETUP(queue)
{
}

TEST_TEAR_DOWN(queue)
{
	queue_destroy(s_handle);
}

TEST(queue, create)
{
}

TEST(queue, destroy)
{
}

TEST(queue, write_read)
{
}

TEST(queue, get)
{
}

TEST(queue, size)
{
}

TEST(queue, purge)
{
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


