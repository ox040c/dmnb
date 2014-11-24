%{
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

#include "parse.tab.h"  // to get the token types that we return
#include "parse.h" // own header file

// stuff from flex that bison needs to know about:
extern "C" int yylex();
//extern "C" int yyparse();
extern "C" FILE *yyin;


void yyerror(const char *s);

#include "../Utility/Utility.hpp"
#include "../API/API.hpp"

extern PlanList& parse(string str);
PlanList plist;
void clear();
void exec();
void apnd(const Wrapper& a);
// global vars
WrapperList wlist;

Action acti = TOTAL_ACTION;
std::string tname;

%}

%union {
	int ival;
	float fval;
	char *sval;
	char *ssval;
    int comp_sign;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT_v
%token <fval> FLOAT_v
%token <ssval> STRING_v
%token <sval> STRING
%token <comp_sign> OP

// *****
// * SQL keywords
// *****
%token ADD
%token AND
%token CHAR_t
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
      statement ';' { exec(); }
    | list statement ';' { exec(); }
    ;

// TODO: this is a nasty implementaion, needs improvement
quit: QUIT | EXIT { clear(); //cout << "I quit\n"; 
throw logic_error("par_quit"); }

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
        { acti = CTBL; tname = $2; }
    | INDEX STRING ON STRING '(' STRING ')'
        { acti = CIDX; tname = $4;
        apnd(Wrapper(string($6), utls::CHAR, 0, string($2))); }
    ;

attr_list:
      attr_list ',' attr { cout << ""; }
    | attr { cout << ""; }
    ;

attr:
      STRING CHAR_t '(' INT_v ')' {
        apnd(Wrapper(string($1), utls::CHAR, $4));
    }
    | STRING INT_type {
        apnd(Wrapper(string($1), utls::INT));
    }
    | STRING FLOAT_type {
        apnd(Wrapper(string($1), utls::FLOAT));
    }
    | attr UNIQUE { wlist.back().isUnique = true; }
    | PRIMARY KEY '(' STRING ')' {
        int keyCount = 0;
        for ( list<Wrapper>::iterator i = wlist.begin();
             i != wlist.end(); i++ ) {
            if ( i->name == string($4) ) {
                i->isIndex = true;
                keyCount++;
            }
        }
        if (keyCount != 1) {
            yyerror("primary key: specified attr not found,"
                    " key must be defined at the end of the attr list."
                    " Or more than 2 keys defined");
        }
    }
    ;

select_stmt:
      '*' FROM STRING { acti = SELV; tname = $3; }
    | '*' FROM STRING WHERE condition_list { acti = SELV; tname = $3; }
    ;

delete_stmt:
      FROM STRING { acti = DELV; tname = $2; }
    | FROM STRING WHERE condition_list { acti = DELV; tname = $2; }

    ;

condition_list:
      expr { cout << ""; }
    | condition_list AND expr { cout << ""; }
    ;

expr:
      STRING OP STRING_v { string temp($3); 
            temp.erase(temp.length() - 1, 1); temp.erase(0, 1); //cout << temp;
            apnd(Wrapper(string($1),
        (op_t) $2, string(temp.c_str())));
    }
    | STRING OP INT_v { apnd(Wrapper(string($1),
        (op_t) $2, $3));
    }
    | STRING OP FLOAT_v { apnd(Wrapper(string($1),
        (utls::Operator) $2, $3));
    }
    ;

drop_stmt:
      TABLE STRING { acti = DTBL; tname = $2; }
    | INDEX STRING { acti = DIDX; tname = $2; }
    ;

insert_stmt:
      INTO STRING VALUES '(' var_list ')'
        { acti = INSV; tname = $2; }
    ;

var:
      STRING_v { 
            string temp($1);
            temp.erase(0, 1); temp.erase(temp.length() - 1, 1); 
            apnd(Wrapper(utls::CHAR, string(temp.c_str()))); 
      }
    | FLOAT_v { apnd(Wrapper(utls::FLOAT, $1)); }
    | INT_v { apnd(Wrapper(utls::INT, $1)); }
    ;

var_list:
      var
    | var_list ',' var
    ;

%%

PlanList& parse(string str, int action) {

    // put the input string into a temporary file
    // where the parser to read from
    ofstream fout;
    try {

        fout.open("temp.sql");
        fout << str;
        fout.close();

    }
    catch (...) {

        cerr << ":-( sry, cannot create temp file\n";
	throw runtime_error("par_file");

    }
    string fileName;
    if ( action ) fileName = str;
    else fileName = "temp.sql";
    // open a file handle to a particular file:
    FILE *myfile = fopen(fileName.c_str(), "r");
    // make sure it is valid:
    if (!myfile) {
        cout << "I can't open temp.sql file!" << endl;
        plist.clear();
        //remove("temp.sql");
        return plist;
    }
    // set flex to read from it instead of defaulting to STDIN:
    yyin = myfile;
    plist.clear();
    // parse through the input until there is no 
    
    //flushzz();
    try {
        do {
            clear();
            yyparse();
        } while (!feof(yyin));
    }
    catch (logic_error const &e) {
        cerr << e.what() << "exit in sql, exec stmt before exit\n";
    }
    
    fclose(myfile);
    remove("temp.sql");
    return plist;
}

void clear() {

    wlist.clear();

}

void apnd(const Wrapper& a) {

    //cout << "parse.y: " << a.floatv << endl;
    
    wlist.push_back(a);

}

void yyerror(const char *s) {
    //extern yylineno;
    //cerr << "Eek, parse error! Message: " << s << "near line: " << yylineno << endl;
    throw runtime_error("parse_error_syntax");
}

void exec() {

    if ( acti == TOTAL_ACTION ) {
        cerr << "Eek, parser internal error: non-act\n";
        throw runtime_error("par_int");
    }
            
    //cout << acti << endl;
    plist.push_back(Plan(wlist, tname, acti));
    

    acti = TOTAL_ACTION;
    clear();

}
