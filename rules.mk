$(TARGET_LIB): $(OBJECTS)
	@[ -d $(BUILD_LIB_DIR) ] || mkdir -p $(BUILD_LIB_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(TARGET_TEST): $(OBJECTS)
	@[ -d $(BUILD_TEST_DIR) ] || mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(LDFLGAS) $(LIBDIRS) $(LIBS) -o $@ $(OBJECTS)

$(BUILD_OBJ_DIR)/%.o:%.c
	@[ -d $(BUILD_OBJ_DIR) ] || mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

-include $(DEPENDS)

