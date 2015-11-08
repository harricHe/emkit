#ifndef __MAP_H__
#define __MAP_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* handle_t;

handle_t map_create();
error_t map_destroy(handle_t hdl);
void* map_get(handle_t hdl, uint32 key);
error_t map_set(handle_t hdl, uint32 key, void *value);
void* map_remove(handle_t hdl, uint32 key);
size_t map_capacity(handle_t hdl);
size_t map_available(handle_t hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MAP_H__ */

