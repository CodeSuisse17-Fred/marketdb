#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <calls.h>
extern int yyerror(char *, ...);

FILE *getprices(char *dbname, char *tickr, struct s_timedate *s, struct s_timedate *e, float *h, float *l){
	
	// Check date range
	/* This function needs lots of trivial little routines that I'm not gonna do right now */
	
	FILE *fp = fopen("/tmp/db", "w");

	char st[32];
	char et[32];
	char sd[32];
	char ed[32];
	// sprintf!!! use snprintf!!
	sprintf(st, "%2d%2d%2d", s->t->h, s->t->m, s->t->s);
	sprintf(et, "%2d%2d%2d", e->t->h, e->t->m, e->t->s);
	sprintf(sd, "%2d%2d%2d", s->d->m, s->d->d, s->d->y);
	sprintf(ed, "%2d%2d%2d", e->d->m, e->d->d, e->d->y);

	FILE *fp2;
	char path[64];
	// Loop through time range
	//for(;;){}
	/* TEST */
	strcat(path, tickr);
	strcat(path, "_");
	strcat(path, sd);
	strcat(path, "_");
	strcat(path, st);
	fp2 = fopen(path, "r");

	// Inefficient
	int ms;
	float price;
	while(fscanf(fp2, "%d %f\n", &ms, &price)){
		fprintf(fp, "%s %s %s %d %f\n", tickr, sd, st, ms, price);
	}
	fclose(fp2);

	memset(path, 0, 64);
	strcat(path, tickr);
	strcat(path, "_");
	strcat(path, ed);
	strcat(path, "_");
	strcat(path, et);
	fp = fopen(path, "r");
	
	// Inefficient
	while(fscanf(fp2, "%d %f\n", &ms, &price)){
		fprintf(fp, "%s %s %s %d %f\n", tickr, sd, st, ms, price);
	}
	fclose(fp2);

	return fp;
}

void print_prices(FILE *rs){

}

char *getgraph(FILE *rs){

}

void print_pred(FILE *rs){

}

int print(struct query *sq){
	for(;sq;sq=sq->next){
		printf("Print records with %d and\nConstraints:\n", sq->f_type);
		for(struct s_constr *cc = sq->c; cc; cc=cc->next){
			printf("%s :: %s\n", cc->f, cc->v);
		}
	}
}

int process(struct query *sq){
	printf("In process()...\n");
	print(sq);

	char *c_name;
	struct s_timedate *s, *e;
	for(struct s_constr *cc = sq->c; cc; cc=cc->next){
		if(!strcmp(cc->f, "DATE")){
			if(!cc->isrange){
				yyerror("Date range required!");
				return -1;
			}
			s = cc->r->start;
			e = cc->r->end;
		}else if(!strcmp(cc->f, "NAME")){
			c_name = cc->v;
		}
	}

	FILE *srs;
	int geth = 0;
	int getl = 0;
	float high = 0;
	float low = 0;
	if((sq->f_type & F_LOW) && (sq->f_type & F_HIGH)){
		srs = getprices("db", c_name, s, e, &high, &low);
		geth = getl = 1;
	}else if(sq->f_type & F_LOW){
		srs = getprices("db", c_name, s, e, 0, &low);
		getl = 1;
	}else if(sq->f_type & F_HIGH){
		srs = getprices("db", c_name, s, e, &high, 0);
		geth = 1;
	}else{
		srs = getprices("db", c_name, s, e, 0, 0);
	}

	if(sq->f_type & F_PRICE){
		print_prices(srs);
	}

	if(geth && getl){
		printf("{ \"HIGH\": %f; \"LOW\": %f; }\n", high, low);
	}else if(geth){
		printf("{ \"HIGH\": %f; }\n", high);
	}else if(getl){
		printf("{ \"LOW\": %f; }\n", low);
	}

	if(sq->f_type & F_GRPH){
		printf("{ GRAPH:%s; }\n", getgraph(srs));
	}

	if(sq->f_type & F_PRED){
		print_pred(srs);
	}
}
