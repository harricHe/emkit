$(TARGET_TEST): $(UNITY_OBJECTS) $(OBJECTS) $(LINKLIBS)
	@[ -d $(BUILD_TEST_DIR) ] || mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(LDFLGAS) $(LIBDIRS) -o $@ $(OBJECTS) $(UNITY_OBJECTS) $(LIBS)

$(UNITY_OBJECTS):
	@[ -d $(BUILD_OBJ_DIR) ] || mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_OBJ_DIR)/unity.o -c $(UNITY_ROOT)/src/unity.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_OBJ_DIR)/unity_fixture.o -c $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c

$(BUILD_OBJ_DIR)/%.o:%.c
	@[ -d $(BUILD_OBJ_DIR) ] || mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

-include $(DEPENDS)

