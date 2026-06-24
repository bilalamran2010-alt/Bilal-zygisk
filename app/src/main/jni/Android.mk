LOCAL_PATH := $(call my-dir)

# ============================================================================#
include $(CLEAR_VARS)
LOCAL_MODULE    := libdobby
LOCAL_SRC_FILES := Dobby/armeabi-v7a/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE           := LOL
LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17 -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS          := -Wl,--gc-sections,--strip-all -llog
LOCAL_ARM_MODE         := arm

CURL_ROOT              := $(LOCAL_PATH)/LOLX/Tools/curl/curl-android-armeabi-v7a
SSL_ROOT               := $(LOCAL_PATH)/LOLX/Tools/curl/openssl-android-armeabi-v7a

LOCAL_C_INCLUDES       := $(LOCAL_PATH)
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include/Substrate
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Dobby
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Unity
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Struct
LOCAL_C_INCLUDES       += $(CURL_ROOT)/include
LOCAL_C_INCLUDES       += $(SSL_ROOT)/include

LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz
LOCAL_LDLIBS           += -latomic

LOCAL_LDFLAGS          += $(CURL_ROOT)/lib/libcurl.a
LOCAL_LDFLAGS          += $(SSL_ROOT)/lib/libssl.a
LOCAL_LDFLAGS          += $(SSL_ROOT)/lib/libcrypto.a

FILE_LIST              := ImGui/imgui.cpp ImGui/imgui_draw.cpp ImGui/imgui_widgets.cpp ImGui/imgui_tables.cpp ImGui/imgui_impl_android.cpp ImGui/imgui_impl_opengl3.cpp
FILE_LIST              += $(wildcard $(LOCAL_PATH)/xdl/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/KittyMemory/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/LOLX/IL2CppSDKGenerator/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/LOLX/Tools/*.cpp)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SRC_FILES        := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES     := exceptions

include $(BUILD_SHARED_LIBRARY)
