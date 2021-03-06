
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	
	JNI_PATH := $(abspath $(call my-dir))
	LOCAL_PATH := $(abspath $(call my-dir))
	
#load NE10
 	include $(CLEAR_VARS)
	NE10_LIB_PATH := $(JNI_PATH)/../../Ne10
	LOCAL_MODULE := NE10
	LOCAL_EXPORT_C_INCLUDES := $(NE10_LIB_PATH)/inc
	LOCAL_SRC_FILES := $(NE10_LIB_PATH)/lib/libNE10.a
	include $(PREBUILT_STATIC_LIBRARY)
 	
#load Tapir	
	include $(CLEAR_VARS)
	TAPIR_LIB_PATH = $(JNI_PATH)/../../tapir-build
	LOCAL_MODULE := Tapir
	
	LOCAL_EXPORT_C_INCLUDES := $(TAPIR_LIB_PATH)/include
	LOCAL_SRC_FILES := $(TAPIR_LIB_PATH)/lib/libtapir.a
	include $(PREBUILT_STATIC_LIBRARY)
	
	
	include $(CLEAR_VARS)	
	
	LOCAL_MODULE    := TapirReceiver
	LOCAL_SRC_FILES := TapirReceiver.cpp AudioInputAccessor.cpp
	LOCAL_STATIC_LIBRARIES := Tapir NE10 neon_utils cpufeatures 
	LOCAL_ARM_NEON  := true
	LOCAL_C_INCLUDES += $(NDK_PATH)/sources/cpufeatures
	LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
	LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true
	LOCAL_LDLIBS  += -lOpenSLES -llog -landroid
	
	include $(BUILD_SHARED_LIBRARY)
endif