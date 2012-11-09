LOCAL_PATH := $(call my-dir)

#
# input_event_recorder
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES := armAsmInline.c
LOCAL_MODULE := armTest
LOCAL_STATIC_LIBRARIES := libcutils libc

include $(BUILD_EXECUTABLE)

