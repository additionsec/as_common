

##############################################
# Source files

CTI_HEADERS := \
	src/cti/as_cti.h \
	src/cti_GENERATED/ascti_tests.h

CTI_DEP := \
	src/cti/as_cti.h \
	src/cti_GENERATED/ascti_tests.h \
	src/cti_GENERATED/cti_protobuf/addsec_cti.pb.h \
	src/cti_GENERATED/protobuf/pb.h

CTI_SRC := \
	src/cti/as_cti.c

CTI_INCS := \
	-Isrc/cti/ \
	-Isrc/cti_GENERATED/ \
	-Isrc/cti_GENERATED/protobuf/ \
	-Isrc/cti_GENERATED/cti_protobuf/
	
CTI_DEFS := 


################################################
# Add to global build target

HEADERS += $(CTI_HEADERS)
DEFS += $(CTI_DEFS)
DEP += $(CTI_DEP)
SRC += $(CTI_SRC)
INCS += $(CTI_INCS)


##################################################
# RECIPES

cti_tools: $(LIBNOM)_$(ARCH).a

cti_tests: $(LIBNOM)_$(ARCH).a 
	@mkdir -p $(BUILD_DIR)/test/
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/test/test_basic -I$(BUILD_DIR)/include/ \
		test/cti/test_basic.c $(BUILD_DIR)/lib/$(LIBNOM)_$(ARCH).a $(EXELIBS)

cti_runtests: cti_tests
	@echo '-- CTI TESTS -----------------------------'

