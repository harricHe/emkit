#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* handle_t;

handle_t mempool_create(void *memory, size_t memsz);
void  mempool_destroy(handle_t hdl);
void* mempool_allocate(handle_t hdl, size_t size);
void  mempool_free(handle_t hdl, void *block);
void  mempool_destroy(handle_t hdl);
size_t mempool_available(handle_t hdl);
size_t mempool_used(handle_t hdl);
size_t mempool_capacity(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MEMORYPOOL_H__ */

