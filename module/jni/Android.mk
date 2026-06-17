LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := LAZ
LOCAL_CPPFLAGS  := -std=c++17 -fexceptions -frtti -fpermissive
LOCAL_LDLIBS    := -llog -landroid -lGLESv2 -lEGL

LOCAL_SRC_FILES := \
    Main.cpp \
    KittyMemory/KittyMemory.cpp \
    KittyMemory/KittyArm64.cpp \
    KittyMemory/KittyScanner.cpp \
    KittyMemory/KittyUtils.cpp \
    KittyMemory/MemoryBackup.cpp \
    KittyMemory/MemoryPatch.cpp \
    LOLX/IL2CppSDKGenerator/Il2Cpp.cpp \
    LOLX/Tools/Tools.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/KittyMemory \
                    $(LOCAL_PATH)/LOLX \
                    $(LOCAL_PATH)/LOLX/Tools \
                    $(LOCAL_PATH)/Struct

LOCAL_STATIC_LIBRARIES := libdobby

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/LOLX/Tools/Dobby/libraries)
include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := LOLX/Tools/Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)
