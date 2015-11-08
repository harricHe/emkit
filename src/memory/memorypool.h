#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_MEMORYPOOLS (10)

handle_t mempool_create(void *memory, size_t memsz);
error_t mempool_destroy(handle_t hdl);
void* mempool_allocate(handle_t hdl, size_t size);
error_t mempool_free(handle_t hdl, void *block);
size_t mempool_available(handle_t hdl);
size_t mempool_used(handle_t hdl);
size_t mempool_capacity(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MEMORYPOOL_H__ */

