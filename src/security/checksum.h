#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t  checksum_calc_8bit(const uint8_t  *data, size_t cnt);
uint16_t checksum_calc_16bit(const uint16_t *data, size_t cnt);
uint32_t checksum_calc_32bit(const uint32_t *data, size_t cnt);
uint64_t checksum_calc_64bit(const uint64_t *data, size_t cnt);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CHECKSUM_H__ */

