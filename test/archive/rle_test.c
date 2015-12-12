#include "unity_fixture.h"
#include "rle.h"

TEST_GROUP(rle);

TEST_SETUP(rle)
{
}

TEST_TEAR_DOWN(rle)
{
}

TEST(rle, encode_decode)
{
}

TEST_GROUP_RUNNER(rle)
{
	/* normal tests */
	RUN_TEST_CASE(rle, encode_decode);
}

