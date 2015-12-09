RM := rm -rf
CC ?= gcc
AS := as
AR := ar
LD := ld
STRIP := strip
CFLAGS  = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -Wpointer-arith
# CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wmissing-declarations
CFLAGS += -Wno-unused-parameter
# CFLAGS += -Wfloat-equal
CFLAGS += -pedantic-errors
CFLAGS += -g -O0
# CFLAGS += -O3 -flto
CFLAGS += -MMD -MP
ARFLAGS := crsv
LDFLGAS := 

BUILD_DIR = $(ROOT_DIR)/_build

BUILD_OBJ_DIR := $(BUILD_DIR)/obj
BUILD_LIB_DIR := $(BUILD_DIR)/lib
BUILD_TEST_DIR := $(BUILD_DIR)/test
INCLUDES = $(addprefix -I,$(INCLUDE_DIR))
LIBDIRS = $(addprefix -L,$(BUILD_LIB_DIR))

OBJECTS = $(addprefix $(BUILD_OBJ_DIR)/,$($(notdir SOURCES):.c=.o))
DEPENDS = $(OBJECTS:.o=.d)
LINKLIBS = $(BUILD_LIB_DIR)/$(call libname,$(MODULE))

TARGET_LIB  = $(BUILD_LIB_DIR)/$(call libname,$(MODULE))
TARGET_TEST = $(BUILD_TEST_DIR)/$(MODULE)_test

# functions definitions
modulename = $(basename $(notdir $(1)))
libname = lib$(1).a

# module dependent values
MODULE ?= 
ROOT_DIR ?= .
INCLUDE_DIR ?= $(ROOT_DIR)/src/common

