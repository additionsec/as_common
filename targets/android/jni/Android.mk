LOCAL_PATH := $(call my-dir)

###########################################

include $(CLEAR_VARS)

LOCAL_MODULE := libas_common

include $(LOCAL_PATH)/Core.mk

LOCAL_CFLAGS := $(MCFLAGS) $(MDEFS) $(MINCS)
LOCAL_SRC_FILES := $(MSRC)

include $(BUILD_STATIC_LIBRARY)

###########################################
