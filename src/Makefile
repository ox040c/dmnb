
RULE := parse
CC := g++
LIBS := BufferManager.o CatalogManager.o IndexManager.o

all: $(RULE).o

$(RULE).tab.c $(RULE).tab.h: $(RULE).y Utility.hpp
	bison -d $(RULE).y

lex.yy.c: $(RULE).tab.h $(RULE).l
	flex $(RULE).l

$(RULE).o: main.o lex.yy.c $(RULE).tab.c $(LIBS)
	$(CC) -o $@ $^

TableProtobuf.pb.cc TableProtobuf.pb.h: TableProtobuf.proto
	protoc $< --cpp_out=.

%.o:%.cpp
	g++ -c $< -o $@
	
clean:
	@rm -f lex.yy.c $(RULE).tab.h $(RULE).tab.c
	@rm -f temp.sql
	@rm -f TableProtobuf.pb.cc TableProtobuf.pb.h
	@rm -f *.o
