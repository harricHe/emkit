ROOT_DIR := .

include $(ROOT_DIR)/defs.mk

SOURCE_DIR := $(ROOT_DIR)/src
TEST_DIR = $(ROOT_DIR)/test

UNITTESTS = $(addsuffix _test,$(addprefix $(BUILD_TEST_DIR)/,$(TARGETS)))

TARGETS = 
# TARGETS += archive
# TARGETS += bit
TARGETS += container
# TARGETS += framework
# TARGETS += log
TARGETS += memory
# TARGETS += protocol
# TARGETS += search
# TARGETS += security
# TARGETS += sort
# TARGETS += string

.PHONY: all test clean testclean rebuild

all: build-lib build-test

build-lib:
	@for module in $(TARGETS); do \
		$(MAKE) -C $(SOURCE_DIR)/$$module || exit $?; \
	done

build-test:
	@for module in $(TARGETS); do \
		$(MAKE) -C $(TEST_DIR)/$$module || exit $?; \
	done

test:
	@for unittest in $(UNITTESTS); do \
		echo; echo $$unittest; \
		$$unittest || exit $?; \
	done

clean:
	$(RM) $(BUILD_DIR)

clean-test:
	@for module in $(TARGETS); do \
		$(MAKE) -C $(TEST_DIR)/$$module clean || exit $?; \
	done

rebuild: clean all

