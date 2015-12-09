MODULE := security
ROOT_DIR := ../..

INCLUDE_DIR  = 
INCLUDE_DIR  = .
INCLUDE_DIR += $(ROOT_DIR)/src/$(MODULE)
INCLUDE_DIR += $(ROOT_DIR)/src/common

LIBS  = -l$(MODULE)

SOURCES  = test_security.c
SOURCES += checksum_test.c
# SOURCES += crc_test.c

include $(ROOT_DIR)/mk/defs.mk
include $(ROOT_DIR)/mk/unity.mk

.PHONY: all
all: $(TARGET_TEST)

clean:
	$(RM) $(addprefix $(BUILD_OBJ_DIR)/,$(SOURCES:.c=.o))
	$(RM) $(addprefix $(BUILD_OBJ_DIR)/,$(SOURCES:.c=.d))
	$(RM) $(TARGET_TEST)

include $(ROOT_DIR)/mk/rules-test.mk


