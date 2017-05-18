%{
#include <stdio.h>
extern int yylex();
extern void yyerror(char *, ...);
%}

%token SELECT WHERE HIGH LOW GRAPH ETC
%start query_list
%%

query_list: query
		  | query_list query

query: SELECT field_list WHERE constr_list ';' { printf("A valid query!!!\n"); }
	 ;

field_list: field
		  | field_list ',' field
		  ;

field: HIGH { printf("We want HIGH\n"); }
	 | LOW  { printf("We want LOW\n"); }
	 | GRAPH { printf("We want GRAPH\n"); }
	 ;

constr_list: constr_f '=' ETC
		   ;

constr_f: ETC
		;

%%

int main(){
return yyparse();
}
