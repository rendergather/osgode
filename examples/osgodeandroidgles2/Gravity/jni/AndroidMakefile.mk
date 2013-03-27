LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := nativelib
LOCAL_SRC_FILES  := Application.cpp Scene.cpp Viewer.cpp nativelib.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS     := -lGLESv2 -llog -ldl -lz
LOCAL_CPPFLAGS   := -DOSG_LIBRARY_STATIC -DdSINGLE
LOCAL_LDFLAGS := -L $(LOCAL_PATH)/lib/armeabi/osg -L $(LOCAL_PATH)/lib/armeabi \
-losgODEUtil \
-losgODE \
-losgViewer \
-losgGA \
-losgDB \
-losgUtil \
-losg \
-lOpenThreads \
-lode \
-lode_joints \
-lOPCODE \
-lIce \
-lccd \
-lgnustl_static

include $(BUILD_SHARED_LIBRARY)
