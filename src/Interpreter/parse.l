%option noyywrap
%option yylineno
%{
#include <cstdio>
#include <iostream>

#include "../Utility/Utility.hpp"

using namespace std;
#define YY_DECL extern "C" int yylex()

#include "parse.tab.h"  // to get the token types that we return

// TODO: void yyerror(char *s, ...);

%}
%%


[ \t\n\r] /* whitespace */
[0-9]+\.[0-9]+ { yylval.fval = atof(yytext); return FLOAT_v; }
[0-9]+            { yylval.ival = atoi(yytext); return INT_v; }
"//".* /* ignore comments */

add|ADD { return ADD; }
and|AND { return AND; }
char|CHAR(ACTER)? { return CHAR; }
create|CREATE { return CREATE; }
database|DATABASE { return DATABASE; }
databases|DATABASES { return DATABASES; }
delete|DELETE { return DELETE; }
drop|DROP { return DROP; }
exit|EXIT { return EXIT; }
float|FLOAT4? { return FLOAT_type; }
from|FROM { return FROM; }
index|INDEX { return INDEX; }
insert|INSERT { return INSERT; }
int|INT4?|INTEGER { return INT_type; }
into|INTO { return INTO; }
key|KEY { return KEY; }
not|NOT { return NOT; }
on|ON { return ON; }
primary|PRIMARY { return PRIMARY; }
select|SELECT { return SELECT; }
table|TABLE { return TABLE; }
values?|VALUES? { return VALUES; }
unique|UNIQUE { return UNIQUE; }
where|WHERE { return WHERE; }

quit|QUIT { return QUIT; }

'(\\.|''|[^'\n])*' |
\"(\\.|\"\"|[^"\n])*\" {
char temp[257]; 
if (strlen(yytext) > 256) cout << "warn: too long a string, truncated\n";
strcpy(temp, yytext + 1);
temp[strlen(temp) - 1 > 255? 255: strlen(temp) - 1] = 0;
yylval.sval = strdup(temp); 
return STRING;
}
'(\\.|[^'\n])*$ { printf("Unterminated string %s", yytext); }
\"(\\.|[^"\n])*$ { printf("Unterminated string %s", yytext); }

[a-zA-Z0-9]+      {
	// we have to copy because we can't rely on yytext not changing underneath us:
	yylval.sval = strdup(yytext);
	return STRING;
}

[\*\(\)\{\}:,;] { return yytext[0]; }

">=" { yylval.comp_sign = utls::NO_LESS ; return OP; }
"<=" { yylval.comp_sign = utls::NO_MORE ; return OP; }
"<>" { yylval.comp_sign = utls::UNEQUAL ; return OP; }
">"  { yylval.comp_sign = utls::MORE ; return OP; }
"<"  { yylval.comp_sign = utls::LESS ; return OP; }
"="  { yylval.comp_sign = utls::EQUAL ; return OP; }

. { cout << "mystery character " << yytext;
//yyerror("mystery character '%c'", *yytext);
}

%%
