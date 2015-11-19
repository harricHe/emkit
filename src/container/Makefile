MODULE := container
ROOT_DIR := ../..

INCLUDE_DIR  = 
INCLUDE_DIR  = .
INCLUDE_DIR += $(ROOT_DIR)/src/common

SOURCES  = 
SOURCES += ringbuffer.c
SOURCES += fixed_ringbuffer.c
# SOURCES += graph.c
# SOURCES += list.c
# SOURCES += map.c
SOURCES += queue.c
SOURCES += priority_queue.c
SOURCES += stack.c
# SOURCES += tree.c

include $(ROOT_DIR)/mk/defs.mk

.PHONY: all
all: $(TARGET_LIB)

include $(ROOT_DIR)/mk/rules-lib.mk

