

##############################################
# Source files

LIC_HEADERS := \
	src/license/as_license.h

LIC_DEP := \
	src/license/as_license.h

LIC_SRC := 

LIC_INCS := \
	-Isrc/license/ 
	
LIC_DEFS := 


################################################
# Add to global build target

HEADERS += $(LIC_HEADERS)
DEFS += $(LIC_DEFS)
DEP += $(LIC_DEP)
SRC += $(LIC_SRC)
INCS += $(LIC_INCS)


##################################################
# RECIPES

license_tools: $(LIBNOM)_$(ARCH).a

license_tests: $(LIBNOM)_$(ARCH).a 
	@mkdir -p $(BUILD_DIR)/test/

license_runtests: cti_tests
	@echo '-- LICENSE TESTS -----------------------------'

