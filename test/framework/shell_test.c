#include "unity_fixture.h"
#include "shell.h"
#include <pthread.h>
#include <string.h>

TEST_GROUP(shell);

static handle_t s_handle = NULL;
#define POOL_SIZE 256
#define LINE_SIZE 64
static uint8_t s_pool[POOL_SIZE];
char    s_rxbuf[LINE_SIZE];
int32_t s_rxindex;
char    s_txc;
pthread_t s_thread;
pthread_mutex_t s_mutex;
char    s_result_line[LINE_SIZE];

static error_t stub_putc(char c) {
	s_rxbuf[s_rxindex++] = c;
	return 0;
}

static char stub_getc(void) {
	char c;
	pthread_mutex_lock( &s_mutex );
	c = s_txc;
	pthread_mutex_unlock( &s_mutex );
	return c;
}

static error_t stub_exec(char *line) {
	if (!line) return -1;
	strcpy(s_result_line, line);
	return 0;
}

TEST_SETUP(shell)
{
	memset(s_rxbuf, 0, sizeof(s_rxbuf));
	memset(s_result_line, 0, sizeof(s_result_line));
	s_rxindex = 0;
	s_txc = 0;
	pthread_mutex_init(&s_mutex, NULL);
}

TEST_TEAR_DOWN(shell)
{
	shell_destroy(s_handle);
	pthread_mutex_destroy(&s_mutex);
	pthread_detach(s_thread);
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

static void* test_thread(void* args)
{
	char test_chars[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
		'\n', 0
	};
	char *p = test_chars;
	pthread_mutex_lock( &s_mutex );

	TEST_ASSERT_UNLESS( shell_start(s_handle) );

	while (p) {
		s_txc = *p++;
		pthread_mutex_unlock( &s_mutex );
		/* delay(1ms) */
		pthread_mutex_lock( &s_mutex );
	}

	/* excludes new line code */
	TEST_ASSERT_EQUAL_STRING_LEN(test_chars, s_rxbuf, strlen(test_chars) - 1);
	TEST_ASSERT_EQUAL_STRING_LEN(test_chars, s_result_line, strlen(test_chars) - 1);

	pthread_mutex_unlock( &s_mutex );

	return NULL;
}

TEST(shell, start)
{
	s_handle = shell_create(s_pool, POOL_SIZE, stub_putc, stub_getc, stub_exec);
	TEST_ASSERT_NOT_NULL( s_handle );

	pthread_create(&s_thread, NULL, test_thread, NULL);

	pthread_join(s_thread, NULL);
}

TEST_GROUP_RUNNER(shell)
{
	RUN_TEST_CASE(shell, create);
	RUN_TEST_CASE(shell, destroy);
	//RUN_TEST_CASE(shell, start);
}
