#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct stat st = {0};

void print_err(){
	printf("Error: %s\n", strerror(errno));
	exit(-2);
}

int db_init(char *fn, char **argv){
	char *dir = argv[0];
	if (stat(dir, &st) == -1) {
		printf("Initializing a new database structure @ %s\n", dir);
		if(mkdir(dir, 0755)) print_err();
	}else{
		printf("Directory exists?\n");
	}
	
	if(chdir(dir)) print_err();
	if(mkdir("by-tikr", 0755)) print_err();
	if(mkdir("by-date", 0755)) print_err();
	if(mkdir("unsorted", 0755)) print_err();
}
