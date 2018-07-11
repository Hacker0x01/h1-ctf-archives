LOCAL_PATH := $(call my-dir)

FLAGS_INCLUDE := -I$(LOCAL_PATH)

FLAGS_COMMON := $(FLAGS_INCLUDE) -O3 -D_GNU_SOURCE -static \
    -DPB_NO_ERRMSG=1 -DPB_FIELD_16BIT=1 \
    -fvisibility=hidden -fPIE

FLAGS_CPP_COMMON := -std=c++11 -fvisibility-inlines-hidden -fexceptions

ifeq ($(NDEBUG),1)
    FLAGS_COMMON += -DDEBUG
endif

####################################################
# Android specific Unit Tests for Security Library - uses GTest
####################################################
include $(CLEAR_VARS)

LOCAL_MODULE := super-dooper
LOCAL_CPPFLAGS  := $(FLAGS_COMMON) $(FLAGS_CPP_COMMON)
LOCAL_CFLAGS  := $(FLAGS_COMMON)

LOCAL_SRC_FILES := md5.obf.cpp SeverThing.obf.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_EXECUTABLE)

