LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)


#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)


LOCAL_MODULE := game_shared


LOCAL_MODULE_FILENAME := libgame
define walk
 $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

ALLFILES = $(call walk, $(LOCAL_PATH)/../../sources)

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST += $(filter %.c, $(ALLFILES))
FILE_LIST += $(filter %.cc, $(ALLFILES))


FILE_INCLUDES += $(shell find $(LOCAL_PATH)/../../sources -type d)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../sources \
  $(FILE_INCLUDES) \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../lib/cocos2d-x/cocos2dx \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../lib/cocos2d-x/cocos2dx/platform \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../lib/cocos2d-x/cocos2dx/include \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../lib/cocos2d-x/cocosDenshion/include \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../lib/cocos2d-x/extensions \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../lib/cocos2d-x/external/extra/network

LOCAL_CFLAGS += -D__GXX_EXPERIMENTAL_CXX0X__ -std=gnu++11 -Wno-psabi -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS)

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,lib/proj.android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external) 