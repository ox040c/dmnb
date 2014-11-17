%{
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

#include "../Utility/Utility.hpp"

using namespace std;

#include "parse.tab.h"  // to get the token types that we return

// stuff from flex that bison needs to know about:
extern "C" int yylex();
//extern "C" int yyparse();
extern "C" FILE *yyin;

extern int parse(string str);

void yyerror(const char *s);
%}

%union {
	int ival;
	float fval;
	char *sval;
    int comp_sign;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token <comp_sign> OP

// *****
// * SQL keywords
// *****
%token ADD
%token AND
%token CHAR
%token CREATE
%token DATABASE
%token DATABASES
%token DELETE
%token DROP
%token EXIT
%token FLOAT_type
%token FROM
%token INTO
%token INDEX
%token INSERT
%token INT_type
%token KEY
%token NOT
%token ON
%token PRIMARY
%token SELECT
%token TABLE
%token VALUES
%token UNIQUE
%token WHERE

// non standard thing
%token QUIT


%%
// parsing rules

list:
      statement ';' { cout << "fst_stmt" << endl; }
    | list statement ';' { cout << "stmt" << endl; }
    ;

// TODO: this is a nasty implementaion, needs improvement
quit: QUIT { cout << "I quit\n"; exit(0); }

statement:
      CREATE create_stmt
    | SELECT select_stmt
    | DROP drop_stmt
    | INSERT insert_stmt
    | DELETE delete_stmt
    | quit
    ;


create_stmt:
      TABLE STRING '(' attr_list ')'
        { cout << "create table" << endl; }
    | INDEX STRING ON STRING '(' STRING ')'
        { cout << "craete index" << $2 << $4 << $6; }
    ;

attr_list:
      attr_list ',' attr { cout << "attr\n"; }
    | attr { cout << "fst attr\n"; }
    ;

attr:
      STRING CHAR '(' INT ')' { cout << "t_char" << $4; }
    | STRING INT_type { cout << "t_int "; }
    | STRING FLOAT_type { cout << "t_float "; }
    | attr UNIQUE { cout << "uniq "; }
    | PRIMARY KEY '(' STRING ')' { cout << "key " << $4 << endl; }
    ;

select_stmt:
      '*' FROM STRING
    | '*' FROM STRING WHERE condition_list
    ;

delete_stmt:
      FROM STRING
    | FROM STRING WHERE condition_list
    ;

condition_list:
      expr { cout << "fst_cond "; }
    | condition_list AND expr { cout << "cond "; }
    ;

expr:
      STRING OP STRING { cout << $1 << $2 << $3; }
    | STRING OP INT { cout << $1 << $2 << $3; }
    | STRING OP FLOAT { cout << $1 << $2 << $3; }
    ;

drop_stmt:
      TABLE STRING { cout << "drop tbl" << $2; }
    | INDEX STRING { cout << "drop idx" << $2; }
    ;

insert_stmt:
      INTO STRING VALUES '(' var_list ')'
        { cout << "insert "; }
    ;

var:
      STRING { cout << " str:" << $1; }
    | FLOAT { cout << " flot:" << $1; }
    | INT { cout << " int:" << $1; }
    ;

var_list:
      var
    | var_list ',' var
    ;

%%

int parse(string str) {

    // put the input string into a temporary file
    // where the parser to read from
    ofstream fout;
    try {

        //fout.open("temp.sql");
        //fout << str;
        //fout.close();

    }
    catch (...) {

        cerr << ":-( sry, cannot create temp file\n";

    }

	// open a file handle to a particular file:
	FILE *myfile = fopen("_temp.sql", "r");
	// make sure it is valid:
	if (!myfile) {
		cout << "I can't open temp.sql file!" << endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));

}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
