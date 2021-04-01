# master folders
src_d = src
obj = bin
# include folders
include = src/include src/pch
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

# additional flags
flags = -Wall -Wextra -Werror

# library links flags
ldflags = 
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
	$(SS)$(CC) $(mk_obj) $<

obj_folders :=
mklib: $(gch) $(obj) $(addprefix $(obj)/,$(addsuffix .o,$(obj_folders)))
	$(SS)echo Compiling static library
	$(SS)$(CC) -shared $(filter-out $(gch) $(obj),$^) \
	$(mk_out) ./lib.so $(inc) $(opt) $(flags) $(ldflags)

.PHONY: build
build: $(gch) $(obj) $(target)

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

.PHONY: clean
clean:
	$(SS)echo Cleaning ...
	$(SS)rm -f $(obj)/* $(target)

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

.PHONY: run
run: build
	$(SS)$(target)

.PHONY: dump
dump: build
	objdump -M intel -d $(obj)/main.o

.PHONY: valgrind
valgrind: build
	valgrind --leak-check=full $(target)