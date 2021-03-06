#include "unity_fixture.h"
#include "rle.h"
#include <stdio.h>
#include <string.h>

TEST_GROUP(rle);

TEST_SETUP(rle)
{
}

TEST_TEAR_DOWN(rle)
{
}

static void dump(const uint8_t *data, size_t length)
{
#if 0
	size_t i;
	printf("{ ");
	for (i=0; i<length; i++) {
		printf("%02x ", data[i]);
	}
	printf("}\n");
#endif
}

TEST(rle, encode_decode)
{
	const uint8_t decode_data_0[] = {
		0x01, 0x32, 0x32, 0x32, 0x32, 0x32, 0x55, 0x37, 0x11, 0xAC, 0x8E, 0x97,
		0xC9, 0xC9, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0x88
	};
	const uint8_t encode_data_0[] = {
		0x01, 0x01, 0x05, 0x32, 0x00, 0x06, 0x55, 0x37, 0x11, 0xAC, 0x8E, 0x97,
		0x02, 0xC9, 0x08, 0xA0, 0x01, 0x88
	};

	const uint8_t decode_data_1[] = {
		0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3
	};
	const uint8_t encode_data_1[] = {
		0x0C, 0xB3
	};

	const uint8_t decode_data_2[] = {
		0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF
	};
	const uint8_t encode_data_2[] = {
		0x00, 0x08, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF
	};

	const uint8_t decode_data_3[] = {
		0x12, 0x12, 0x12, 0x78, 0x90, 0xAB, 0xAB, 0xAB, 0xAB
	};
	const uint8_t encode_data_3[] = {
		0x03, 0x12, 0x00, 0x02, 0x78, 0x90, 0x04, 0xAB
	};

	struct rle_test_data_s {
		const uint8_t *decode_data;
		const uint8_t *encode_data;
		size_t numof_decode_data;
		size_t numof_encode_data;
	} rtd[] = {
		{ decode_data_0, encode_data_0, sizeof(decode_data_0), sizeof(encode_data_0) },
		{ decode_data_1, encode_data_1, sizeof(decode_data_1), sizeof(encode_data_1) },
		{ decode_data_2, encode_data_2, sizeof(decode_data_2), sizeof(encode_data_2) },
		{ decode_data_3, encode_data_3, sizeof(decode_data_3), sizeof(encode_data_3) }
	};
	size_t numof_rle_data = sizeof(rtd)/sizeof(rtd[0]);
	int32_t i;

	for (i=0; i<(int32_t)numof_rle_data; i++) {
		struct rle_test_data_s *p = &rtd[i];
		uint8_t work[32];
		memset(work, 0, sizeof(work));

		size_t act_len = rle_encode(p->decode_data, p->numof_decode_data, work, sizeof(work));

		/* printf("<encode>\n"); */
		dump(p->decode_data, p->numof_decode_data);
		dump(work, act_len);
		/* printf("\n"); */

		TEST_ASSERT_EQUAL(p->numof_encode_data, act_len);
		TEST_ASSERT_EQUAL_UINT8_ARRAY(p->encode_data, work, act_len);

		memset(work, 0, sizeof(work));
		act_len = rle_decode(p->encode_data, p->numof_encode_data, work, sizeof(work));

		/* printf("<decode>\n"); */
		dump(p->encode_data, p->numof_encode_data);
		dump(work, act_len);
		/* printf("\n"); */

		TEST_ASSERT_EQUAL(p->numof_decode_data, act_len);
		TEST_ASSERT_EQUAL_UINT8_ARRAY(p->decode_data, work, act_len);
	}

}


TEST(rle, encode_overflow)
{
	const uint8_t decode_data[] = {
		0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF
	};
	const uint8_t encode_data[] = {
		0x00, 0x08, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF
	};

	struct rle_test_data_s {
		const uint8_t *decode_data;
		const uint8_t *encode_data;
		size_t numof_decode_data;
		size_t numof_encode_data;
	} rtd[] = {
		{ decode_data, encode_data, sizeof(decode_data), sizeof(encode_data) }
	};
	size_t numof_rle_data = sizeof(rtd)/sizeof(rtd[0]);
	int32_t i;

	for (i=0; i<(int32_t)numof_rle_data; i++) {
		struct rle_test_data_s *p = &rtd[i];
		uint8_t work[8];
		memset(work, 0, sizeof(work));

		size_t act_len = rle_encode(p->decode_data, p->numof_decode_data, work, sizeof(work));

		/* printf("<encode>\n"); */
		dump(p->decode_data, p->numof_decode_data);
		dump(work, act_len);
		/* printf("\n"); */

		TEST_ASSERT_EQUAL(0, act_len);
	}

}


TEST_GROUP_RUNNER(rle)
{
	/* normal tests */
	RUN_TEST_CASE(rle, encode_decode);

	/* abnormal tests */
	RUN_TEST_CASE(rle, encode_overflow);
}

