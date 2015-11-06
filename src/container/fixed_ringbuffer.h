#ifndef __FIXED_RINGBUFFER_H__
#define __FIXED_RINGBUFFER_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_FIXEDRINGBUFFERS (3)

handle_t fixedringbuf_create(void *memory, size_t blksz, size_t blkcnt);
error_t fixedringbuf_destroy(handle_t hdl);
error_t fixedringbuf_write(handle_t hdl, const void *block);
error_t fixedringbuf_read(handle_t hdl, void *block);
void* fixedringbuf_get(handle_t hdl);
size_t fixedringbuf_available(handle_t hdl);
size_t fixedringbuf_used(handle_t hdl);
size_t fixedringbuf_capacity(handle_t hdl);
error_t fixedringbuf_purge(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FIXED_RINGBUFFER_H__ */

