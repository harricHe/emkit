$(TARGET_LIB): $(OBJECTS) $(BUILD_LIB_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(TARGET_TEST): $(OBJECTS) $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLGAS) $(LIBDIRS) $(LIBS) -o $@ $(OBJECTS)

$(BUILD_OBJ_DIR)/%.o:%.c $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLGAS) -o $@ -c $<

$(BUILD_OBJ_DIR):
	mkdir -p $@

$(BUILD_LIB_DIR):
	mkdir -p $@

$(BUILD_TEST_DIR):
	mkdir -p $@

-include $(DEPENDS)

