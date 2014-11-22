 #include "BufferManager.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

BufferManager::BufferManager(){}
BufferManager::~BufferManager()
{
	for (int i = 0; i < 100; i++)
	{
		if (page[i].call_time != -1)
		{
			page[i].write_to_file();
		}
	}
	
	//cout << "~BufferManager" << endl;
}
bool check_in_page(const Page &page, const FilePtr &dataaddr)
{
	bool result = false;
	if (page.call_time > 0 && page.filename == dataaddr.filename)
	{
		if (page.pages_addr <= dataaddr.dataaddr && page.pages_addr + 4096 > dataaddr.dataaddr)
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

void writedeleted(string name, vector <unsigned int> deleted)
{
	ofstream outfile;
	string filename = "del_" + name + ".txt";
	outfile.open(filename.c_str());
	vector<unsigned int>::iterator it;
	for (it = deleted.begin(); it != deleted.end(); it++)
	{
		outfile << *it << " ";
	}
	outfile.close();
}

vector<unsigned int> readdeleted(string name)
{
	ifstream infile;
	unsigned int  key;
	vector<unsigned int> result;
	string filename = "del_" + name + ".txt";
	infile.open(filename.c_str(), ios::in);
	while (infile >> key)
	{
		result.push_back(key);
	}
	infile.close();
	return result;
}

FilePtr BufferManager::Insert(const FilePtr &addr, const char * data) //通过DataAddr决定插入的文件
{
	FilePtr result = addr;
	unsigned int blocknum;
	vector<unsigned int> deleted;

	deleted = readdeleted(result.filename);
	//表格中有空间可以做插入，直接插入所对应的位置
	if (deleted.size() != 0)
	{
		result.dataaddr = deleted[0];
		deleted.erase(deleted.begin());//把表中的数据再写回文件中
		writedeleted(result.filename, deleted);
		Update(result, data);
		return result;
	}//表格中没有空间做插入，需要向file中增加一个block
	fstream file;
	file.open(result.filename.c_str(), ios::in | ios::out | ios::binary );
	file.seekg(0,ios::beg);
	//将表格中的信息块数据更新
	file.read((char *)&blocknum, sizeof(unsigned int));
	blocknum++;
	file.seekp(0,ios::beg);
	file.write((char *)&blocknum, sizeof(unsigned int));
	file.close();
	result.dataaddr = 4096 * blocknum;

	//addblock(addr);
	fstream outfile(result.filename.c_str(), ios::in | ios::out | ios::binary);
	outfile.seekp(result.dataaddr, ios::beg);
	char * ch = new char[4096];
//	outfile.write(ch, 4096);
	outfile.close();
	Update(result, data);
	//新建了一个表格之后
	for (unsigned int i = 1; i*result.datalen < 4096; i++)
	{
		deleted.push_back(result.dataaddr + i*result.datalen);
	}
	writedeleted(result.filename, deleted);
	return result;
}
void BufferManager::Search(const FilePtr &addr, char * ReturnDate)//数据通过ReturnDate 返回
{
	int page_id = -1;
	unsigned int offset;

	page_id = get_pageid(addr);
	offset = addr.dataaddr - page[page_id].pages_addr;

	page[page_id].call_time++;
//	for (unsigned int i = 0; i < addr.datalen; i++)
//		ReturnDate[i] = page[page_id].data[offset + i];
	memcpy(ReturnDate, page[page_id].data + offset, addr.datalen);
}
void BufferManager::Delete(const FilePtr &addr) //直接删除指定地点的指定长度，通过懒删除实现,记录在del_filename.txt中
{
	ofstream outfile;
	string filename = "del_" + addr.filename + ".txt";
	outfile.open(filename.c_str(), ios::out | ios::app);
	outfile << addr.dataaddr<< " ";
	outfile.close();
}
void BufferManager::Update(const FilePtr &addr, const char * data)//直接将需要更新的地址送给
{
	int page_id = -1;
	int offset;

	page_id = get_pageid(addr);
	offset = addr.dataaddr - page[page_id].pages_addr;
	page[page_id].call_time++;
//	for (unsigned int i = 0; i < addr.datalen; i++)
//		page[page_id].data[offset + i] = date[i];
	memcpy(page[page_id].data + offset, data, addr.datalen);
}

void BufferManager::Creat(const FilePtr &addr)
{
	unsigned int datalen = addr.datalen;
	unsigned int blocknum = 0;
	char * ch,*temp;
	ch = new char[4096];
	temp = ch + sizeof(unsigned int);

	for (int i = 0; i < sizeof(unsigned int); i++)
	{
		ch[i] = *((char *)&blocknum + i);
		temp[i] = *((char *)&datalen+i);
	}
	fstream file(addr.filename.c_str());
	if (file)
	{
		cerr << "exist error!" << endl;
		abort();
	}
	else
	{
		ofstream file(addr.filename.c_str(), ios::binary);
		file.write(ch, 4096);
	}
	delete[] ch;
}

FilePtr BufferManager::NextAddr(const FilePtr &addr)
{
	vector <unsigned int> deleted;
	unsigned blocknum;
	FilePtr result = addr;
	deleted = readdeleted(result.filename);

	fstream file;
	file.open(result.filename.c_str(), ios::in | ios::binary | ios::out);
	file.seekg(0, ios::beg);
	file.read((char *)&blocknum, sizeof(unsigned int));
	file.close();

	result.dataaddr += result.datalen;
	vector<unsigned int>::iterator it = find(deleted, result.dataaddr);
	while (it != deleted.end())
	{
		result.dataaddr += result.datalen;
		it = find(deleted, result.dataaddr);
	}
	if (result.dataaddr >= 4096 * (blocknum + 1))
		result.dataaddr = -1;
	return result;
}

vector<unsigned int>::iterator BufferManager::find(vector<unsigned int> deleted,unsigned int num)
{
	vector<unsigned int>::iterator it = deleted.begin();
	while (it != deleted.end())
	{
		if (*it == num) return it;
		it++;
	}
	return it;
}

bool BufferManager::Has(const FilePtr &addr)
{
	fstream file(addr.filename.c_str());
	if (file)
		return true;
	else 
		return false;
}

void BufferManager::Drop(const FilePtr addr)
{
	string operater1 = "del " + addr.filename;
	string operater2 = "del del_" + addr.filename;
	system(operater1.c_str());
	system(operater2.c_str());
}