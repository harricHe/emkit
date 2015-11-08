#ifndef __SORT_H__
#define __SORT_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int32_t (*compfunc_t)(const void *a, const void *b);
typedef void* (*iterfunc_t)(const void *v);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SORT_H__ */

