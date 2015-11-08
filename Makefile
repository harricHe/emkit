ROOT_DIR := .

include $(ROOT_DIR)/defs.mk

SOURCE_DIR := $(ROOT_DIR)/src
# TEST_DIR = $(ROOT_DIR)/test
TARGET_DIR = $(addprefix $(SOURCE_DIR)/,$(TARGETS))
LIBS = $(addprefix $(BUILD_LIB_DIR)/,$(addsuffix .a,$(TARGETS)))
# TESTS = $(addprefix $(BUILD_TEST_DIR)/,$(addsuffix .a,$(TARGETS)))

TARGETS = 
# TARGETS += archive
# TARGETS += bit
# TARGETS += container
# TARGETS += framework
# TARGETS += log
TARGETS += memory
# TARGETS += protocol
# TARGETS += search
# TARGETS += security
# TARGETS += sort
# TARGETS += string

.PHONY: all
all: $(LIBS)

$(LIBS):
	$(MAKE) -C $(SOURCE_DIR)/$(call file_basename,$@)

# .PHONY: test
# test:
# 	$(MAKE) -C $(TEST_DIR)

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)

