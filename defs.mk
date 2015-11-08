RM := rm -rf
CC := gcc
AR := ar
CFLAGS := -g -Wall -Wextra -Winit-self -Wno-unused-parameter -Wfloat-equal
CFLAGS += -MMD -MP
ARFLAGS := crsv
LDFLGAS := 

BUILD_DIR = $(ROOT_DIR)/_build

BUILD_OBJ_DIR := $(BUILD_DIR)/obj
BUILD_LIB_DIR := $(BUILD_DIR)/lib
BUILD_TEST_DIR := $(BUILD_DIR)/test
INCLUDES = $(addprefix -I,$(INCLUDE_DIR))

OBJECTS = $(addprefix $(BUILD_OBJ_DIR)/,$(SOURCES:.c=.o))
DEPENDS = $(OBJECTS:.o=.d)
TARGET  = $(BUILD_LIB_DIR)/lib$(MODULE).a

# functions definitions
file_basename = $(basename $(notdir $(1)))

# module dependent values
MODULE ?= 
ROOT_DIR ?= .
INCLUDE_DIR ?= $(ROOT_DIR)/src/common

