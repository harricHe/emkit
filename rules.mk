$(TARGET_LIB): $(OBJECTS)
	@[ -d $(BUILD_LIB_DIR) ] || mkdir -p $(BUILD_LIB_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(TARGET_TEST): $(UNITY_OBJECTS) $(OBJECTS)
	@[ -d $(BUILD_TEST_DIR) ] || mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(LDFLGAS) $(LIBDIRS) $(LIBS) -o $@ $(OBJECTS) $(UNITY_OBJECTS)

$(UNITY_OBJECTS):
	@[ -d $(BUILD_OBJ_DIR) ] || mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_OBJ_DIR)/unity.o -c $(UNITY_ROOT)/src/unity.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_OBJ_DIR)/unity_fixture.o -c $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c

$(BUILD_OBJ_DIR)/%.o:%.c
	@[ -d $(BUILD_OBJ_DIR) ] || mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

-include $(DEPENDS)

