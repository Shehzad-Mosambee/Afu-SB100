##############################################################################
## General Targets
##############################################################################
export CT_DEVICE := $(DEVICE)

###################################################################################
# Linker script generation
###################################################################################
LD_SRC := ${TOPDIR}/Devices/${CT_DEVICE}/${CT_DEVICE}.ld

NBR :=
PRODUCT :=

# Build the base pathname of the generated lodfile.
#LODBASE_NO_PATH := ${NBR}${CT_TARGET}_${PRODUCT}${CT_RELEASE}
LODBASE_NO_PATH := ${NBR}${PRODUCT}${CT_RELEASE}
LODBASE_NO_PATH := $(notdir ${CT_DEVICE})_${LODBASE_NO_PATH}

# Path for storing all the generated files for one test (elf, dis, lod...).
BINARY_PATH := ${HEX_ROOT}
BAS_FINAL := ${BINARY_PATH}/${LODBASE_NO_PATH}
BIN_FINAL := ${BAS_FINAL}.elf
MAP_FINAL := ${BAS_FINAL}.map
BINF_FINAL :=${BAS_FINAL}.bin
LD_FILE := ${BAS_FINAL}.ld


########################## Directories #######################################

DOS_BINARY_PATH:=$(subst /,\,$(BINARY_PATH))
${BINARY_PATH}:
	@if not exist $(DOS_BINARY_PATH) mkdir $(DOS_BINARY_PATH)

########################## Default Target ###################################
.PHONY: force
force: ;

${BIN_FINAL}: ${BINARY_PATH}
	${CP} ${LD_SRC} ${LD_FILE}
	${LD} -o ${BIN_FINAL} ${OFORMAT} ${FULL_SRC_OBJECTS} \
		${LDFLAG_USED_ELF_FILES}	\
		--script ${LD_FILE} \
		$(LDFLAGS) \
		-Map ${MAP_FINAL}

############################ hex files #######################################
# Generate srec and disassembly files, same logic as bin
# Always build APPBIN
APPBIN: force
	${MAKE} ${BIN_FINAL}
	
	$(OBJCOPY) ${OBJCPY_OPTS} ${BINARY} ${BIN_FINAL} ${BINF_FINAL}
		
	@copy /y .\SysLib\HeaderStub.bin /b + $(subst /,\,$(BINF_FINAL)) /b $(subst /,\,$(BAS_FINAL)).temp /b
	@copy $(subst /,\,$(BAS_FINAL)).temp $(subst /,\,$(BINF_FINAL))
	
	@del $(subst /,\,$(BAS_FINAL)).temp

ifeq ($(DEVICE),SecondApp)
	${SIGN} Sign app ${TOPDIR}/make/make_cmd/keyPair.bin ${BINF_FINAL} $(DEVICE) $(VERSION_HI) $(VERSION_MI) $(VERSION_LO) 3866624 3866624 AmoiTechnology	
else
	${SIGN} Sign app ${TOPDIR}/make/make_cmd/keyPair.bin ${BINF_FINAL} $(DEVICE) $(VERSION_HI) $(VERSION_MI) $(VERSION_LO) 3342336 3342336 AmoiTechnology	
endif
	
	@${ECHO} ${BINF_FINAL} build success!!!
	
	${CP} ${BAS_FINAL}.sign.img ${TOPDIR}/prebuilts/UIS8910DM_release.img
	
ifeq ($(DEVICE),SecondApp)	
	$(PYTHON) $(PACGEN) pac-gen ${TOPDIR}/prebuilts/secApp_flash.json ${BAS_FINAL}.pac
else
	$(PYTHON) $(PACGEN) pac-gen ${TOPDIR}/prebuilts/appimg_flash.json ${BAS_FINAL}.pac
endif
	
#	@$(OBJDUMP) -D ${BIN_FINAL} > ${BIN_FINAL}.txt
	del .\prebuilts\UIS8910DM_release.img
################### "all" target #############################################
# Needs to come after everything else has been defined...
.PHONY: all
all: APPBIN


