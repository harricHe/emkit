#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_RINGBUFFERS (3)

handle_t ringbuf_create(void *memory, size_t size);
error_t ringbuf_destroy(handle_t hdl);
error_t ringbuf_write(handle_t hdl, const void *data, size_t size);
size_t ringbuf_read(handle_t hdl, void *data, size_t size);
size_t ringbuf_read_to(handle_t hdl, void *data, size_t size, uint8_t token);
size_t ringbuf_available(handle_t hdl);
size_t ringbuf_used(handle_t hdl);
size_t ringbuf_capacity(handle_t hdl);
error_t ringbuf_purge(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RINGBUFFER_H__ */

