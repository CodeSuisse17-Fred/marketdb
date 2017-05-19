#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <zlib.h>
#include <assert.h>

#define CHUNK 16384

int gzip_file(char *);

int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

void zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror(stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}

int db_import(char *fn, char **argv){
	printf("Starting import from file...\n");
	char *db = argv[0];
	char *inf = argv[1];
	printf("Opening file %s\n", inf);
	FILE *fp = fopen(inf, "r");

	struct stat st = {0};
	char *line = malloc(64);

	while(!feof(fp)){
		char *tickr;
		char *date;
		int ms;
		float price;

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

		strcat(path, ".gz");
//		FILE *fp3 = fopen(path, "w");

		/*
		int ret = def(fp, fp3, Z_DEFAULT_COMPRESSION);
		if(ret != Z_OK)
			zerr(ret);
		*/

		fclose(fp2);
//		fclose(fp3);
		line = lp;

		// Below code makes hard links
/*
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
*/

	}
	
	free(line);
	fclose(fp);
}
