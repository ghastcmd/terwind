# master folders
src_d = src
obj = bin
# include folders
include = src/include src/pch src
src = $(filter-out $(include), $(patsubst %/.,%,$(wildcard $(src_d)/*/.))) $(src_d)

# cli compiler
CC = gcc
# compiler flags
mk_obj = -c
mk_inc = -I
mk_out = -o

SS = @

# pre-compiled header
pch = pch/pch.h

# target
target = $(obj)/bin
ifeq ($(OS),Windows_NT)
	target:=$(target).exe
endif

# option flags
opt =
thread_count = 12

# additional flags
flags = -Wall -Wextra -Werror

# library links flags
ldflags = -lm 
# windows specific libraries
ifeq ($(OS), Windows_NT)
	ldflags+=
else
	ldflags+=-lpthread
endif

# full blown include paths
inc = $(addprefix $(mk_inc),$(include))

source = $(foreach var,$(src),$(wildcard $(var)/*.c))
object = $(patsubst %,$(obj)/%.o, $(basename $(notdir $(source))))

$(target): $(object)
	$(SS)echo Compiling target $@
	$(SS)$(CC) $^ $(mk_out) $@ $(inc) $(opt) $(flags) $(ldflags)

VPATH = $(src)
$(obj)/%.o: %.c
	$(SS)echo Compiling $< to $@
	$(SS)$(CC) $(mk_obj) $< $(mk_out) $@ $(inc) $(opt) $(flags)

gch = $(pch:.h=.h.gch)
$(gch): $(pch)
	$(SS)echo Compiling precompiled header $@
	$(SS)$(CC) $(mk_obj) $(ldflags) $<

help:
	@echo $(gch)

obj_folders :=
mklib: $(gch) $(obj) mklib_o

mklib_o: $(addprefix $(obj)/,$(addsuffix .o,$(obj_folders)))
	$(SS)echo Compiling static library
	$(SS)ld -r -o lib.a $^

.PHONY: build compile
build:
	@$(MAKE) -s compile -j $(thread_count)

compile: $(gch) $(obj) $(target)

$(obj):
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


comma := ,
space := $(null) #

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