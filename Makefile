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
	@echo Compiling target $@
	@$(CC) $^ $(mk_out) $@ $(inc) $(opt) $(flags) $(ldflags)

VPATH = $(src)
$(obj)/%.o: %.c
	@echo Compiling $< to $@
	@$(CC) $(mk_obj) $< $(mk_out) $@ $(inc) $(opt) $(flags)

gch = $(pch:.h=.h.gch)
$(gch): $(pch)
	@echo Compiling precompiled header $@
	@$(CC) $(mk_obj) $<

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
	@echo Cleaning ...
	@rm -f $(obj)/* $(target)

.PHONY: verbose
verbose:
	$(eval flags+=-H)

.PHONY: vars
vars:
	@echo src:     $(src)
	@echo obj:     $(obj)
	@echo sources: $(source)
	@echo objects: $(object)
	@echo include: $(include)
	@echo targets: $(target)
	@echo flags:   $(flags)
	@echo opt:     $(opt)
	@echo config:  $(config)

.PHONY: run
run: build
	@$(target)

.PHONY: dump
dump: build
	objdump -M intel -d $(obj)/main.o

.PHONY: valgrind
valgrind: build
	valgrind --leak-check=full $(target)