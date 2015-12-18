#include "rle.h"

/* run length encoding */

static size_t encode(const uint8_t *src, size_t slen, uint8_t *dst, size_t dlen)
{
	/**
	 *   { 0 0 1 2 3 4 8 8 8 8 } のようなデータ列を考える
	 * 1. 0
	 * 2. 0 0 -> continuous
	 * 3. 0 0 1 -> 1でbreakして 2 0 をwrite (1はwriteしない)
	 * 4. 1 2 -> absolute
	 * 5. 1 2 3
	 * 6. 1 2 3 4 -> 1 2 3までabsoluteが確定したので 1 2 3 を 0 3 1 2 3としてwrite
	 * 7. 4 8
	 * 8. 4 8 8 -> 8でabsoluteが終わる4をwrite
	 * 9. 8 8 8 8 end -> 4 8として書き込み
	 **/

	/**
	 *
	 * 1 2
	 * -> [abs] 0 2 1 2
	 * -> [con] 1 1 1 2
	 *
	 * 1 2 3
	 * -> [abs] 0 3 1 2 3
	 * -> [con] 1 1 1 2 1 3
	 *
	 * => 2つ以上の異なる値の連続で絶対モードを有効にしてよい (変わらない)
	 *
	 **/

	const uint8_t *rp = src;
	uint8_t *wp = dst;

	uint8_t value;
	uint8_t continuous_count = 0;
	size_t  remain = slen;

	uint8_t buf_count = 0;
	uint8_t buf[3];

#define IDLE_MODE       (0)
#define ABSOLUTE_MODE   (1)
#define CONTINUOUS_MODE (2)
	uint8_t mode = IDLE_MODE;

	while (remain--) {
		buf[buf_count++] = *rp++;
		switch (mode) {
			case IDLE_MODE:
				if (buf_count == 3) {
					if (buf[0] == buf[1]) {
						if (buf[0] == buf[2]) {
							/* A A A */
							value = buf[0];
							continuous_count = 3;
							mode = CONTINUOUS_MODE;
							buf_count = 0;
						} else {
							/* A A B */
							*wp++ = 2;
							*wp++ = buf[0];
							buf[0] = buf[2];
							continuous_count = 0;
							buf_count = 1;
						}
					} else {
						if (buf[1] == buf[2]) {
							/* A B B */
							*wp++ = 0;      /* absolute: flag */
							*wp++ = 1;      /* absolute: length */
							*wp++ = buf[0]; /* absolute: value */
							value = buf[1];
							continuous_count = 2;
							mode = CONTINUOUS_MODE;
							buf_count = 0;
						} else {
							/* A B C */
							*wp++ = 0;      /* absolute: flag */
							*wp++ = 1;      /* absolute: length */
							*wp++ = buf[0]; /* absolute: value */
							*wp++ = buf[1]; /* absolute: value */
							continuous_count = 0;
							mode = ABSOLUTE_MODE;
							buf[0] = buf[2];
							buf_count = 1;
						}
					}
				}
				break;
			case ABSOLUTE_MODE:
				if (buf[0] != buf[1]) {
					*wp++ = buf[0]; /* absolute: value */
					buf[0] = buf[1];
					continuous_count = 0;
					buf_count = 1;
				} else {
					value = buf[0];
					continuous_count = 2;
					mode = CONTINUOUS_MODE;
					buf_count = 0;
				}
				break;
			case CONTINUOUS_MODE:
				if (value == buf[0]) {
					continuous_count++;
					buf_count = 0;
				} else {
					*wp++ = continuous_count;
					*wp++ = value;
					continuous_count = 0;
					mode = IDLE_MODE;
				}
				break;
			default:
				break;
		}
	}

	switch (mode) {
		case IDLE_MODE:
			switch (buf_count) {
				case 1:
					*wp++ = 0;
					*wp++ = 1;
					*wp++ = buf[0];
					break;
				case 2:
					if (buf[0] == buf[1]) {
						*wp++ = 2;
						*wp++ = buf[0];
					} else {
						*wp++ = 0;
						*wp++ = 2;
						*wp++ = buf[0];
						*wp++ = buf[1];
					}
					break;
				default:
					break;
			}
			break;
		case ABSOLUTE_MODE:
			if (buf_count) {
				*wp++ = buf[0];
			}
			break;
		case CONTINUOUS_MODE:
			if (continuous_count) {
				*wp++ = continuous_count;
				*wp++ = value;
			}
			break;
		default:
			break;
	}

	return wp - dst;
}

size_t rle_encode(const uint8_t *src, size_t slen, uint8_t *dst, size_t dlen)
{
	if (!src)  return 0;
	if (!dst)  return 0;
	if (!slen) return 0;
	if (!dlen) return 0;

	return encode(src, slen, dst, dlen);
}

static size_t decode(const uint8_t *src, size_t slen, uint8_t *dst, size_t dlen)
{
	const uint8_t *rp = src;
	uint8_t *wp = dst;
	size_t remain = slen;
	int32_t i;

	while (remain) {
		uint8_t length = *rp++;
		remain -= 1;
		if (length) {
			uint8_t value = *rp++;
			remain -= 1;
			for (i=0; i<length; i++) {
				*wp++ = value;
			}
		} else { /* absolute mode */
			uint8_t abs_length = *rp++;
			remain -= 1;
			for (i=0; i<abs_length; i++) {
				*wp++ = *rp++;
			}
			remain -= abs_length;
		}
	}

	return wp - dst;
}

size_t rle_decode(const uint8_t *src, size_t slen, uint8_t *dst, size_t dlen)
{
	if (!src)  return 0;
	if (!dst)  return 0;
	if (!slen) return 0;
	if (!dlen) return 0;

	return decode(src, slen, dst, dlen);
}

