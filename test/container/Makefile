MODULE := container
ROOT_DIR := ../..

INCLUDE_DIR  = 
INCLUDE_DIR  = .
INCLUDE_DIR += $(ROOT_DIR)/src/$(MODULE)
INCLUDE_DIR += $(ROOT_DIR)/src/common

LIBS = -l$(MODULE)

SOURCES  = main.c
SOURCES += fixed_ringbuffer_test.c

include $(ROOT_DIR)/defs.mk
include $(ROOT_DIR)/unity.mk

.PHONY: all
all: $(TARGET_TEST)

clean:
	$(RM) $(addprefix $(BUILD_OBJ_DIR)/,$(SOURCES:.c=.o))
	$(RM) $(addprefix $(BUILD_OBJ_DIR)/,$(SOURCES:.c=.d))
	$(RM) $(TARGET_TEST)

include $(ROOT_DIR)/rules.mk

