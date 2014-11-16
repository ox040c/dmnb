
RULE := parse
CC := g++

all: $(RULE).o

$(RULE).tab.c $(RULE).tab.h: $(RULE).y
	bison -d $(RULE).y

lex.yy.c: $(RULE).tab.h $(RULE).l
	flex $(RULE).l

$(RULE).o: main.o lex.yy.c $(RULE).tab.c
	$(CC) -o $@ $^
	
main.o: ../main.cpp
	g++ ../main.cpp -c -o main.o
	
clean:
	@rm -f lex.yy.c $(RULE).tab.h $(RULE).tab.c $(RULE).o 
	@rm -f temp.sql main.o
