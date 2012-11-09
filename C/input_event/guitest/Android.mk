LOCAL_PATH := $(call my-dir)

#
# input_event_recorder
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES := input_event_recorder.c 
LOCAL_MODULE := input_event_recorder
LOCAL_STATIC_LIBRARIES := libcutils libc

include $(BUILD_EXECUTABLE)


#
# input_event_player
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES :=  input_event_player.c uinput.c uinput.h
LOCAL_CFLAGS += -DHAS_LINUX_UINPUT_H 
LOCAL_MODULE := input_event_player
LOCAL_STATIC_LIBRARIES := libcutils libc

include $(BUILD_EXECUTABLE)


