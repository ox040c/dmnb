EXEC := dmnb.run
RULE := parse
CC := g++
LIBS := BufferManager.o CatalogManager.o RecordManager.o IndexManager.o Page.o TableProtobuf.pb.o 
PROTO := TableProtobuf.pb.cc TableProtobuf.pb.h

#pkg-config --cflags --libs protobuf
PROTOFLAG := -I/usr/local/include -L/usr/local/lib -lprotobuf -lz

all: $(EXEC)

$(RULE).tab.c $(RULE).tab.h: $(RULE).y Utility.hpp
	bison -d $(RULE).y

lex.yy.c: $(RULE).tab.h $(RULE).l
	flex $(RULE).l

#Ahhhh
$(EXEC): main.o lex.yy.c $(RULE).tab.c $(LIBS)
	$(CC) -o $@ $^ $(PROTOFLAG)
######

$(PROTO): TableProtobuf.proto
	protoc $< --cpp_out=.

BufferManager.o: BufferManager.cpp
	$(CC) -c $^ -o $@
	
CatalogManager.o: CatalogManager.cpp TableProtobuf.pb.o
	$(CC) -c $^ -o $@
	
IndexManager.o: IndexManager.cpp
	$(CC) -c $^ -o $@
	
RecordManager.o: RecordManager.cpp
	$(CC) -c $^ -o $@
	
Page.o: Page.cpp
	$(CC) -c $^ -o $@

#pkg-config --cflags --libs protobuf
#TableProtobuf.pb.o: TableProtobuf.pb.cc $(PROTO)
#	$(CC) -c $^ -o $@
	
#%.o:%.cpp:
#	g++ -c $< -o $@
	
clean:
	@rm -f lex.yy.c $(RULE).tab.h $(RULE).tab.c
	@rm -f temp.sql
	@rm -f $(PROTO)
	@rm -f *.o $(EXEC)
