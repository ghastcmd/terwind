# master folders
src = src
obj = bin
# include folders
include = src/include src/pch src

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

# option flags
opt:=

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
object = $(patsubst %,$(obj)/%, $(notdir $(source:.c=.o)))

VPATH = $(src)
$(obj)/%.o: %.c
	$(CC) $(mk_obj) $< $(mk_out) $@ $(inc) $(opt) $(flags) $(ldflags)

$(target): $(object)
	$(CC) $^ $(mk_out) $@ $(inc) $(opt) $(flags) $(ldflags)

$(pch:.h=.h.gch): $(pch)
	$(CC) $(mk_obj) $<

.PHONY: build
build: $(pch:.h=.h.gch) $(obj) $(target)

bin:
	mkdir bin

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
	rm -f $(target).exe
	rm -f $(target)

.PHONY: cleans
cleans:
	rm -f $(obj)/*

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