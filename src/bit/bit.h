#ifndef __BIT_H__
#define __BIT_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

size_t bit_count(uint32 v);
uint8  bit_reverse8(uint8 v);
uint16 bit_reverse16(uint16 v);
uint32 bit_reverse32(uint32 v);
uint64 bit_reverse64(uint64 v);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BIT_H__ */

