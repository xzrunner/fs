INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := fs

LOCAL_C_INCLUDES  := \
	${LUA_SRC_PATH} \
	${ZIP_SRC_PATH} \
	${LOGGER_SRC_PATH} \

LOCAL_SRC_FILES := \
	$/fs_android_helper.c \
	$/fs_file_android.c \
	$/lfs_file.c \

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)