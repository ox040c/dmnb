#ifndef IndexManager_HPP
#define IndexManager_HPP

#include <list>
#include <map>
#include <set>
#include "BtreeIndex.hpp"

class IndexManager{
private:
	int indexcount;
//	std::set tid->attrid;
    std::set<int> indexid;
//	std::map<tid,attrid>->filePoint;
	BtreeIndex *Btree;
	
public:
	bool hasIndex(int tid,int attrid);
	bool isIndex(int tid,int indexid);
	int drop_index(int indexid);
	int create_index(int tid,int attrid,int indexid);
// FIXME: void create_index(const std::string tableName,
//                          const std::string colName,
//                          const std::string indexName,
//                          DataType type);
// 在 tableName 的 colName 上建立索引 indexName, 这个 colName 的类型是 type
	int search(int tid,int attrid,int key,int condition_info);
	int search(int tid,int attrid,float key,int condition_info);
	int search(int tid,int attrid,string key,int condition_info);
	int insert(int tid,int eid,int attrid,std::vector<char> v);
// FIXME: void insert(const std::string indexName,
//                    const int &pos,
//                    void *dataPtr);
// 在 indexName 中插入一个点，对应硬盘上的位置 pos，具体数据存在 dataPtr，
// 根据 indexName 可以判断出这个数据是什么类型的（建立索引的时候可以记录一下）

//    int delete(int tid,int eid,int attrid,std::vector<char> v);
// FIXME: `delete` is a reversed word in C++, change a name plz...

// TODO:
    std::string getIndexName(const std::string tableName,
                             const std::string colName);
// 返回表 tableName 建立在 colName 上的索引名，没有建立索引返回空串
};
#endif
