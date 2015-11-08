#ifndef __MERGESORT_H__
#define __MERGESORT_H__

#include "types.h"
#include "sort.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void merge_sort(const void *src, void *dst, size_t size,
		compfunc_t comp, iterfunc_t iter);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MERGESORT_H__ */

