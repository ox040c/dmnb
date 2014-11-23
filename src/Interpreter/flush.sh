#/bin/sh
bison -d parse.y
flex parse.l
mv lex.yy.c lex.yy.cpp
mv parse.tab.c parse.tab.cpp
