#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

size_t string_length(const char8 *str);
size_t string_copy(char8 *dst, const char8 *src);
int32_t string_compare(const char8 *str1, const char8 *str2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STRING_H__ */

