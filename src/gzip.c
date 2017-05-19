#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int db_gzip(char *fn, char **argv){
	printf("Gzipping database...");
	char *s = calloc(1, 30);
	strcat(s, argv[0]);
	strcat(s, "/");
	strcat(s, "unsorted");
	
	char *str = malloc(60);
	sprintf(str, "bash -c \"cd %s && gzip *\"", s);
	system(str);
	free(str);
	free(s);
}
