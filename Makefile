src = src
obj = bin
inc = $(src)/headers

target = $(obj)/bin

opt = -O3

source = $(wildcard $(src)/*.c)
object = $(patsubst %,$(obj)/%, $(notdir $(source:.c=.o)))

$(obj)/%.o: $(src)/%.c
	gcc -c $^ -o $@ -I $(inc) $(opt)

$(target): $(object)
	gcc $^ -o $@ -I $(inc) $(opt)

build: $(target)

raw:
	$(eval opt=)

clean:
	rm -f $(obj)/bin
	rm -f $(obj)/bin.exe

cleans:
	rm -f $(obj)/*

init:
	mkdir bin

tst:
	@echo $(source)
	@echo $(object)
	@echo $(inc)
	@echo $(target)

run: build
	@$(obj)/bin

dump: build
	objdump -M intel -d $(obj)/main.o