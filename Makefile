src = src
obj = bin
inc = $(src)/headers

flags = -Wall -Wextra -Werror

pch = $(inc)/pch.h
target = $(obj)/bin

opt = -O0

source = $(wildcard $(src)/*.c)
object = $(patsubst %,$(obj)/%, $(notdir $(source:.c=.o)))

$(obj)/%.o: $(src)/%.c
	gcc -c $^ -o $@ -I $(inc) $(opt) $(flags)

$(target): $(object)
	gcc $^ -o $@ -I $(inc) $(opt) $(flags)

$(pch:.h=.h.gch): $(pch)
	gcc -c $<

build: $(obj) $(target) $(pch:.h=.h.gch)

bin:
	mkdir bin

raw:
	$(eval opt=)

release:
	$(eval opt+= -DRELEASE)

ter_debug:
	$(eval opt+= -DTER_DEBUG)

clean:
	rm -f $(obj)/bin
	rm -f $(obj)/bin.exe

cleans:
	rm -f $(obj)/*

verbose:
	$(eval flags+=-H)

tst:
	@echo $(source)
	@echo $(object)
	@echo $(inc)
	@echo $(target)

run: build
	@$(obj)/bin

dump: build
	objdump -M intel -d $(obj)/main.o