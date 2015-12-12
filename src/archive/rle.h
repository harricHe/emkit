#ifndef __RLE_H__
#define __RLE_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

size_t rle_encode(const uint8_t *src, size_t slen, uint8_t *dst);
size_t rle_decode(const uint8_t *src, size_t slen, uint8_t *dst);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RLE_H__ */

