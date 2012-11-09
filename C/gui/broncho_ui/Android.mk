LOCAL_PATH:= $(call my-dir)

ifneq ($(TARGET_SIMULATOR),true)

include $(CLEAR_VARS)
 
LOCAL_SRC_FILES:= \
		gui/ft_button.c \
		gui/ft_status_button.c \
		gui/ft_draw.c \
		gui/ft_event.c \
		gui/ft_input.c \
		gui/ft_framebuffer.c \
		gui/ft_list.c \
		gui/ft_widget.c \
		gui/ft_window.c \
		ft_notepad.c \
		ft_config.c \
		main.c
 
LOCAL_MODULE := factory_test
LOCAL_CFLAGS := \
	-fPIC -D_POSIX_SOURCE 

LOCAL_C_INCLUDES += external/icu4c/common

LOCAL_SHARED_LIBRARIES += libcutils libicuuc

include $(BUILD_EXECUTABLE)

endif
