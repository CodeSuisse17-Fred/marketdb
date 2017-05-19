all: marketdb

CFLAGS=-I includes -std=gnu99

marketdb: src/main.c src/init.c src/import.c src/gzip.c src/parser.tab.c src/lex.yy.c src/calls.c
	@echo Building marketdb
	@gcc $(CFLAGS) -lfl -o $@ $^ -lz

src/parser.tab.c: src/parser.y
	@echo Generating parser
	@bison -d src/parser.y -o src/parser.tab.c --defines=includes/parser.tab.h

src/lex.yy.c: src/lexer.l
	@echo Generating lexer
	@flex -l -o src/lex.yy.c src/lexer.l

parsetest: src/parser.tab.c src/lex.yy.c
	@gcc -o $@ $^ -lfl $(CFLAGS)

.PHONY: clean
clean:
	@echo Removing files...
	@rm -rf marketdb
	@rm -rf src/parser.tab.c includes/parser.tab.h src/parser.output src/lex.yy.c parsetest
