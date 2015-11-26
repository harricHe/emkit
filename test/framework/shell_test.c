#include "unity_fixture.h"
#include "shell.h"
#include <pthread.h>

TEST_GROUP(shell);

static handle_t s_handle = NULL;
#define POOL_SIZE 256
static uint8_t s_pool[POOL_SIZE];
struct stub_buffer {
	char    buf[POOL_SIZE];
	int32_t index;
};
static struct stub_buffer s_rx;
pthread_mutex_t s_mutex;

static error_t stub_putc(char c) {
	return 0;
}

static error_t stub_getc(char c) {
	return 0;
}

static error_t stub_exec(char *line) {
	return 0;
}

TEST_SETUP(shell)
{
	s_rx.index = 0;
	pthread_mutex_init(&s_mutex, NULL);
}

TEST_TEAR_DOWN(shell)
{
	shell_destroy(s_handle);
	pthread_mutex_destroy(&s_mutex);
}

TEST(shell, create)
{
	s_handle = shell_create(s_pool, POOL_SIZE, stub_putc, stub_getc, stub_exec);
	TEST_ASSERT_NOT_NULL( s_handle );
}

TEST(shell, destroy)
{
	s_handle = shell_create(s_pool, POOL_SIZE, stub_putc, stub_getc, stub_exec);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( shell_destroy(s_handle) );
	TEST_ASSERT( shell_start(s_handle) );
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
