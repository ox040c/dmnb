#ifndef BtreeIndex
#define BtreeIndex
#include "Tnode.cpp"
#include <vector>
#define filepoint int
template <type KeyType>
class BtreeIndex {
private:
    Tnode<KeyType> *root;
    Tnode<KeyType> *first;
    filepoint rootloc;
    filepoint firstloc;

    void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
    void recursive_remove(CNode* parentNode, KeyType key);
    void printInConcavo(CNode *pNode, int count)const;
    bool recursive_search(CNode *pNode, KeyType key)const;
    void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
    void search(KeyType key, SelectResult& result);
    void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
    void remove(KeyType key, DataType& dataValue);
    void recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue);
public:
    BtreeIndex(filepoint r,filepoint f){
        root(r);
        first(f);
        rootloc =r;
        firstloc =f;
    }
	~BtreeIndex();
    bool insert(KeyType key, const filepoint& data);
    bool remove(KeyType key);
    vector<filepoint> select(KeyType compareKey, int compareOpeartor);    // 范围查询，BETWEEN
    bool search(KeyType key); // 查找是否存在
    void clear();             // 清空
    void print()const;        // 打印树关键字
    void printData()const;    // 打印数据
/* data */
};
#endif

#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include "BPlus_node.h"

using namespace std;

enum COMPARE_OPERATOR{LT, LE, EQ, BE, BT, BETWEEN}; // 比较操作符：<、<=、=、>=、>、<>
const int INVALID_INDEX = -1;

struct SelectResult
{
    int keyIndex;
    CLeafNode* targetNode;
};

class CBPlusTree{
public:
    CBPlusTree();
    ~CBPlusTree();

    bool update(KeyType oldKey, KeyType newKey);
    // 定值查询，compareOperator可以是LT(<)、LE(<=)、EQ(=)、BE(>=)、BT(>)

private:
    CNode* m_Root;
    CLeafNode* m_DataHead;
    KeyType m_MaxKey;  // B+树中的最大键
};

#endif

