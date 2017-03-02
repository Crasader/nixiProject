LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := uptsmaddon_shared

LOCAL_SRC_FILES := libuptsmaddon.so

include $(PREBUILT_SHARED_LIBRARY)