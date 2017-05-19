#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <calls.h>
extern int yyerror(char *, ...);

struct resultset {
	int a;	
};

struct resultset *getprices(char *dbname, char *tickr, struct s_timedate *s, struct s_timedate *e, float *h, float *l){

}

void print_prices(struct resultset *rs){

}

char *getgraph(struct resultset *rs){

}

void print_pred(struct resultset *rs){

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

	struct resultset *srs;
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
