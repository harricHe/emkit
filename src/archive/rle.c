#include "rle.h"

/* run length encoding */

static size_t encode(const uint8_t *src, size_t slen, uint8_t *dst, size_t dlen)
{
	const uint8_t *rp = src;
	uint8_t *wp = dst;
	size_t dstlen = 0;

	uint8_t value = *rp++;
	uint8_t continuous_count = 1;
	size_t  remain = slen - 1;

	while (remain--) {
		if ( (value != *rp) || (continuous_count == 0xFF) ) {
			*wp++ = continuous_count;
			*wp++ = value;
			dstlen += 2;
			value = *rp++;
			continuous_count = 1;
		} else {
			rp++;
			continuous_count++;
		}
		if (dstlen >= dlen) {
			/* encoding data size become bigger than original data size */
			return 0;
		}
	}

	*wp++ = continuous_count;
	*wp = value;
	dstlen += 2;

	return dstlen;
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
	size_t dstlen = 0;
	size_t remain = slen;

	while (remain) {
		uint8_t length = *rp++;
		uint8_t value  = *rp++;
		remain -= 2;
		while (length--) {
			*wp++ = value;
			dstlen++;
		}
		if (dstlen >= dlen) {
			break;
		}
	}

	return dstlen;
}

size_t rle_decode(const uint8_t *src, size_t slen, uint8_t *dst, size_t dlen)
{
	if (!src)  return 0;
	if (!dst)  return 0;
	if (!slen) return 0;
	if (!dlen) return 0;

	return decode(src, slen, dst, dlen);
}

