RM := rm -rf
CC ?= gcc
AR := ar
CFLAGS := -g -Wall -Wextra -Winit-self -Wno-unused-parameter -Wfloat-equal
CFLAGS += -pedantic-errors
CFLAGS += -MMD -MP
ARFLAGS := crsv
LDFLGAS := 

BUILD_DIR = $(ROOT_DIR)/_build

BUILD_OBJ_DIR := $(BUILD_DIR)/obj
BUILD_LIB_DIR := $(BUILD_DIR)/lib
BUILD_TEST_DIR := $(BUILD_DIR)/test
INCLUDES = $(addprefix -I,$(INCLUDE_DIR))
LIBDIRS = $(addprefix -L,$(BUILD_LIB_DIR))

OBJECTS = $(addprefix $(BUILD_OBJ_DIR)/,$(SOURCES:.c=.o))
DEPENDS = $(OBJECTS:.o=.d)
TARGET_LIB  = $(BUILD_LIB_DIR)/$(call libname,$(MODULE))
TARGET_TEST = $(BUILD_TEST_DIR)/$(MODULE)_test

# functions definitions
modulename = $(basename $(notdir $(1)))
libname = lib$(1).a

# module dependent values
MODULE ?= 
ROOT_DIR ?= .
INCLUDE_DIR ?= $(ROOT_DIR)/src/common

