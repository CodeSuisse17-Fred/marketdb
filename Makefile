all: marketdb

CFLAGS=-I includes -std=gnu99

marketdb: src/main.c src/init.c src/import.c src/gzip.c
	@echo Building marketdb
	@gcc $(CFLAGS) -o $@ $^

parsetest: src/parser.y src/lexer.l
	@bison -d src/parser.y -o src/parser.tab.c --defines=includes/parser.tab.h
	@flex -l -o src/lex.yy.c src/lexer.l
	@gcc -o $@ src/parser.tab.c src/lex.yy.c -lfl $(CFLAGS)

.PHONY: clean
clean:
	@echo Removing files...
	@rm -rf marketdb
	@rm -rf src/parser.tab.c includes/parser.tab.h src/parser.output src/lex.yy.c parsetest
