export CT_DEVICE := $(DEVICE)
export CT_TARGET := $(PLATFORM_NAME)
export CT_RELEASE := $(TARGET_BUILD_TYPE)
export ECHO := echo
export OBJCOPY := $(OBJCOPY)
export LD := $(LINKER)
export CP := cp -f
export MAKEFLAGS += -s --no-print-directory

PACGEN := ${TOPDIR}/prebuilts/win32/pacgen.py

export FULL_LIBRARY_PATH := -L${TOPDIR}/build/${CT_DEVICE}/${CT_RELEASE}/lib
FULL_LIBRARY_PATH += -L${TOPDIR}/SysLib/${CT_RELEASE}
FULL_LIBRARY_PATH += -L${TOPDIR}/prebuilts/win32/gcc-arm-none-eabi/lib/gcc/arm-none-eabi/7.2.1

###############################################################################
# Generic environment stuff
###############################################################################
## Generic directory names
export BUILD_DIR := build
export OBJ_DIR := obj
export SRC_DIR := src
export LIB_DIR := lib
export INC_DIR := inc

HEX_DIR := build/${CT_DEVICE}/${CT_RELEASE}/out

export BUILD_ROOT := ${TOPDIR}/${BUILD_DIR}
export HEX_ROOT := ${TOPDIR}/${HEX_DIR}


# Allow the objects of different release types to coexist with each other
export TARGET_TAG := 

BUILD_VERSION_STRING ?= $(shell perl ${TOPDIR}/make/make_cmd/mercury_getversion.pl ${TOPDIR}/version/version.h)
#$(warning  $(BUILD_VERSION_STRING))

export MAJORVER ?= $(word 1, $(BUILD_VERSION_STRING))
#$(warning  $(MAJORVER))

export MINORVER ?= $(word 2, $(BUILD_VERSION_STRING))
#$(warning  $(MINORVER))

export BUILDID ?= $(word 3, $(BUILD_VERSION_STRING))


#################################
## Include path generation ######
#################################

# Root include directory
ROOT_INCLUDE	:= ${TOPDIR}/${INC_DIR}
# Final include path
INCLUDE_PATH	:= ${MYINCLUDEDIR} -I${ROOT_INCLUDE} -I${ROOT_INCLUDE}/private

##################################################################################
## Linker flags
##################################################################################
MYLIBFILES = $(foreach modlib,$(MODULES),-l$(modlib))
MYLIBFILES += -lgcc
ifeq "${BUILD_OPERATOR}" "CUSTOMER_BUILD"
MYLIBFILES += -lsystem
endif
OFORMAT 	:= --oformat=elf32-littlearm
BINARY		:= --input-target=elf32-littlearm --output-target=binary
##################################################################################
## Compile flags
# Some of those are exported recursively, other not.
##################################################################################

# Assembly compilation flags -hard
#ASFLAGS += -march=armv7-a -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard
#CFLAGS += -march=armv7-a -std=gnu11   -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mno-unaligned-access -g -Os -Wall -ffunction-sections -fdata-sections

# vfp
ASFLAGS += -march=armv7-a -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4
CFLAGS += -march=armv7-a -std=gnu11   -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4 -mno-unaligned-access -g -Os -Wall -ffunction-sections -fdata-sections
LDFLAGS		+= ${FULL_LIBRARY_PATH} --start-group ${MYLIBFILES} ${FULL_LIBRARY_EXT} --end-group

COMMON_LIB_PACKER_FLAGS := cru
GCC_COMPILER_FLAGS += -DCPL_GCC
# ------------------------------------------------------------------------
# Export to recursive calls in dependencies
# ------------------------------------------------------------------------
export CT_RELEASE
export CT_TARGET
export CFLAGS
export ASFLAGS
export LDFLAGS
export COMMON_LIB_PACKER_FLAGS
export MYLIBFILES



