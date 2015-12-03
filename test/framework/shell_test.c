#include "unity_fixture.h"
#include "shell.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>

TEST_GROUP(shell);

static handle_t s_handle = NULL;
#define POOL_SIZE 256
#define LINE_SIZE 64
static uint8_t s_pool[POOL_SIZE];
static char s_rxbuf[LINE_SIZE];
static int32_t s_rxindex;
static volatile char s_txc;
static volatile bool_t s_done = FALSE;
static pthread_t s_thread;
static pthread_mutex_t s_mutex;
static char s_result_line[LINE_SIZE];
static int32_t s_error = 0;


static error_t stub_putc(char c) {
	s_rxbuf[s_rxindex++] = c;
	//printf("%c", c);
	return 0;
}

static char stub_getc(void) {
	char c;
	while (!s_txc);

	c = s_txc;

	s_txc = 0;
	return c;
}

static error_t stub_exec(const char *line) {
	if (!line) return -1;
	strcpy(s_result_line, line);
	s_done = TRUE;
	return 0;
}

TEST_SETUP(shell)
{
	memset(s_rxbuf, 0, sizeof(s_rxbuf));
	memset(s_result_line, 0, sizeof(s_result_line));
	s_rxindex = 0;
	s_txc = 0;
	s_done = FALSE;
	s_error = 0;
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
	s_handle = shell_create(s_pool, POOL_SIZE,
			stub_putc, stub_getc, stub_exec, NULL);
	TEST_ASSERT_NOT_NULL( s_handle );
}

TEST(shell, destroy)
{
	s_handle = shell_create(s_pool, POOL_SIZE,
			stub_putc, stub_getc, stub_exec, NULL);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( shell_destroy(s_handle) );
	TEST_ASSERT( shell_start(s_handle) );
}

TEST(shell, set_prompt)
{
	s_handle = shell_create(s_pool, POOL_SIZE,
			stub_putc, stub_getc, stub_exec, NULL);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT( shell_set_prompt(s_handle, NULL) );
	TEST_ASSERT_UNLESS( shell_set_prompt(s_handle, "hoge") );
	TEST_ASSERT_UNLESS( shell_set_prompt(s_handle, "") );
}

static void* thread_shell(void* args)
{
	TEST_ASSERT_UNLESS( shell_start(s_handle) );
	return NULL;
}

TEST(shell, start)
{
	char test_chars[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
		'\n', 0
	};
	char *p = test_chars;

	s_handle = shell_create(s_pool, POOL_SIZE,
			stub_putc, stub_getc, stub_exec, NULL);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( shell_set_prompt(s_handle, "") );

	pthread_create(&s_thread, NULL, thread_shell, NULL);

	while (*p) {
		while (s_txc);
		s_txc = *p++;
	}

	while (!s_done);

	TEST_ASSERT_EQUAL_STRING_LEN(test_chars, s_rxbuf, strlen(test_chars));
	/* excludes new line code */
	TEST_ASSERT_EQUAL_STRING_LEN(test_chars, s_result_line, strlen(test_chars) - 1);

	TEST_ASSERT_UNLESS( shell_destroy(s_handle) );
	s_txc = '\n';

	pthread_join(s_thread, NULL);
}

static error_t stub_exec_ok(const char *line) {
	return 0;
}

static error_t stub_exec_ng(const char *line) {
	return -1;
}

static void stub_post_hook(error_t err, const char *line) {
	s_error = err;
	s_done = TRUE;
}

TEST(shell, hook)
{
	char test_chars[] = {
		'a', '\n', 0
	};
	char *p;

	// OK hook
	s_done = FALSE;
	s_error = 1;
	p = test_chars;
	s_handle = shell_create(s_pool, POOL_SIZE,
			stub_putc, stub_getc, stub_exec_ok, stub_post_hook);
	TEST_ASSERT_NOT_NULL( s_handle );
	pthread_create(&s_thread, NULL, thread_shell, NULL);
	while (*p) {
		while (s_txc);
		s_txc = *p++;
	}
	while (!s_done);
	TEST_ASSERT_EQUAL_INT32( 0, s_error );
	TEST_ASSERT_UNLESS( shell_destroy(s_handle) );
	s_txc = '\n';
	pthread_join(s_thread, NULL);


	// NG hook
	s_done = FALSE;
	s_error = 0;
	p = test_chars;
	s_handle = shell_create(s_pool, POOL_SIZE,
			stub_putc, stub_getc, stub_exec_ng, stub_post_hook);
	TEST_ASSERT_NOT_NULL( s_handle );
	pthread_create(&s_thread, NULL, thread_shell, NULL);
	while (*p) {
		while (s_txc);
		s_txc = *p++;
	}
	while (!s_done);
	TEST_ASSERT_EQUAL_INT32( -1, s_error );
	TEST_ASSERT_UNLESS( shell_destroy(s_handle) );
	s_txc = '\n';
	pthread_join(s_thread, NULL);
}

TEST_GROUP_RUNNER(shell)
{
	RUN_TEST_CASE(shell, create);
	RUN_TEST_CASE(shell, destroy);
	RUN_TEST_CASE(shell, set_prompt);
	RUN_TEST_CASE(shell, start);
	RUN_TEST_CASE(shell, hook);
}

