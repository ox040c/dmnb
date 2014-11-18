#include "BufferManager.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
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
	//�����Ƿ�hit
	for (int i = 0; i < 100; i++)
	{
		if (check_in_page(page[i], addr))
		{
			return i;
		}
	}
	//�����Ƿ��пյ�page
	for (int i = 0; i < 100; i++)
	if (page[i].call_time == -1)
	{
		page_id = i;
		read_from_file(page_id, addr);
		return page_id;
	}

	//���뵽���ʴ�����С��page��
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

FilePtr BufferManager::Insert(FilePtr addr, const char * data) //ͨ��DataAddr��������ļ�
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
	//�������пռ����������룬ֱ�Ӳ�������Ӧ��λ��
	if (deleted.size() != 0)
	{
		result.dataaddr = deleted[0];
		deleted.erase(deleted.begin());//�ѱ��е�������д���ļ���
		writedeleted(filename, deleted);
		
		Update(result,data);
	}//������û�пռ������룬��Ҫ��file������һ��block
	fstream file;
	file.open(addr.filename.c_str(), ios::in | ios::binary | ios::out | ios::app);
	file.seekg(0, ios::beg);
	//�������е���Ϣ�����ݸ���
	file.read((char *)&blocknum, 4096);
	blocknum++;
	file.write((char *)&blocknum, 4096);
	file.close();

	result.dataaddr = 4096 * blocknum;
	Update(result, data);
	//�½���һ������֮��
	for (int i = 1; i*result.datalen < 4096; i++)
		deleted.push_back(result.dataaddr + i*result.datalen);
	writedeleted(filename, deleted);
	return result;
}

void BufferManager::Search(FilePtr addr, char * ReturnDate)//����ͨ��ReturnDate ����
{
	int page_id = -1;
	unsigned int offset;

	page_id = get_pageid(addr);
	offset = addr.dataaddr - page[page_id].pages_addr;

	page[page_id].call_time++;
	for (unsigned int i = 0; i < addr.datalen; i++)
		ReturnDate[i] = page[page_id].data[offset + i];
}
void BufferManager::Delete(FilePtr addr) //ֱ��ɾ��ָ���ص���ָ�����ȣ�ͨ����ɾ��ʵ��,��¼��del_filename.txt��
{
	ofstream outfile;
	string filename = "del_" + addr.filename + ".txt";
	outfile.open(filename.c_str(), ios::out | ios::app);
	outfile << addr.dataaddr;
	outfile.close();
}
void BufferManager::Update(FilePtr addr, const char * date)//ֱ�ӽ���Ҫ���µĵ�ַ�͸�
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
