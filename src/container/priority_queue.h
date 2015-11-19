#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_NUMOF_PRIORITY_QUEUE_HANDLES (2)

handle_t priqueue_create(void *memory, uint32_t pricnt, size_t qsz, size_t qcnt);
error_t priqueue_destroy(handle_t hdl);
error_t priqueue_write(handle_t hdl, uint32_t pri, const void *data);
error_t priqueue_read(handle_t hdl, void *data);
error_t priqueue_read_with_priority(handle_t hdl, uint32_t pri, void *data);
void* priqueue_get(handle_t hdl);
void* priqueue_get_with_priority(handle_t hdl, uint32_t pri);
size_t priqueue_available(handle_t hdl, uint32_t pri);
size_t priqueue_used(handle_t hdl, uint32_t pri);
size_t priqueue_capacity(handle_t hdl, uint32_t pri);
error_t priqueue_purge(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PRIORITY_QUEUE_H__ */


