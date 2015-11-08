.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS) $(BUILD_LIB_DIR)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJECTS)

$(BUILD_OBJ_DIR)/%.o:%.c $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLGAS) -o $@ -c $<

$(BUILD_OBJ_DIR):
	mkdir -p $@

$(BUILD_LIB_DIR):
	mkdir -p $@

-include $(DEPENDS)

