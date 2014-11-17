#include "BufferManager.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct kk{
	int k;
	char name[30];
};

int masin()
{
	char page[4096];
	char *temp;
	cout << sizeof(char) << endl;
	kk kkk[3] = { 1, "a", 2, "b", 3, "c" };
	ofstream outfile("kk.dat", ios::binary);
	if (!outfile)
	{
		cout << "out open error" << endl;
	}
	outfile.seekp(1000, ios::beg);
	for (int i = 0; i < 3; i++)
	{
		temp = (char *)&kkk[i];
		outfile.write(temp, sizeof(kkk[0]));
	}
	outfile.close();
	fstream infile("kk.dat", ios::in | ios::binary);
	if (!infile)
	{
		cout << "in open error" << endl;
	}
	kk *out;
	int *a;
	infile.seekg(0, ios::beg);
	infile.read((char *)&page, 4096);
	out = (kk *)page;
	a = (int *)page;
	cout << hex << *a << endl;
	infile.seekg(0, ios::beg);
	infile.close();
	return 0;
}

BufferManager::BufferManager(){}
BufferManager::~BufferManager(){}
bool check_in_page(Page page, FilePtr dataaddr)
{
	bool result = false;
	if (page.call_time > 0 && page.filename == dataaddr.filename)
	{
		if (page.pages_addr <= dataaddr.dataaddr && page.pages_addr + 4096 >= dataaddr.dataaddr)
			result = true;
	}
	return result;
}

int BufferManager::min_call()
{
	int result, min = 1000000;
	for (int i = 0; i < 100; i++)
	{
		if (min > page[i].call_time)
		{
			min = page[i].call_time;
			result = i;
		}
	}
	return result;
}


int BufferManager::get_pageid(FilePtr addr)
{
	int page_id = -1;
	//检查是否hit
	for (int i = 0; i < 100; i++)
	{
		if (check_in_page(page[i], addr))
		{
			return i;
		}
	}
	//检查是否有空的page
	for (int i = 0; i < 100; i++)
	if (page[i].call_time == -1)
	{
		page_id = i;
		read_from_file(page_id, addr);
		return page_id;
	}

	//插入到访问次数最小的page中
	page_id = min_call();
	page[page_id].write_to_file();
	read_from_file(page_id, addr);
	return page_id;
}

void BufferManager::read_from_file(int page_id, FilePtr addr)
{
	page[page_id].filename = addr.filename;
	page[page_id].pages_addr = (addr.dataaddr / 4096) * 4096;
	page[page_id].read_from_file();
}

FilePtr BufferManager::Insert(FilePtr addr, const char * data) //通过DataAddr决定插入的文件
{
	FilePtr result=addr;
	ifstream infile;
	unsigned int blocknum;
	unsigned int key;
	int page_id;
	vector<unsigned int> deleted;
	string filename = "del_" + addr.filename + ".txt";
	infile.open(filename.c_str(), ios::in);
	while (infile >> key)
	{
		deleted.push_back(key);
	}
	infile.close();
	//表格中有空间可以做插入，直接插入所对应的位置
	if (deleted.size() != 0)
	{
		result.dataaddr = deleted[0];
		deleted.erase(deleted.begin());//把表中的数据再写回文件中
		writedeleted(filename, deleted);
		
		Update(result,data);
	}//表格中没有空间做插入，需要向file中增加一个block
	fstream file;
	file.open(addr.filename.c_str(), ios::in | ios::binary | ios::out | ios::app);
	file.seekg(0, ios::beg);
	//将表格中的信息块数据更新
	file.read((char *)&blocknum, 4096);
	blocknum++;
	file.write((char *)&blocknum, 4096);
	file.close();

	result.dataaddr = 4096 * blocknum;
	Update(result, data);
	//新建了一个表格之后
	for (int i = 1; i*result.datalen < 4096; i++)
		deleted.push_back(result.dataaddr + i*result.datalen);
	writedeleted(filename, deleted);
	return result;
}
void writedeleted(string name,vector <unsigned int> deleted)
{
	ofstream outfile;
	outfile.open(name.c_str(), ios::trunc);
	vector<unsigned int>::iterator it;
	for (it = deleted.begin(); it != deleted.end(); it++)
	{
		outfile << *it;
	}
	outfile.close();
}
void BufferManager::Search(FilePtr addr, char * ReturnDate)//数据通过ReturnDate 返回
{
	int page_id = -1;
	unsigned int offset;

	page_id = get_pageid(addr);
	offset = addr.dataaddr - page[page_id].pages_addr;

	page[page_id].call_time++;
	for (unsigned int i = 0; i < addr.datalen; i++)
		ReturnDate[i] = page[page_id].data[offset + i];
}
void BufferManager::Delete(FilePtr addr) //直接删除指定地点的指定长度，通过懒删除实现,记录在del_filename.txt中
{
	ofstream outfile;
	string filename = "del_" + addr.filename + ".txt";
	outfile.open(filename.c_str(), ios::out | ios::app);
	outfile << addr.dataaddr;
	outfile.close();
}
void BufferManager::Update(FilePtr addr, const char * date)//直接将需要更新的地址送给
{
	int page_id = -1;
	int offset;

	page_id = get_pageid(addr);
	offset = addr.dataaddr - page[page_id].pages_addr;

	for (unsigned int i = 0; i < addr.datalen; i++)
		page[page_id].data[offset + i] = date[i];
}

void BufferManager::creat(string name)
{
	fstream file(name.c_str());
	if (file)
	{
		cerr << "exist error!" << endl;
		abort();
	}
	else
	{
		ofstream file(name.c_str(), ios::binary);
		file << (unsigned int)0;
	}
}