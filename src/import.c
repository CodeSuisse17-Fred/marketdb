#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int db_import(char *fn, char **argv){
	printf("Starting import from file...\n");
	char *db = argv[0];
	char *inf = argv[1];
	printf("Opening file %s\n", inf);
	FILE *fp = fopen(inf, "r");

	struct stat st = {0};

	while(!feof(fp)){
		char *tickr;
		char *date;
		int ms;
		float price;

		char *line = malloc(64);
		char *lp = line;
		fscanf(fp, "%s\n", line);
		if(line[0] == '#')
			continue;

		// Tokenize parts
		tickr = strsep(&line, ",");
		date = strsep(&line, ".");
		ms = atoi(strsep(&line, ","));
		strsep(&line, ",");
		price = strtof(strsep(&line, ","), NULL);

		char fname1[20], fname2[20];
		sprintf(fname1, "%s_%.8s_%s", tickr, date, date+8);
//		printf("%s\n", fname1);

		char path[40] = {0};
		strcat(path, db);
		strcat(path, "/unsorted/");
		strcat(path, fname1);
		FILE *fp2 = fopen(path, "a");
		fprintf(fp2, "%d %f\n", ms, price);
		fclose(fp2);

		char path2[40] = {0};
		strcat(path2, db);
		strcat(path2, "/by-tikr/");
		strcat(path2, tickr);

	//	printf("%s\n", path2);
		if(stat(path2, &st) || !S_ISDIR(st.st_mode))
			if(mkdir(path2, 0755))
				fprintf(stderr, "Error:%s\n", strerror(errno));

		strcat(path2, "/");
		sprintf(fname2, "%s", date+8);
		strcat(path2, fname2);

		link(path, path2);

		char path3[40] = {0};
		strcat(path3, db);
		strcat(path3, "/by-date/");
		char *t = strdup(date);
		t[8] = 0;
		strcat(path3, t);
		free(t);

	//	printf("%s\n", path3);
		if(stat(path3, &st) || !S_ISDIR(st.st_mode))
			if(mkdir(path3, 0755))
				fprintf(stderr, "Error:%s\n", strerror(errno));
		
		strcat(path3, "/");
		strcat(path3, fname2);
		link(path, path3);

		free(lp);
	}

	fclose(fp);
}
