

##############################################
# Source files

TLV_HEADERS := \
	src/tlv/as_tlv.h

TLV_DEP := \
	src/tlv/as_tlv.h

TLV_SRC := \
	src/tlv/tlv.c

TLV_INCS := \
	-Isrc/tlv/
	
TLV_DEFS := 


################################################
# Add to global build target

HEADERS += $(TLV_HEADERS)
DEFS += $(TLV_DEFS)
DEP += $(TLV_DEP)
SRC += $(TLV_SRC)
INCS += $(TLV_INCS)


##################################################
# RECIPES

