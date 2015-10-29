#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* handle_t;

handle_t fixedmempool_create(void *memory, size_t blksz, size_t blkcnt);
void  fixedmempool_destroy(handle_t hdl);
void* fixedmempool_allocate(handle_t hdl);
void  fixedmempool_free(handle_t hdl, void *block);
size_t fixedmempool_available(handle_t hdl);
size_t fixedmempool_used(handle_t hdl);
size_t fixedmempool_capacity(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MEMORYPOOL_H__ */


