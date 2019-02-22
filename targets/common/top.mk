
TFS_LIBC_D := $(BUILD_BASE)/tfs_libc/build/internal/$(TARGET)/
TFSLIBC ?= libc

##############################################
# Source files & definitions

# starting/defaults
HEADERS :=
DEFS += -DPLATFORM_H=\"platform.$(TFSLIBC).h\"
DEP :=
SRC :=
INCS := -I$(TFS_LIBC_D)/include/
EXELIBS := 

CFLAGS += -Wall -Werror -fPIC -O3 -fno-exceptions \
	-ffast-math -fno-unwind-tables -fvisibility=hidden \
	-fno-asynchronous-unwind-tables

OBJ_DIR = $(BUILD_DIR)/obj_$(ARCH)/


# module additions
include ../common/cti.mk
include ../common/tlv.mk
include ../common/license.mk


CFLAGS += $(DEFS) $(INCS)
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

################################################
# Compiler/cross-compile support

CROSSCOMPILE ?=
CC ?= $(CROSSCOMPILE)gcc
AR ?= $(CROSSCOMPILE)ar
STRIP ?= $(CROSSCOMPILE)strip

SHAREDLIBEXT ?= .so


##################################################
# RECIPES

.PHONY: build clean 

build: $(LIBNOM)_$(ARCH).a
	@mkdir -p $(BUILD_DIR)/include/
	@cp $(HEADERS) $(BUILD_DIR)/include/
	@echo '' >> $(BUILD_DIR)/build.environment
	@echo $(LIBNOM)_$(ARCH) >> $(BUILD_DIR)/build.environment
	@$(CC) $(CFLAGS) --version >> $(BUILD_DIR)/build.environment
	@echo $(CFLAGS) >> $(BUILD_DIR)/build.environment

$(OBJ): $(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo - CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(LIBNOM)_$(ARCH).a: $(OBJ)
	@mkdir -p $(BUILD_DIR)/lib/
	@$(AR) rcs $(BUILD_DIR)/lib/$@ $(OBJ)
	@$(LD) -r $(OBJ) -o $(BUILD_DIR)/lib/$@.o

$(LIBNOM)_$(ARCH).$(SHAREDLIBEXT): $(OBJ)
	@mkdir -p $(BUILD_DIR)/lib/
	@$(CC) -shared -o $(BUILD_DIR)/lib/$@ $(CFLAGS) $(OBJ)
	@$(STRIP) $(BUILD_DIR)/lib/$@
    
clean:
	rm -rf $(BUILD_DIR)/


