
-include $(ROOT_PROJECT_DIRECTORY)color.mk

BUILD=build
BUILD_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)$(BUILD)/

BIN_DIRECTORY=$(BUILD_DIRECTORY)bin/
OBJ_DIRECTORY=$(BUILD_DIRECTORY)obj/
LIB_DIRECTORY=$(BUILD_DIRECTORY)lib/
GEN_DIRECTORY=$(BUILD_DIRECTORY)gen/

REL_PATH=$(shell realpath --relative-to $(ROOT_PROJECT_DIRECTORY) .)/
SRC_PATH=$(ROOT_PROJECT_DIRECTORY)$(REL_PATH)
OBJ_PATH=$(OBJ_DIRECTORY)$(REL_PATH)

# erase automatic vars
.SUFFIXES:

OS=$(shell uname)
ifeq ($(OS),Linux)
CC=/usr/bin/clang
CXX:=/usr/bin/clang++
LD:=$(CXX)
AR=/usr/bin/ar
RANLIB=/usr/bin/ranlib
YACC=/usr/bin/bison
LEX=/usr/bin/flex
PYTHON3=/usr/bin/python3
else ifeq ($(OS),Darwin)
CC=/usr/local/opt/llvm/bin/clang
CXX:=$(CC)
LD:=$(CC)
AR=/usr/local/opt/llvm/bin/llvm-ar
RANLIB=/usr/local/opt/llvm/bin/llvm-ranlib
YACC=/usr/local/opt/bison/bin/bison
LEX=/usr/local/opt/flex/bin/flex
PYTHON3=/usr/bin/python3
else
$(error Unsupported build on $(OS))
endif


VERBOSE=0
ifeq ($(VERBOSE),0)
AT=@
else
AT=
endif

define strip_root
$$(shell sed s%$(ROOT_PROJECT_DIRECTORY)%% <<< $1)
endef
define _generate_verbose_call
override $1_0=@printf "$(COLOR_GREEN)$(COLOR_BOLD)%s %s$(COLOR_RESET)\n" $1 $(call strip_root,$$@); $($1)
override $1_1=$($1)
override $1=$$($1_$(VERBOSE))
endef
define generate_verbose_call
$(eval $(call _generate_verbose_call,$1))
endef

map = $(foreach a,$(2),$(call $(1),$(a)))
$(call map,generate_verbose_call,CC CXX LD AR RANLIB YACC LEX PYTHON3)

COMPILE_FLAGS_=
LINK_FLAGS_=
COMPILE_FLAGS_+= -Wno-comment
COMPILE_FLAGS_+= -flto
LINK_FLAGS_+= -flto

ifeq ($(DEBUG),1)
COMPILE_FLAGS_+= -DDEBUG=1 -g -O0 -fstandalone-debug
LINK_FLAGS_+= -g
else
# #warnings are for developers
COMPILE_FLAGS_+= -O3 -Wno-\#warnings
endif

COMPILE_FLAGS_+= -masm=intel
COMPILE_FLAGS_+= -Wall -Wextra

# add runtime lib path
COMPILE_FLAGS_+= -DRUNTIME_LINKER_PATH=\"$(CC_1)\"
COMPILE_FLAGS_+= -DRUNTIME_LINKER_ARGS=\"\"
COMPILE_FLAGS_+= -DRUNTIME_LIBRARY_DIR=\"$(ROOT_PROJECT_DIRECTORY)runtime/\"
COMPILE_FLAGS_+= -DRUNTIME_LIBRARY_NAME=\"runtime\"

ifdef LINKER
LINK_FLAGS_+= -fuse-ld=$(LINKER)
endif

LLVM=1
LLVM_COMPILE_FLAGS=
LLVM_LINK_FLAGS=
LLVM_INSTALL=$(ROOT_PROJECT_DIRECTORY)third-party/llvm
ifeq ($(LLVM),1)
LLVM_CONFIG=$(LLVM_INSTALL)/bin/llvm-config
LLVM_VERSION=$(shell $(LLVM_CONFIG) --version | sed -E 's/^([0-9]+)\..+$$/\1/')
ifeq ($(shell test $(LLVM_VERSION) -lt 16; echo $$?),0)
$(error "Insufficent LLVM version, requires 16+")
endif
# dont use c++ flags, use c preprocessor flags to get includes and macros
# use -isystem for the include to silence warnings from llvm
LLVM_COMPILE_FLAGS= -isystem $(shell $(LLVM_CONFIG) --includedir) $(shell $(LLVM_CONFIG) --cppflags)
LLVM_LINK_FLAGS=-Wl,-rpath=$(shell $(LLVM_CONFIG) --libdir) $(shell $(LLVM_CONFIG) --ldflags --system-libs --libs core)
endif
COMPILE_FLAGS_+= $(LLVM_COMPILE_FLAGS)
LINK_FLAGS_+= $(LLVM_LINK_FLAGS)


# from user
COMPILE_FLAGS_+= $(COMPILE_FLAGS)
LINK_FLAGS_+= $(LINK_FLAGS)

# from user
BISON_FLAGS_=
BISON_FLAGS_+= -Wall
BISON_FLAGS_+= $(BISON_FLAGS)
ifeq ($(DEBUG),1)
BISON_FLAGS_+= -Dparse.trace
ifeq ($(DEBUG_PARSER),1)
COMPILE_FLAGS_+= -DDEBUG_PARSER=1
endif
endif


override CFLAGS+= $(COMPILE_FLAGS_) -std=c17 -D_XOPEN_SOURCE=700
override CXXFLAGS+= $(COMPILE_FLAGS_) -std=c++17
override ASFLAGS+=
override LDFLAGS+= $(LINK_FLAGS_)
override INCLUDE+=
override YFLAGS+= $(BISON_FLAGS_)
override LFLAGS+=

