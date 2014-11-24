#include "Page.hpp"
#include <stdio.h>
#include <iostream>
#include <stdexcept>
using namespace std;

Page::~Page(){}

void Page::read_from_file()
{
//	cout << "page read" << endl;
//	ifstream infile;
	FILE *infile;
//	infile.open(filename.c_str(), ios::binary | ios::in);
	infile = fopen(filename.c_str(), "rb");
	if (infile == NULL)
	{
		//cerr << "there is no such file:" + filename << endl;
		throw runtime_error( "there is no such file:" + filename );
	}
//	infile.seekg(pages_addr, ios::beg);
	fseek(infile, pages_addr, SEEK_SET);
//	infile.read(data, 4096);
	fread(data, 4096, 1, infile);
	call_time = 0;
//	infile.close();
	fclose(infile);
}  

void Page::write_to_file()
{
//	cout << "page write" << endl;
//	fstream outfile;
	FILE *outfile;
//	outfile.open(filename.c_str(), ios::in | ios::out | ios::binary );
	outfile = fopen(filename.c_str(), "r+b");
//	outfile.seekp(pages_addr, ios::beg);
	fseek(outfile, pages_addr, SEEK_SET);
//	outfile.write(data, 4096);
	fwrite(data, 4096, 1, outfile);
	call_time = -1;
//	outfile.close();
	fclose(outfile);
}
//
//void Page::read_page(pagedata &info)
//{
//	for ( int i = 0; i < info.datalen; i++)
//	{
//		info.data[i] = data[info.offset + i];
//	}
//}
//
//void Page::write_page(const pagedata info)
//{
//	for ( int i = 0; i < info.datalen; i++)
//	{
//		data[info.offset+i] = info.data[i];
//	}
//}
