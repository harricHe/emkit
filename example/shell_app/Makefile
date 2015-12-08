EXAMPLE = shell_app
ROOT_DIR := ../..

INCLUDE_DIR  = .
INCLUDE_DIR += $(ROOT_DIR)/src/common
INCLUDE_DIR += $(ROOT_DIR)/src/framework
INCLUDE_DIR += $(ROOT_DIR)/src/text

SOURCES  = shell_app.c

LIBS  = -lframework
LIBS += -ltext

include $(ROOT_DIR)/mk/defs.mk

BUILD_EXAMPLE_DIR = _build
TARGET_EXAMPLE = $(BUILD_EXAMPLE_DIR)/$(EXAMPLE)

.PHONY: all
all: $(TARGET_EXAMPLE)

$(TARGET_EXAMPLE):
	@[ -d $(BUILD_EXAMPLE_DIR) ] || mkdir -p $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) $(LDFLGAS) $(LIBDIRS) $(LIBS) -o $@

clean:
	$(RM) $(BUILD_EXAMPLE_DIR)

