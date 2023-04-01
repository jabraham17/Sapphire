
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
PYTHON3=/usr/bin/python3
else ifeq ($(OS),Darwin)
CC=/usr/local/opt/llvm/bin/clang
CXX:=$(CC)
LD:=$(CC)
AR=/usr/local/opt/llvm/bin/llvm-ar
RANLIB=/usr/local/opt/llvm/bin/llvm-ranlib
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
$(call map,generate_verbose_call,CC CXX LD AR RANLIB PYTHON3)

COMPILE_FLAGS=
LINK_FLAGS=
COMPILE_FLAGS+= -Wno-comment
COMPILE_FLAGS+= -flto
LINK_FLAGS+= -flto

ifeq ($(DEBUG),1)
COMPILE_FLAGS+= -DDEBUG=1 -g -O0 -fstandalone-debug
LINK_FLAGS+= -g
else
# #warnings are for developers
COMPILE_FLAGS+= -O3 -Wno-\#warnings
endif

COMPILE_FLAGS+= -masm=intel
COMPILE_FLAGS+= -Wall -Wextra

ifdef LINKER
LINK_FLAGS+= -fuse-ld=$(LINKER)
endif


override CFLAGS+= $(COMPILE_FLAGS) -std=c17 -D_XOPEN_SOURCE=700
override CXXFLAGS+= $(COMPILE_FLAGS) -std=c++17
override ASFLAGS+=
override LDFLAGS+= $(LINK_FLAGS)
override INCLUDE+=


