#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct stat st = {0};

int db_init(char *fn, char **argv){
	char *dir = argv[0];
	if (stat(dir, &st) == -1) {
		printf("Initializing a new database structure @ %s\n", dir);
		if(mkdir(dir, 0755)){
			printf("Error: %s\n", strerror(errno));
		}
	}else{
		printf("Directory exists?\n");
	}
}
