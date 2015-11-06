#ifndef __FIXED_MEMORYPOOL_H__
#define __FIXED_MEMORYPOOL_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_FIXEDMEMORYPOOLS (10)

handle_t fixedmpool_create(void *memory, size_t blksz, size_t blkcnt);
error_t fixedmpool_destroy(handle_t hdl);
void* fixedmpool_allocate(handle_t hdl);
error_t fixedmpool_free(handle_t hdl, void *block);
size_t fixedmpool_available_blocks(handle_t hdl);
size_t fixedmpool_used_blocks(handle_t hdl);
size_t fixedmpool_blockdata_size(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FIXED_MEMORYPOOL_H__ */

