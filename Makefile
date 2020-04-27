# master folders
src = src
obj = bin
# include folders
include = src/headers

# cli compiler
CC = gcc
# compiler flags
mk_obj = -c
mk_inc = -I
mk_out = -o

# pre-compiled header
pch = $(include)/pch.h

# target
target = $(obj)/bin

# optimize flags
opt=

# additional flags
flags = -Wall -Wextra -Werror

# full blown include paths
inc = $(addprefix $(mk_inc),$(include))

ldflags = 
ifneq ($(OS), Windows_NT)
	ldflags+=-lpthread
endif

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

.PHONY: optimize
optimize:
	$(eval opt+=-O3)

.PHONY: release
release: optimize
	$(eval opt+=-DRELEASE)

.PHONY: logg
logg:
	$(eval opt+=-DLOGG)

.PHONY: ter_debug
ter_debug:
	$(eval opt+=-DTER_DEBUG)

.PHONY: clean
clean:
	rm -f $(obj)/bin.exe
	rm -f $(obj)/bin

.PHONY: cleans
cleans:
	rm -f $(obj)/*

.PHONY: verbose
verbose:
	$(eval flags+=-H)

.PHONY: tst
tst:
	@echo src: $(src)
	@echo obj: $(obj)
	@echo sources: $(source)
	@echo objects: $(object)
	@echo include: $(inc)
	@echo targets: $(target)
	@echo flags:   $(flags)
	@echo opt:     $(opt)

.PHONY: run
run: build
	@$(obj)/bin

.PHONY: dump
dump: build
	objdump -M intel -d $(obj)/main.o

.PHONY: valgrind
valgrind: build
	valgrind --leak-check=full ./bin/bin

.PHONY: help
help:
	@echo run dump tst verbose
	@echo clean cleans logg
	@echo release optimize bin build