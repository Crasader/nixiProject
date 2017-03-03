LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := entryexpro_shared

LOCAL_SRC_FILES := libentryexpro.so

include $(PREBUILT_SHARED_LIBRARY)