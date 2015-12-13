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
	const uint8_t decode_data[] = {
		0x01, 0x32, 0x32, 0x32, 0x55, 0xC9, 0xC9, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0x88
	};
	const uint8_t encode_data[] = {
		0x01, 0x01, 0x03, 0x32, 0x01, 0x55, 0x02, 0xC9, 0x05, 0xA0, 0x01, 0x88
	};
	uint8_t work[32];
	size_t act_len;

	act_len = rle_encode(decode_data, sizeof(decode_data), work);
	TEST_ASSERT_EQUAL(sizeof(encode_data), act_len);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(encode_data, work, act_len);

	act_len = rle_decode(encode_data, act_len, work);
	TEST_ASSERT_EQUAL(sizeof(decode_data), act_len);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(decode_data, work, act_len);
}

TEST_GROUP_RUNNER(rle)
{
	/* normal tests */
	RUN_TEST_CASE(rle, encode_decode);
}

