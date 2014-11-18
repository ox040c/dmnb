#ifndef IndexManager_HPP
#define IndexManager_HPP
#include ""
#include <list>
#include <map>
#include <set>
#include <string>
#include "BtreeIndex.hpp"

class IndexManager{
private:
	int indexcount;
    std::map<std::string,std::string> indexList;//表+属性--索引名
    std::map<std::string,int> rootList;//表名根节点地址
    std::map<std::string,int> headList;//表名头节点地址
    std::map<std::string,int> dataLength;//每个表一个节点的长度
    std::map<std::string,int> dataTypeList;//每个表key种类
	BtreeIndex *Btree;
	
public:
    bool hasIndex(const std::string tableName,const std::string attrName);
    int drop_index(const std::string indexName);
    void create_index(const std::string tableName,
                      const std::string colName,
                      const std::string indexName,
                      DataType type);
// 在 tableName 的 colName 上建立索引 indexName, 这个 colName 的类型是 type
	int search(int tid,int attrid,int key,int condition_info);
	int search(int tid,int attrid,float key,int condition_info);
    int search(tr);
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
