#include "unity_fixture.h"
#include "text.h"

TEST_GROUP(text);

#define MESSAGE_LENGHT 64

TEST_SETUP(text)
{
}

TEST_TEAR_DOWN(text)
{
}

TEST(text, word_num)
{
	char msg[MESSAGE_LENGHT] = "abc def,ghi	jkl  mno ,	";
	const char *seps = " ,	";
	TEST_ASSERT_EQUAL( 5, text_words_num(msg, MESSAGE_LENGHT, seps) );
}

TEST(text, separate)
{
	char msg[MESSAGE_LENGHT] = "abc def,ghi	jkl  mno ,	";
	const char *seps = " ,	";
	char *args[10];
	char *pre;
	int32_t i;

	pre = args[0] = msg;

	for (i=1; i<10; i++) {
		pre = text_separate(pre, MESSAGE_LENGHT, seps);
		if (!pre) {
			break;
		}
		args[i] = pre;
	}

	TEST_ASSERT_EQUAL( 5, i );
	TEST_ASSERT_NOT_NULL( args[0] );
	TEST_ASSERT_NOT_NULL( args[1] );
	TEST_ASSERT_NOT_NULL( args[2] );
	TEST_ASSERT_NOT_NULL( args[3] );
	TEST_ASSERT_NOT_NULL( args[4] );
	TEST_ASSERT_EQUAL_STRING( "abc", args[0] );
	TEST_ASSERT_EQUAL_STRING( "def", args[1] );
	TEST_ASSERT_EQUAL_STRING( "ghi", args[2] );
	TEST_ASSERT_EQUAL_STRING( "jkl", args[3] );
	TEST_ASSERT_EQUAL_STRING( "mno", args[4] );
}


TEST_GROUP_RUNNER(text)
{
	RUN_TEST_CASE(text, word_num);
	RUN_TEST_CASE(text, separate);
}

