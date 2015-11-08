ROOT_DIR := .

include $(ROOT_DIR)/defs.mk

SOURCE_DIR := $(ROOT_DIR)/src
TEST_DIR = $(ROOT_DIR)/test

TARGET_LIBS = $(addprefix $(BUILD_LIB_DIR)/,$(addsuffix .a,$(TARGETS)))
TARGET_TESTS = $(addprefix $(BUILD_TEST_DIR)/,$(addsuffix .a,$(TARGETS)))

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

.PHONY: all test clean testclean rebuild

all:
	@for module in $(TARGETS); do \
		$(MAKE) -C $(SOURCE_DIR)/$$module || exit $?; \
	done

test: all
	@for module in $(TARGETS); do \
		$(MAKE) -C $(TEST_DIR)/$$module || exit $?; \
	done
	# TODO: execute unit tests

clean:
	$(RM) $(BUILD_DIR)

testclean:
	@for module in $(TARGETS); do \
		$(MAKE) -C $(TEST_DIR)/$$module clean || exit $?; \
	done

rebuild: clean all

