#include "checksum.h"
#include <string.h>

uint8_t checksum_calc_8bit(const uint8_t  *data, size_t cnt)
{
	uint8_t sum = 0;
	const uint8_t *p = data;
	int32_t i;
	for (i=0; i<(int32_t)cnt; i++) {
		sum += *p++;
	}
	return sum;
}

uint16_t checksum_calc_16bit(const uint16_t *data, size_t cnt)
{
	uint16_t sum = 0;
	const uint16_t *p = data;
	int32_t i;
	for (i=0; i<(int32_t)cnt; i++) {
		sum += *p++;
	}
	return sum;
}

uint32_t checksum_calc_32bit(const uint32_t *data, size_t cnt)
{
	uint32_t sum = 0;
	const uint32_t *p = data;
	int32_t i;
	for (i=0; i<(int32_t)cnt; i++) {
		sum += *p++;;
	}
	return sum;
}

uint64_t checksum_calc_64bit(const uint64_t *data, size_t cnt)
{
	uint64_t sum = 0;
	const uint64_t *p = data;
	int32_t i;
	for (i=0; i<(int32_t)cnt; i++) {
		sum += *p++;;
	}
	return sum;
}

