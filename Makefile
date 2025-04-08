# symbols for use in actions
comma := ,
space := $(null) #
wild  := *

# master folders
src_d = src
obj = bin
# include folders
pch_include = src/pch
include = src/include $(pch_include) src
libs_d = $(foreach var,$(wildcard $(src_d)/$(wild)_lib/.),$(var:/.=))
src = $(filter-out $(include) $(libs_d), $(patsubst %/.,%,$(wildcard $(src_d)/*/.))) $(src_d)

# cli compiler
CC = gcc
# compiler flags
mk_obj = -c
mk_inc = -I
mk_out = -o

SS = @

# pre-compiled header
pch = src/pch/pch.h

# creating version path
ifeq ($(OS),Windows_NT)
version := windows
else
version := linux
endif

# target
target := $(obj)/$(version)/bin
code_lib_target = 
ifeq ($(OS),Windows_NT)
	target := $(target).exe
	code_lib_target := $(obj)/$(version)/libcode_lib.dll
else
	code_lib_target := $(obj)/$(version)/code_lib.so
endif

# option flags
opt =

# additional flags
flags = -Wall -Wextra -Werror

# library links flags
ldflags = -lm 
# windows specific libraries
ifeq ($(OS),Windows_NT)
	ldflags += 
else
	ldflags += -lpthread
endif

VPATH = $(src) $(libs_d)

# full blown include paths
inc = $(addprefix $(mk_inc),$(include))
inc_pch = $(addprefix $(mk_inc),$(pch_include))

source = $(foreach var,$(src),$(wildcard $(var)/*.c))
object = $(patsubst %,$(obj)/$(version)/%.o, $(basename $(notdir $(source))))

code_lib_source = $(foreach var,$(libs_d),$(wildcard $(var)/$(wild).c))
code_lib_object = $(patsubst %,$(obj)/$(version)/%.o, $(basename $(notdir $(code_lib_source))))

$(target): $(code_lib_target) $(object)
	$(SS)echo Compiling target $@
	$(SS)$(CC) $^ $(mk_out) $@ $(inc) $(opt) $(flags) $(ldflags)

$(obj)/$(version)/%.o: %.c
	$(SS)echo Compiling $< to $@
	$(SS)$(CC) $(mk_obj) $< $(mk_out) $@ $(inc) $(opt) $(flags)

gch = $(pch:.h=.h.gch)
$(gch): $(pch)
	$(SS)echo Compiling precompiled header $@
	$(SS)$(CC) $(mk_obj) $(ldflags) $<

# making dynamic libraries
$(obj)/$(version)/lib/%.o: %c
	$(SS)echo Compiling lib item $< to $@
	$(SS)$(CC) $(mk_obj) $< $(mk_out) $@ $(inc_pch)

$(code_lib_target): $(code_lib_object)
	$(SS)echo Compiling dynamic library target $@
	$(SS)$(CC) -shared $^ $(mk_out) $@ $(inc_pch)

help:
	@echo $(gch)

# makind directories
obj_folders :=
mklib: $(gch) $(obj) mklib_o

mklib_o: $(addprefix $(obj)/,$(addsuffix .o,$(obj_folders)))
	$(SS)echo Compiling static library
	$(SS)ld -r -o lib.a $^

.PHONY: build compile
build:
	@$(MAKE) -s compile -j

ifeq ($(OS),Windows_NT)
path_v = $(obj)/$(version)/.stamp
path_v_w = $(obj)/$(version)/lib/.stamp

$(obj)/$(version)/.stamp: $(obj)
	-mkdir $(obj)\$(version)
	touch $(obj)/$(version)/.stamp

$(obj)/$(version)/lib/.stamp: $(obj)/$(version)/.stamp
	-mkdir $(obj)\$(version)\lib
	touch $(obj)/$(version)/lib/.stamp
else
#linux
path_v := $(obj)/$(version)
path_v_w = $(obj)/$(version)/lib

$(obj)/$(version): $(obj)
	mkdir $(obj)/$(version)

$(obj)/$(version)/lib: $(obj)/$(version)
	mkdir $(obj)/$(version)/lib
endif

compile: $(gch) $(path_v) $(path_v_w) $(target)


$(obj):
	echo another
	mkdir $(obj)

config:=
ifndef config
	config=debug
endif

# defining the configurations
# runs the application without graphics
ifeq ($(config),nographics)
	opt=-DTER_DEBUG -DLOGG
# only enables logg
else ifeq ($(config),debug)
	opt=-DLOGG
# doesn't logg and enables release define
else ifeq ($(config),release)
	opt=-DRELEASE
# release define and optimization
else ifeq ($(config),dist)
	opt=-DRELEASE -O3
else
	$(error invalid configuration)
endif

.PHONY: clean
clean:
	$(SS)echo Cleaning ...
ifeq ($(OS),Windows_NT)
ifneq ($(wildcard $(obj)/*),$(null))
	-$(SS)powershell -c 'rm -Force -Recurse $(subst $(space),$(comma),$(strip $(wildcard $(obj)/*))) -ErrorAction SilentlyContinue'
endif
	-$(SS)powershell -c 'rm -Force -Recurse $(target) -ErrorAction SilentlyContinue'
	-$(SS)powershell -c 'rm -Force -Recurse lib.a -ErrorAction SilentlyContinue'
	-$(SS)powershell -c 'rm -Force -Recurse $(src_d)/$(gch) -ErrorAction SilentlyContinue'
ifneq ($(wildcard log/*.dat),$(null))	
	-$(SS)powershell -c 'rm -Force -Recurse $(subst $(space),$(comma),$(strip $(wildcard log/*.dat))) -ErrorAction SilentlyContinue'
endif
else
	$(SS)rm -rf $(wildcard $(obj)/*)
	$(SS)rm -rf $(target)
	$(SS)rm -rf lib.a
	$(SS)rm -rf $(src_d)/$(gch)
	$(SS)rm -rf $(wildcard log/*.dat)
endif

.PHONY: verbose
verbose:
	$(eval flags+=-H)

.PHONY: vars
vars:
	$(SS)echo src:     $(src)
	$(SS)echo obj:     $(obj)
	$(SS)echo libs_d:  $(libs_d)
	$(SS)echo sources: $(source)
	$(SS)echo objects: $(object)
	$(SS)echo include: $(include)
	$(SS)echo targets: $(target)
	$(SS)echo flags:   $(flags)
	$(SS)echo opt:     $(opt)
	$(SS)echo config:  $(config)
	$(SS)echo pch:     $(pch)
	$(SS)echo gch:     $(gch)

.PHONY: run
run: build
	$(SS)$(target)

to_dump = 

.PHONY: dump
dump: build
ifeq ($(to_dump),$(null))
	objdump -M intel -d $(obj)/main.o
else
	objdump -M intel -d $(obj)/$(to_dump)
endif

.PHONY: valgrind
valgrind: build
	valgrind --leak-check=full $(target)