
RULE := parse
CC := g++

all: $(RULE).o

$(RULE).tab.c $(RULE).tab.h: $(RULE).y ../Utility/Utility.hpp
	bison -d $(RULE).y

lex.yy.c: $(RULE).tab.h $(RULE).l
	flex $(RULE).l

$(RULE).o: main.o lex.yy.c $(RULE).tab.c api.o
	$(CC) -o $@ $^

main.o: ../main.cpp
	g++ ../main.cpp -c -o main.o

api.o:  ../API/API.cpp ../API/API.hpp
	g++ ../API/API.cpp -c -o api.o
	
clean:
	@rm -f lex.yy.c $(RULE).tab.h $(RULE).tab.c $(RULE).o
	@rm -f temp.sql main.o
