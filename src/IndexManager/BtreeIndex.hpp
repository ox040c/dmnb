#ifndef B_TREE_INDEX_HPP
#define B_TREE_INDEX_HPP

#include "Tnode.hpp"
#include "BufferManager.hpp"
#include <vector>
typedef unsigned int filepoint;
template <class KeyType>
class BtreeIndex {
private:
    Tnode<KeyType> root;
    Tnode<KeyType> first;
    filepoint rootloc;
    filepoint firstloc;
    struct DataAddr fp;

/*
    void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
    void recursive_remove(CNode* parentNode, KeyType key);
    bool recursive_search(CNode *pNode, KeyType key)const;
    void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
    void search(KeyType key, SelectResult& result);
    void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
    void remove(KeyType key, DataType& dataValue);
    void recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue);

public:
    BtreeIndex(filepoint r,filepoint f){
        char* s = (char*)malloc(4096);
        fp.filename = "index.data";
        fp.datalen = 4096;

        readFrom();
        firstloc =f;
    }

    filepoint WritetoFile(Tnode x){
        char* s = (char*)malloc(4096);
        x.WritetoFile(s);
        filepoint p = buff.insert(fp,s);
        return p;
    }

    Tnode ReadFromFile (filepoint p){
        Tnode tempnode;
        fp.dataaddr = p;
        char* s =(char*)malloc(4096);
        buff.Search(fp,s);
        tempnode.ReadFromFile(s);
        return tempnode;
    }

	~BtreeIndex();

    bool haskey(KeyType key){
        return recursive_search(m_Root, key);
    }

    bool recursive_search(CNode *pNode, KeyType key)const{
        if (pNode==NULL) {//检测节点指针是否为空，或该节点是否为叶子节点
            return false;
        }
        else {
            int keyIndex = pNode->getKeyIndex(key);
            int childIndex = keyIndex; // 孩子结点指针索引
            if (keyIndex >=0 && keyIndex<pNode->getKeyNum() && key==pNode->getKeyValue(keyIndex-1)){
                return true;
            }
            else{
                if (pNode->isleaf()){   //检查该节点是否为叶子节点
                    return false;
                }
                else{
                    return recursive_search(pNode->getChild(childIndex), key);
                }
            }
        }
    }

    bool insert(KeyType key, const filepoint& data){
        if (haskey(root,key)){
            return false;
        }
        if (root==NULL){
            root = new Tnode();
            first = root;
        }
        if (root->getKeyNum()>=MAXMUN_KEY){
            Tnode<KeyType>* newnode = new Tnode<>();
            filepoint p = root->getself();
            newnode->setChild(0,p);
            root->split(newnode,0);
            root = newnode;
            rootloc =newnode->getself();
        }
        recursive_insert(root,key,data);
        return true;
    }

    bool recursive_insert(Tnode* parentnode, KeyType key,filepoint data){
        if (parentnode->isleaf()){
            parentnode->insert(key,data);
        }
        else{
            int keyIndex = parentnode->getKeyIndex(key);
            int childIndex = keyIndex;
            Tnode childnode = parentnode->getChild(childIndex);
            if (childnode->getKeyNum()>=MAXNUM_LEAF) { // 子结点已满，需进行分裂
                childnode->split(parentNode, childIndex);
                if (parentNode->getKeyValue(childIndex)<=key){   // 确定目标子结点
                    childnode = parentNode->getChild(childIndex+1);
                }
            }
            recursive_insert(childnode, key, data);
        }
    }

    bool remove(KeyType key){
        if (!haskey(key)){  //不存在
            return false;
        }
        if (m_Root->getKeyNum()==1){//特殊情况处理
            if (m_Root->isleaf()){
               clear();
               return true;
               }
            else{
               Tndoe *pChild1 = root->getChild(0);
               Tnode *pChild2 = root->getChild(1);
               if (pChild1->getKeyNum()==MINNUM_KEY && pChild2->getKeyNum()==MINNUM_KEY){
                   pChild1->mergeChild(root, pChild2, 0);
                   delete m_Root;
                   m_Root = pChild1;
               }
           }
        }
        recursive_remove(root, key);
        return true;
    }

    void recursive_remove(CNode* parentNode, KeyType key){
        int keyIndex = parentNode->getKeyIndex(key);
        int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
        if (parentNode->isleaf()) {// 找到目标叶子节点
            parentNode->removeKey(keyIndex, childIndex);  // 直接删除
            // 如果键值在内部结点中存在，也要相应的替换内部结点
            if (childIndex==0 && !root->isleaf() && parentNode!=m_DataHead){
                changeKey(root, key, parentNode->getKeyValue(0));
            }
        }
        else {// 内结点
            Tnode *pChildNode = (parentNode)->getChild(childIndex); //包含key的子树根节点
            if (pChildNode->getKeyNum()==MINNUM_KEY){                       // 包含关键字达到下限值，进行相关操作
                CNode *pLeft = childIndex>0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;                       //左兄弟节点
                CNode *pRight = childIndex<parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;//右兄弟节点
                // 先考虑从兄弟结点中借
                if (pLeft && pLeft->getKeyNum()>MINNUM_KEY){// 左兄弟结点可借
                    pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
                }
                else if (pRight && pRight->getKeyNum()>MINNUM_KEY){//右兄弟结点可借
                    pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
                }
                //左右兄弟节点都不可借，考虑合并
                else if (pLeft){                    //与左兄弟合并
                    pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
                    pChildNode = pLeft;
                }
                else if (pRight){                   //与右兄弟合并
                    pChildNode->mergeChild(parentNode, pRight, childIndex);
                }
            }
            recursive_remove(pChildNode, key);
        }
    }

    vector<filepoint> select(KeyType compareKey, int compareOpeartor);    // 范围查询，BETWEEN
    void clear();             // 清空
    void print()const;        // 打印树关键字
    void printData()const;    // 打印数据
/* data */
};
#endif

