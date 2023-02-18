#########################################################################
#
# Amoi Tech Mercury Application Build System
# All Rights Reserved 2017
# Author: LuXiao
#########################################################################

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := lib
LOCAL_MODULE := MainApp
LOCAL_PATH := $(CURDIR)
LOCAL_IS_HOST_MODULE := 

#添加源文件搜索路径
MSRCPATH = Source/MainApp
MSRCPATH += Source/MainApp/inc
MSRCPATH += Source/MainApp/src
MSRCPATH += Source/MainApp/include
MSRCPATH += Source/MainApp/include/MQTT
MSRCPATH += Source/MainApp/include/mbedtls
#添加头文件搜索路径
INCPATH = inc  
INCPATH += Source/MainApp
INCPATH += Source/MainApp/inc
INCPATH += Source/MainApp/src
INCPATH += Source/MainApp/include
INCPATH += Source/MainApp/include/MQTT
INCPATH += Source/MainApp/include/mbedtls

LOCAL_C_INCLUDES := $(addprefix -I , $(subst \,/,$(INCPATH)))

vpath
vpath %.h        $(subst \,/,$(INCPATH))
vpath %.c        $(subst \,/,$(MSRCPATH))
vpath %.s        $(subst \,/,$(MSRCPATH))

LOCAL_COMPILER_VARIABLES := DUMMY_MACRO
LOCAL_COMPILER_VARIABLES+=TEST_MACRO1
LOCAL_COMPILER_VARIABLES+=TEST_MACRO2

LOCAL_C_INCLUDES_I = -I ./inc -I ./inc/std_inc  
LOCAL_C_INCLUDES_I += -I ./Source/MainApp
LOCAL_C_INCLUDES_I += -I ./Source/MainApp/inc
LOCAL_C_INCLUDES_I += -I ./Source/MainApp/src
LOCAL_C_INCLUDES_I += -I ./Source/MainApp/include
LOCAL_C_INCLUDES_I += -I ./Source/MainApp/include/MQTT
LOCAL_C_INCLUDES_I += -I ./Source/MainApp/include/mbedtls

SOURCES1 := $(foreach n,$(MSRCPATH),$(wildcard $(n)/*.c))
SOURCES := $(notdir $(SOURCES1))

include $(BUILD_STATIC_LIBRARY)

