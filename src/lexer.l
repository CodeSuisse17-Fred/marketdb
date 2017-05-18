%{
#include <stdarg.h>
#include <stdio.h>
#include <parser.tab.h>

%}
%%

 /* Keywords */
"SELECT" { return SELECT; }
"WHERE"  { return WHERE; }

 /* Fields */
"HIGH"	 { return HIGH; }
"LOW"	 { return LOW; }
"GRAPH"	 { return GRAPH; }
"MOVE"	 { return MOVE; }
"PREDICTION" {return PREDICTION; }

[,=;]	 { return yytext[0]; }
[a-zA-z_]+ { return ETC; }

%%
int yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "Error:");
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}
