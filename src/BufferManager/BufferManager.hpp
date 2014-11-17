#ifndef BUFFER_MANAGER_h
#define BUFFER_MANAGER_H
#include "Utility.hpp"
#include "Page.hpp"
#include <string>
#include <fstream>
#include <map>
#include <vector>
class BufferManager{
private:
	Page page[100];
	int min_call();
	int get_pageid(FilePtr);//�ҵ���ҪѰ�ҵ�page�����������⽫block���뵽page������pageID
	void read_from_file(int,FilePtr);
public:
	BufferManager();
	~BufferManager();
	void creat(std::string name);//����һ��������һ��block�洢��������ݵ�block ����Ŀ
	FilePtr Insert(FilePtr addr, const char * data); //ͨ��DataAddr����������ļ�
	void Search(FilePtr addr, char * ReturnDate);//����ͨ��ReturnDate ����
	void Delete(FilePtr addr); //ֱ��ɾ��ָ���ص��ָ�����ȣ�ͨ����ɾ��ʵ��,��¼��del_filename.txt��
	void Update(FilePtr addr, const char * date);//����Ҫ�޸ĵ����ݴ�����
};
#endif