%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <calls.h>
extern int yylex();
extern void yyerror(char *, ...);

char *get_timedate_str(struct s_timedate *);

%}
	
%union{
	int num;
	char *str;
	struct s_date *date;
	struct s_time *utime;
	struct s_range *range;
	struct s_timedate *td;
	struct s_constr *c;
	int f_type;	
	struct query *q;
}

%token SELECT WHERE HIGH LOW GRAPH WORD PRED MOVE NUMBER FROM KAND NAME DATE PRICE

%type<num> NUMBER
%type<str> NAME WORD DATE
%type<date> date
%type<utime> time
%type<td> timedate
%type<range> range
%type<c> constr constr_list
%type<f_type> field field_list
%type<q> query query_list

%start query_list
%%

query_list: query { process($1); $$ = $1; }
		  | query_list query { process($2); $$ = $2; $2->next = $$; }
		  ;

query: SELECT field_list WHERE constr_list ';' {
			$$ = malloc(sizeof(struct query));
			$$->f_type = $2;
			$$->c = $4;
			/*
			printf("Constraints:\n");
			struct s_constr *p = $4;
			while(p){
				printf("%s:%s\n", p->f, p->v);
				p = p->next;
			}
			*/
	 }
	 | ';' { }
	 ;

field_list: field
		  | field_list ',' field { $$ = $1 | $3; }
		  ;

field: HIGH { $$ = F_HIGH; }
	 | LOW  { $$ = F_LOW; }
	 | GRAPH { $$ = F_GRPH; }
	 | MOVE { $$ = F_MOVE; }
	 | PRED { $$ = F_PRED; }
	 | PRICE { $$ = F_PRICE; }
	 ;

constr_list: constr_list KAND constr {
				$$ = $3;
				$3->next = $1;
		   }
		   | constr
		   ;

constr: NAME '=' WORD {
		$$ = malloc(sizeof(struct s_constr));
		$$->next = NULL;
		$$->f = $1;
		$$->v = $3;
		$$->isrange = 0;
	  }
	  | WORD '=' WORD {
		$$ = malloc(sizeof(struct s_constr));
		$$->next = NULL;
		$$->f = $1;
		$$->v = $3;
		$$->isrange = 0;
	  }
	  | DATE '=' '(' range ')' {
	  	$$ = malloc(sizeof(struct s_constr));
		$$->next = NULL;
	  	$$->f = "DATE";
	  	$$->v = calloc(44, 1); 
		$$->isrange = 1;
		$$->r = $4;
		strcat($$->v, "(");
	  	strcat($$->v, get_timedate_str($4->start));
	  	strcat($$->v, " - ");
	  	strcat($$->v, get_timedate_str($4->end));
		strcat($$->v, ")");
		/*
	  }
	  | DATE '=' timedate {
	  	$$ = malloc(sizeof(struct s_constr));
		$$->next = NULL;
	  	$$->f = $1;
	  	$$->v = get_timedate_str($3);
		$$->isrange = 0;
		$$->td = $3;
		*/
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
		if($1 > 12)	yyerror("Month out of range");
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
