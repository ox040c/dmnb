#include "Page.hpp"
#include <fstream>
using namespace std;
void Page::read_from_file()
{
	ifstream infile;
	infile.open(filename.c_str(), ios::binary | ios::in);
	infile.seekg(pages_addr, ios::beg);
	infile.read(data, 4096);
	call_time = 0;
	infile.close();
} 

void Page::write_to_file()
{
	ofstream outfile;
	outfile.open(filename.c_str(), ios::binary | ios::out | ios::app);
	outfile.seekp(pages_addr, ios::beg);
	outfile.write(data, 4096);
	call_time = -1;
	outfile.close();
}

void Page::read_page(pagedata &info)
{
	for (unsigned int i = 0; i < info.datalen; i++)
	{
		info.data[i] = data[info.offset + i];
	}
}

void Page::write_page(const pagedata info)
{
	for (unsigned int i = 0; i < info.datalen; i++)
	{
		data[info.offset+i] = info.data[i];
	}
}
