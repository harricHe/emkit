#include "unity_fixture.h"
#include "checksum.h"

TEST_GROUP(checksum);

TEST_SETUP(checksum)
{
}

TEST_TEAR_DOWN(checksum)
{
}

TEST(checksum, calc_8bit)
{
	uint8_t array[] = {
		0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
		0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
	};
	size_t len = sizeof(array) / sizeof(array[0]);
	uint8_t expects = 0xF8;  /* 0x7F8 */
	TEST_ASSERT_EQUAL_UINT8( expects, checksum_calc_8bit(array, len) );
}

TEST(checksum, calc_16bit)
{
	uint16_t array[] = {
		0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x8888,
		0x9999, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF, 0x0000
	};
	size_t len = sizeof(array) / sizeof(array[0]);
	uint16_t expects = 0xFFF8;  /* 0x7FFF8 */
	TEST_ASSERT_EQUAL_UINT16( expects, checksum_calc_16bit(array, len) );
}

TEST(checksum, calc_32bit)
{
	uint32_t array[] = {
		0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777, 0x88888888,
		0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x00000000
	};
	size_t len = sizeof(array) / sizeof(array[0]);
	uint32_t expects = 0xFFFFFFF8;  /* 0x7FFFFFFF8 */
	TEST_ASSERT_EQUAL_UINT32( expects, checksum_calc_32bit(array, len) );
}

TEST(checksum, calc_64bit)
{
	uint64_t array[] = {
		0x1111111111111111, 0x2222222222222222, 0x3333333333333333, 0x4444444444444444,
		0x5555555555555555, 0x6666666666666666, 0x7777777777777777, 0x8888888888888888,
		0x9999999999999999, 0xAAAAAAAAAAAAAAAA, 0xBBBBBBBBBBBBBBBB, 0xCCCCCCCCCCCCCCCC,
		0xDDDDDDDDDDDDDDDD, 0xEEEEEEEEEEEEEEEE, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000
	};
	size_t len = sizeof(array) / sizeof(array[0]);
	uint64_t expects = 0xFFFFFFFFFFFFFFF8;  /* 0x7FFFFFFFFFFFFFFF8 */
	TEST_ASSERT_EQUAL_UINT64( expects, checksum_calc_64bit(array, len) );
}

TEST_GROUP_RUNNER(checksum)
{
	/* normal tests */
	RUN_TEST_CASE(checksum, calc_8bit);
	RUN_TEST_CASE(checksum, calc_16bit);
	RUN_TEST_CASE(checksum, calc_32bit);
	RUN_TEST_CASE(checksum, calc_64bit);
}

