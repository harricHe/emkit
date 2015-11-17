#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_QUEUE_HANDLES (2)

handle_t queue_create(void *memory, size_t qsz, size_t qcnt);
error_t queue_destroy(handle_t hdl);
error_t queue_write(handle_t hdl, const void *data);
error_t queue_read(handle_t hdl, void *data);
void* queue_get(handle_t hdl);
size_t queue_available(handle_t hdl);
size_t queue_used(handle_t hdl);
size_t queue_capacity(handle_t hdl);
error_t queue_purge(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __QUEUE_H__ */

