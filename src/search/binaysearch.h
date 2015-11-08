#ifndef __BINAYSEARCH_H__
#define __BINAYSEARCH_H__

#include "types.h"
#include "sort.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32 binarysearch(const void *key, const void *array, size_t size,
		compfunc_t comp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BINAYSEARCH_H__ */

