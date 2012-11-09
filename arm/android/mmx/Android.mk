LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= mmx.c mmx_test.S
LOCAL_MODULE := mmx
LOCAL_CFLAGS := -mcpu=iwmmxt -mtune=iwmmxt
LOCAL_STATIC_LIBRARIES := libcutils libc
include $(BUILD_EXECUTABLE)

