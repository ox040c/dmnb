#ifndef BUFFER_MANAGER_HPP
#define BUFFER_MANAGER_HPP

#include "Utility.hpp"
#include "Page.hpp"  
#include <string>
#include <fstream>
#include <map>
#include <vector>
typedef struct DataAddr FilePtr;
class BufferManager{
private:
	Page page[100];
	int min_call();
	int get_pageid(FilePtr);//找到有要寻找的page，若不存在这将block读入到page，返回pageID
	void read_from_file(int,FilePtr);
    std::vector<unsigned int>::const_iterator find(const std::vector<unsigned int> &deleted, unsigned int num);
	void addblock(FilePtr addr);
public:
	BufferManager();
	~BufferManager();
    FilePtr nextAddr(const FilePtr &addr);//如果是最后一个的话返回的dataaddr=-1;
    void create(const FilePtr &addr);//建立一个表单，第一块block存储表格中数据的block的数目和数据的长度
    FilePtr insert(const FilePtr &addr, const char * data); //通过DataAddr决定插入的文件
    void search(const FilePtr &addr, char * ReturnDate);//数据通过ReturnDate 返回
    void remove(const FilePtr &addr); //直接删除指定地点的指定长度，通过懒删除实现,记录在del_filename.txt中
    void update(const FilePtr &addr, const char * date);//把需要修改的数据传进来
    bool has(const std::string &filename);//检查文件是否存在
    void drop(const FilePtr);//删除整个表单和相应的懒删除记录
};
#endif
