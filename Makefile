all: marketdb

CFLAGS=-I includes -std=gnu99

marketdb: src/main.c src/init.c src/import.c
	@echo Building marketdb
	@gcc $(CFLAGS) -o $@ $^

.PHONY: clean
	@echo Removing files...
	@rm -rf marketdb
