#ifndef __QUICKSORT_H__
#define __QUICKSORT_H__

#include "types.h"
#include "sort.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void quick_sort(const void *src, void *dst, size_t size,
		compfunc_t comp, iterfunc_t iter);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __QUICKSORT_H__ */

