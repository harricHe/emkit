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

	const uint8_t *sp = rp;
	uint8_t count = 0;
	size_t  remain = slen;

#define IDLE_MODE       (0)
#define ABSOLUTE_MODE   (1)
#define CONTINUOUS_MODE (2)
	uint8_t mode = IDLE_MODE;

	while (remain--) {
		switch (mode) {
			case IDLE_MODE:
				if (count == 2) {
					if (sp[0] == sp[1]) {
						if (sp[0] == sp[2]) {
							/* A A A */
							count++;
							mode = CONTINUOUS_MODE;
						} else {
							/* A A B */
							*wp++ = 2;
							*wp++ = sp[0];
							sp = &sp[2];
							count = 1;
						}
					} else {
						if (sp[1] == sp[2]) {
							/* A B B */
							*wp++ = 1;
							*wp++ = sp[0];
							sp = &sp[1];
							count = 2;
							mode = CONTINUOUS_MODE;
						} else {
							/* A B C */
							mode = ABSOLUTE_MODE;
							count = 3;
						}
					}
				} else {
					count++;
				}
				break;
			case ABSOLUTE_MODE:
				if (sp[count-1] == sp[count]) {
					/* ... A B B */
					/* break */
					int32_t i;
					*wp++ = 0;
					*wp++ = count - 1;
					for (i=0; i<count-1; i++) {
						*wp++ = sp[i];
					}
					sp = &sp[count-1];
					count = 2;
					mode = CONTINUOUS_MODE;
				} else {
					count++;
				}
				break;
			case CONTINUOUS_MODE:
				if (sp[count-1] != sp[count]) {
					/* ... A A B */
					/* break */
					*wp++ = count;
					*wp++ = sp[0];
					sp = &sp[count];
					count = 1;
					mode = IDLE_MODE;
				} else {
					count++;
				}
				break;
			default:
				break;
		}
		rp++;
	}

	switch (mode) {
		case IDLE_MODE:
			switch (count) {
				case 1:
					*wp++ = 1;
					*wp++ = sp[0];
					break;
				case 2:
					if (sp[0] == sp[1]) {
						*wp++ = 2;
						*wp++ = sp[0];
					} else {
						*wp++ = 0;
						*wp++ = 2;
						*wp++ = sp[0];
						*wp++ = sp[1];
					}
					break;
				default:
					break;
			}
			break;
		case ABSOLUTE_MODE:
			{
				int32_t i;
				*wp++ = 0;
				*wp++ = count;
				for (i=0; i<count; i++) {
					*wp++ = sp[i];
				}
			}
			break;
		case CONTINUOUS_MODE:
			*wp++ = count;
			*wp++ = sp[0];
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

