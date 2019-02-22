
TARGET := android
TFS_LIBC_D := $(BUILD_BASE)/tfs_libc/build/internal/$(TARGET)/

ID := $(LOCAL_PATH)/

MSRC :=
MINCS :=
MDEP :=
MDEFS := -D_FILE_OFFSET_BITS=64 -DASCTI_SYSTEM_TYPE=2 -DNDEBUG
MHEADERS :=

MCFLAGS += -Wall -Werror -fPIC -O3 -fno-exceptions \
	-ffast-math -fno-unwind-tables -fvisibility=hidden \
	-fno-asynchronous-unwind-tables


###############################################
# TFS Libc

TFSLIBC ?= libc
MDEFS += -DPLATFORM_H=\"platform.$(TFSLIBC).h\"
MINCS += -I$(TFS_LIBC_D)/include/


###############################################
# CTI (cti.mk)

MHEADERS += \
        src/cti/as_cti.h \
        src/cti_GENERATED/ascti_tests.h

MDEP += \
        src/cti/as_cti.h \
        src/cti_GENERATED/ascti_tests.h \
        src/cti_GENERATED/cti_protobuf/addsec_cti.pb.h \
        src/cti_GENERATED/protobuf/pb.h

MSRC += \
        src/cti/as_cti.c

MINCS += \
        -I$(ID)src/cti/ \
        -I$(ID)src/cti_GENERATED/ \
        -I$(ID)src/cti_GENERATED/protobuf/ \
        -I$(ID)src/cti_GENERATED/cti_protobuf/


##############################################
# License (license.mk)

MHEADERS += \
        src/license/as_license.h

MDEP += \
        src/license/as_license.h

MSRC +=

MINCS += \
        -I$(ID)src/license/
