#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

size_t text_words_num(char *string, size_t len, const char *separators);
char* text_separate(char *string, size_t len, const char *separators);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STRING_H__ */

