%{
#include <stdarg.h>
#include <stdio.h>
#include <parser.tab.h>

%}
%%

 /* Keywords */
"SELECT" { return SELECT; }
"WHERE"  { return WHERE; }
"FROM"   { return FROM; }
"AND"    { return KAND; }
"NAME"   { 
			yylval.str = strdup(yytext);
			return NAME;
		 }
"DATE"	 {
			yylval.str = strdup(yytext);
			return DATE;
		 }

 /* Fields */
"HIGH"	 { return HIGH; }
"LOW"	 { return LOW; }
"GRAPH"	 { return GRAPH; }
"MOVE"	 { return MOVE; }
"PREDICTION" {return PRED; }

[0-9]+  {
			yylval.num = atoi(yytext); 
			return NUMBER;
		}
[-,=/:();]	 { return yytext[0]; }
[a-zA-Z_]+ {
				yylval.str = strdup(yytext);
				return WORD;
			}

%%
int yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "Error:");
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}
