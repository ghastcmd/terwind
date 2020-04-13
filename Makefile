src = src
obj = bin
inc = $(src)/headers

flags = -Wall -Wextra -Werror

ifneq ($(OS), Windows_NT)
	flags+=-lpthread
endif

pch = $(inc)/pch.h
target = $(obj)/bin

opt=

source = $(wildcard $(src)/*.c)
object = $(patsubst %,$(obj)/%, $(notdir $(source:.c=.o)))

$(obj)/%.o: $(src)/%.c
	gcc -c $^ -o $@ -I $(inc) $(opt) $(flags)

$(target): $(object)
	gcc $^ -o $@ -I $(inc) $(opt) $(flags)

$(pch:.h=.h.gch): $(pch)
	gcc -c $<

build: $(pch:.h=.h.gch) $(obj) $(target)

bin:
	mkdir bin

optimize:
	$(eval opt+=-O3)

release: optimize
	$(eval opt+=-DRELEASE)

logg:
	$(eval opt+=-DLOGG)

ter_debug:
	$(eval opt+=-DTER_DEBUG)

clean:
	rm -f $(obj)/bin.exe
	rm -f $(obj)/bin

cleans:
	rm -f $(obj)/*

verbose:
	$(eval flags+=-H)

tst:
	@echo "sources:" $(source)
	@echo "objects:" $(object)
	@echo "include:" $(inc)
	@echo "targets:" $(target)
	@echo "flags:  " $(flags)
	@echo "opt:    " $(opt)

run: build
	@$(obj)/bin

dump: build
	objdump -M intel -d $(obj)/main.o

valgrind: build
	valgrind --leak-check=full ./bin/bin

help:
	@echo run dump tst verbose
	@echo clean cleans logg
	@echo release optimize bin build