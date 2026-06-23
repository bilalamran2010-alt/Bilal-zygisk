LOCAL_PATH := $(call my-dir)

# ============================================================================#
# Prebuilt Dobby Library
include $(CLEAR_VARS)
LOCAL_MODULE    := libdobby
LOCAL_SRC_FILES := Dobby/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE           := LOL

# Flags and Includes
LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS           += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS         += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all -llog
LOCAL_ARM_MODE         := arm

# Dependencies Path
DEPS_PATH              := $(LOCAL_PATH)/LOLX/Tools/curl

# Includes for CURL and OpenSSL
LOCAL_C_INCLUDES       += $(DEPS_PATH)/curl-android-armeabi-v7a/include
LOCAL_C_INCLUDES       += $(DEPS_PATH)/openssl-android-armeabi-v7a/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Dobby
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Unity
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include/And64InlineHook

# Libraries
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz
LOCAL_LDLIBS           += -L$(DEPS_PATH)/curl-android-armeabi-v7a/lib -lcurl
LOCAL_LDLIBS           += -L$(DEPS_PATH)/openssl-android-armeabi-v7a/lib -lssl -lcrypto

# Sources
FILE_LIST              := ImGui/imgui.cpp ImGui/imgui_draw.cpp ImGui/imgui_widgets.cpp ImGui/imgui_tables.cpp ImGui/imgui_impl_android.cpp ImGui/imgui_impl_opengl3.cpp
FILE_LIST              += $(wildcard $(LOCAL_PATH)/xdl/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/KittyMemory/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/LOLX/IL2CppSDKGenerator/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/*.cpp)

# Conditionally include And64InlineHook only for aarch64
ifneq ($(TARGET_ARCH_ABI),armeabi-v7a)
    HOOK_SRC           := $(wildcard $(LOCAL_PATH)/include/And64InlineHook/*.c*)
    FILE_LIST          += $(HOOK_SRC:$(LOCAL_PATH)/%=%)
endif

LOCAL_SRC_FILES        := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES     := exceptions

include $(BUILD_SHARED_LIBRARY)
