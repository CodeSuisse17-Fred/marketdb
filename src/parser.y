%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern void yyerror(char *, ...);


struct s_date {
	int m, d, y;
};

struct s_time{
	int h, m, s;
};

struct s_timedate{
	struct s_date *d;
	struct s_time *t;
};

char *get_timedate_str(struct s_timedate *);

struct s_range {
	struct s_timedate *start, *end;
};

struct s_constr {
	struct s_constr *next;
	char *f, *v;
};

%}
	
%union{
	int num;
	char *str;
	struct s_date *date;
	struct s_time *utime;
	struct s_range *range;
	struct s_timedate *td;
	struct s_constr *c;
}

%token SELECT WHERE HIGH LOW GRAPH WORD PREDICTION MOVE NUMBER FROM KAND NAME DATE
%start query_list
%type<num> NUMBER
%type<date> date
%type<utime> time
%type<td> timedate
%type<range> range
%type<str> NAME WORD DATE
%type<c> constr constr_list
%%

query_list: query
		  | query_list query

query: SELECT field_list WHERE constr_list FROM range ';' { printf("A valid query!!!\n"); }
	 | SELECT field_list WHERE constr_list ';' {
	 	printf("A valid query!!!\n");
	 	printf("Constraints:\n");
	 	struct s_constr *p = $4;
	 	while(p){
	 		printf("%s:%s\n", p->f, p->v);
	 		p = p->next;
	 	}
	 }
	 | ';' { printf("A null query!\n"); }
	 ;

field_list: field
		  | field_list ',' field
		  ;

field: HIGH { printf("We want HIGH\n"); }
	 | LOW  { printf("We want LOW\n"); }
	 | GRAPH { printf("We want GRAPH\n"); }
	 | MOVE {printf("We want MOVE\n");}
	 | PREDICTION {printf("We want PREDICTION\n");}
	 ;

constr_list: constr_list KAND constr {
	$$ = $1;
	$$->next = $3;
}
		   | constr
		   ;

constr: NAME '=' WORD {
	$$ = malloc(sizeof(struct s_constr));
	$$->next = NULL;
	$$->f = $1;
	$$->v = $3;
}
	  | DATE '=' '(' range ')' {
	  	$$ = malloc(sizeof(struct s_constr));
	$$->next = NULL;
	  	$$->f = "DATE RANGE";
	  	$$->v = calloc(44, 1); 
	  	strcat ($$->v, get_timedate_str($4->start));
	  	strcat($$->v, " - ");
	  	strcat($$->v, get_timedate_str($4->end));
	  }
	  | DATE '=' timedate {
	  	$$ = malloc(sizeof(struct s_constr));
	$$->next = NULL;
	  	$$->f = $1;
	  	$$->v = get_timedate_str($3);
	  }
	  ;



range: timedate '-' timedate {
		$$ = malloc(sizeof(struct s_range));
		$$->start = $1;
		$$->end = $3;
	}
	;

timedate: date time {
		$$ = malloc(sizeof(struct s_timedate));
		$$->d = $1;
		$$->t = $2;
	}
	;

date: NUMBER '/' NUMBER '/' NUMBER {
		$$ = malloc(sizeof(struct s_date));
		if($1 > 13)	yyerror("Month out of range");
		if($3 > 31)	yyerror("Day out of range");
		if($5 < 0)	yyerror("Year out of range");
		$$->m = $1;
		$$->d = $3;
		$$->y = $5;
	}
	;

time: NUMBER ':' NUMBER ':' NUMBER {
		$$ = malloc(sizeof(struct s_time));
		if ($1 > 23) yyerror("Hour out of range");
		if ($3 > 59) yyerror("Minute out of range");
		if ($5 > 59) yyerror("Second out of range");
		$$->h = $1;
		$$->m = $3;
		$$->s = $5;
	}
	;

%%

char *get_timedate_str(struct s_timedate *std){
	char *str = calloc(20, 1);
	sprintf(str, "%i/%i/%i %i:%i:%i", (std->d->m), (std->d->d), (std->d->y), (std->t->h), (std->t->m), (std->t->s));
	return str;
}

int main(){
	return yyparse();
}
