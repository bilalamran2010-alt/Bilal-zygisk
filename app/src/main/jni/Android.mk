LOCAL_PATH := $(call my-dir)

# ============================================================================#
# Prebuilt Dobby Library
include $(CLEAR_VARS)
LOCAL_MODULE    := libdobby
# This will automatically pick up arm64-v8a/libdobby.a because we forced 64-bit in Application.mk
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
LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE         := arm
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz

LOCAL_C_INCLUDES       += $(LOCAL_PATH)
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Dobby
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Unity

# Manually including ImGui sources to ensure proper linking
FILE_LIST              := ImGui/imgui.cpp
FILE_LIST              += ImGui/imgui_draw.cpp
FILE_LIST              += ImGui/imgui_widgets.cpp
FILE_LIST              += ImGui/imgui_tables.cpp
FILE_LIST              += ImGui/imgui_impl_android.cpp
FILE_LIST              += ImGui/imgui_impl_opengl3.cpp

# Adding other sources
FILE_LIST              += $(wildcard $(LOCAL_PATH)/xdl/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/KittyMemory/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/LOLX/IL2CppSDKGenerator/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/*.cpp)

# And64InlineHook for 64-bit support
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include/And64InlineHook
HOOK_SRC               := $(wildcard $(LOCAL_PATH)/include/And64InlineHook/*.c*)
FILE_LIST              += $(HOOK_SRC:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES        := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES     := exceptions

include $(BUILD_SHARED_LIBRARY)
