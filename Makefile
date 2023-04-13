
MKFILE_PATH=$(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR=$(dir $(MKFILE_PATH))
export ROOT_PROJECT_DIRECTORY=$(MKFILE_DIR)

TOPTARGETS=all cppcheck
SUBDIRS=src
.PHONY: $(TOPTARGETS)

-include $(ROOT_PROJECT_DIRECTORY)options.mk

all: $(BUILD_DIRECTORY) $(BIN_DIRECTORY) $(LIB_DIRECTORY) $(OBJ_DIRECTORY) $(GEN_DIRECTORY)
	@:

$(BUILD_DIRECTORY) $(BIN_DIRECTORY) $(LIB_DIRECTORY) $(OBJ_DIRECTORY) $(GEN_DIRECTORY):
	$(AT)mkdir -p $@

.PHONY: cache_makefile
cache_makefile:
	$(MAKE) --no-print-directory -C $(MKFILE_DIR) dump_paths

clean:
	$(RM) -r $(BUILD_DIRECTORY)

$(TOPTARGETS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@printf "$(COLOR_RED)$(COLOR_BOLD)MAKE -C $@ $(MAKECMDGOALS)$(COLOR_RESET)\n"
	@$(MAKE) --no-print-directory -C $@ $(MAKECMDGOALS)

.PHONY: dump_paths
dump_paths:
	$(info CC=$(CC_1))
	$(info CXX=$(CXX_1))
	$(info LD=$(LD_1))
	$(info AR=$(AR_1))
	$(info RANLIB=$(RANLIB_1))
	$(info LEX=$(LEX))
	$(info YACC=$(YACC))
	$(info PYTHON3=$(PYTHON3_1))
	$(info )
	$(info BUILD=$(BUILD))
	$(info BUILD_DIRECTORY=$(BUILD_DIRECTORY))
	$(info BIN_DIRECTORY=$(BIN_DIRECTORY))
	$(info OBJ_DIRECTORY=$(OBJ_DIRECTORY))
	$(info LIB_DIRECTORY=$(LIB_DIRECTORY))
	$(info GEN_DIRECTORY=$(GEN_DIRECTORY))
	$(info )
	$(info VERBOSE=$(VERBOSE))
	$(info DEBUG=$(DEBUG))
	$(info LLVM_INSTALL=$(LLVM_INSTALL))
	$(info LLVM=$(LLVM))
	$(info )
	$(info CFLAGS=$(CFLAGS))
	$(info CXXFLAGS=$(CXXFLAGS))
	$(info ASFLAGS=$(ASFLAGS))
	$(info LDFLAGS=$(LDFLAGS))
	$(info INCLUDE=$(INCLUDE))
	$(info YFLAGS=$(YFLAGS))
	$(info LFLAGS=$(LFLAGS))

	@:

