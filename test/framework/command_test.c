#include "unity_fixture.h"
#include "command.h"
#include <string.h>
#include <stdio.h>  /* putchar() */

TEST_GROUP(command);

#define COMMAND_COUNT (4)
#define POOL_SIZE     (COMMAND_UNIT_SIZE * COMMAND_COUNT)
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];
static int32_t s_value_a;
static int32_t s_value_b;
#define HELP_BUFFER_SIZE (128)
static char s_help_buffer[HELP_BUFFER_SIZE];
static char *s_p;


static error_t func_test_blank(int32_t argc, const char **argv)
{
	return 0;
}

static error_t func_test_set_value_a(int32_t argc, const char **argv)
{
	if (argc == 1) {
		s_value_a = 0;
		return 0;
	}

	if (strcmp(argv[1], "AA") == 0) {
		s_value_a = 1;
	} else if (strcmp(argv[1], "BBB") == 0) {
		s_value_a = 2;
	} else if (strcmp(argv[1], "error") == 0) {
		return -1;
	}

	return 0;
}

static error_t func_test_set_value_b(int32_t argc, const char **argv)
{
	if (argc < 3) {
		s_value_b = 1;
		return 0;
	}

	if (strcmp(argv[1], "sub") == 0) {
		if (strcmp(argv[2], "subsub") == 0) {
			s_value_b = 2;
		}
	}

	return 0;
}

static error_t put_character(char c)
{
	*(s_p++) = c;
	return 0;
}

TEST_SETUP(command)
{
	s_value_a = 0;
	s_value_b = 0;
	command_init(put_character);
	memset(s_help_buffer, 0, HELP_BUFFER_SIZE);
	s_p = s_help_buffer;
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
	TEST_ASSERT( command_add(s_handle, "test", func_test_blank, NULL) );
}

TEST(command, add)
{
	s_handle = command_create(s_pool, POOL_SIZE);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( command_add(s_handle, "aaa", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "bbb", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "ccc", func_test_blank, NULL) );

	// add same command
	TEST_ASSERT( command_add(s_handle, "aaa", func_test_blank, NULL) );
	// add blank command
	TEST_ASSERT( command_add(s_handle, "", func_test_blank, NULL) );
	// add NULL func command
	TEST_ASSERT( command_add(s_handle, "null", NULL, NULL) );

	TEST_ASSERT_UNLESS( command_add(s_handle, "ddd", func_test_blank, NULL) );

	// command list is ful
	TEST_ASSERT( command_add(s_handle, "xxxx", func_test_blank, NULL) );
}


TEST(command, remove)
{
	s_handle = command_create(s_pool, POOL_SIZE);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( command_add(s_handle, "aaa", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "bbb", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "ccc", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "ddd", func_test_blank, NULL) );

	TEST_ASSERT( command_add(s_handle, "xxx", func_test_blank, NULL) );

	TEST_ASSERT_UNLESS( command_remove(s_handle, "aaa") );
	TEST_ASSERT( command_remove(s_handle, "aaa") );

	TEST_ASSERT_UNLESS( command_remove(s_handle, "bbb") );
	TEST_ASSERT( command_remove(s_handle, "bbb") );

	TEST_ASSERT_UNLESS( command_remove(s_handle, "ccc") );
	TEST_ASSERT( command_remove(s_handle, "ccc") );

	TEST_ASSERT_UNLESS( command_remove(s_handle, "ddd") );
	TEST_ASSERT( command_remove(s_handle, "ddd") );

	TEST_ASSERT_UNLESS( command_add(s_handle, "aaa", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "bbb", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "ccc", func_test_blank, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "ddd", func_test_blank, NULL) );
}


TEST(command, execute)
{
	s_handle = command_create(s_pool, POOL_SIZE);

	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( command_add(s_handle, "aaa", func_test_set_value_a, NULL) );
	TEST_ASSERT_UNLESS( command_add(s_handle, "bbb", func_test_set_value_b, NULL) );

	{
		const char *cmdline[] = { "aaa", "AA" };
		TEST_ASSERT_UNLESS( command_execute(s_handle, 2, cmdline) );
		TEST_ASSERT_EQUAL_INT32( 1, s_value_a );
	}
	{
		const char *cmdline[] = { "aaa", "BBB" };
		TEST_ASSERT_UNLESS( command_execute(s_handle, 2, cmdline) );
		TEST_ASSERT_EQUAL_INT32( 2, s_value_a );
	}
	{
		const char *cmdline[] = { "aaa", "error" };
		TEST_ASSERT( command_execute(s_handle, 2, cmdline) );
	}

	{
		const char *cmdline[] = { "bbb" };
		TEST_ASSERT_UNLESS( command_execute(s_handle, 1, cmdline) );
		TEST_ASSERT_EQUAL_INT32( 1, s_value_b );
	}
	{
		const char *cmdline[] = { "bbb", "sub", "subsub" };
		TEST_ASSERT_UNLESS( command_execute(s_handle, 3, cmdline) );
		TEST_ASSERT_EQUAL_INT32( 2, s_value_b );
	}
}


TEST(command, help)
{
	s_handle = command_create(s_pool, POOL_SIZE);
	const char *help_expects = "command list\naaa\tthis is aaa command\nbbb\tthis is bbb command\n";

	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( command_add(s_handle, "aaa", func_test_set_value_a, "this is aaa command") );
	TEST_ASSERT_UNLESS( command_add(s_handle, "bbb", func_test_set_value_b, "this is bbb command") );

	{
		const char *cmdline[] = { "help" };
		TEST_ASSERT( command_execute(s_handle, 1, cmdline) );
		TEST_ASSERT_EQUAL_STRING( help_expects, s_help_buffer );
	}
}


TEST_GROUP_RUNNER(command)
{
	RUN_TEST_CASE(command, create);
	RUN_TEST_CASE(command, destroy);
	RUN_TEST_CASE(command, add);
	RUN_TEST_CASE(command, remove);
	RUN_TEST_CASE(command, execute);
	RUN_TEST_CASE(command, help);
}

