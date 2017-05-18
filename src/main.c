#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commands.h>

char *usage = "Usage:\n%s [COMMAND] <PARAMETERS>\n\
  Commands list:\n\
    init\t[dir]\n\
    import\t[filename]\n";

void print_usage(char *pn){
	printf(usage, pn);
	exit(-1);
}

int main(int argc, char **argv){
	if(argc < 2) print_usage(argv[0]);

	if(!strcmp(argv[1], "init")){
		if(argc < 3) print_usage(argv[0]);
		db_init("init", argv+2);
	}else if(!strcmp(argv[1], "import")){
		db_import("import", argv+2);
	}else{
		printf("Unknown command \'%s\'. Aborting.\n", argv[1]);
		exit(-1);
	}
}
