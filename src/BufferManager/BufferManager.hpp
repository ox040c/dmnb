#ifndef BUFFER_MANAGER_h
#define BUFFER_MANAGER_H
#include "Utility.hpp"
#include "Page.h"
#include <string>
#include <fstream>
#include <map>

using namespace std;
class BufferManager{
private:
	ifstream infile;
	ofstream outfile;
	map<int, string> pages_table;
	map<int, int> pages_write_time;
	map<int, unsigned int> pages_addr;
	Page page[100];
public:
	BufferManager();
	BufferManager();
	~BufferManager();
	unsigned int Insert(DataAddr & addr,char * data);
	void Search(DataAddr & addr, char * ReturnDate);
	void Delete(DataAddr & addr);
	void Update(DataAddr & addr, char * ReturnDate);
};
#endif