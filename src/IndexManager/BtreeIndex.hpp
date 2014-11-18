#ifndef BtreeIndex
#define BtreeIndex
#include "Tnode.hpp"
#include "BufferManager.hpp"
#include <vector>
typedef unsigned int filepoint;
const int ORDER = 10;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1;        // 最小键值个数
const int MAXNUM_KEY = 2*ORDER-1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数

template <class KeyType>
class BtreeIndex {
private:
    Tnode<KeyType> *root;
    Tnode<KeyType> *first;
    filepoint rootloc;
    filepoint firstloc;
    struct DataAddr fp;
    BufferManager buff;
    map<filepoint,filepoint> father;


//    void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
//    void recursive_remove(CNode* parentNode, KeyType key);
//    bool recursive_search(CNode *pNode, KeyType key)const;
//    void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
//    void search(KeyType key, SelectResult& result);
//    void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
//    void remove(KeyType key, DataType& dataValue);
//    void recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue);

public:
    BtreeIndex(const BufferManager &x,finlpoint rootloc,filepoint firstloc){
        fp.filename = "index.data";
        fp.datalen = 4096;
        root = new Tnode<KeyType>(ReadFromFile (rootloc));
        firstloc = new Tnode<KeyType>(ReadFromFile (firstloc));
        buff = &x;
    }

    BtreeIndex(const BufferManager &x){
        fp.filename = "index.data";
        fp.datalen = 4096;
        root = NULL;
        firstloc = NULL;
        buff = &x;
    }

    ~BtreeIndex(){

    }
    void clear();             // 清空
    void print()const;        // 打印树关键字
    void printData()const;    // 打印数据

    filepoint WritetoFile(Tnode x){
        char* s = new char[4096]
        x.WritetoFile(s);
        delete[] s;
        filepoint p = buff.insert(fp,s);
        return p;
    }

    Tnode ReadFromFile (filepoint p){
        Tnode tempnode;
        fp.dataaddr = p;
        char* s = new char[4096]
        buff.Search(fp,s);
        tempnode.ReadFromFile(s);
        delete[] s;
        return tempnode;
    }

    void UpdateFile(filepoint p,Tnode x){
        char* s = new char[4096];
        x.WritetoFile (s);
        fp.dataaddr = p;
        buff.Update(fp,s);
        delete[] s;
    }

    filepoint find(const KeyType &key){
        Tnode<KeyType> temp;
        filepoint nextnode,tempnode;
        int keyindex;
        int keynum;
        if (root == NULL) return -1;
        temp = *root;
        tempnode = rootloc;
        while (temp.getLeaf ()==0){
            keyindex = temp.getKeyIndex (key);
            keynum = temp.getKeyNum ();
            if (keyindex==keynum){
                nextnode = temp.getChild (keynum);
                father.add(nextnode,tempnode)
                temp = ReadFromFile (nextnode);
                tempnode = nextnode;
            }
            else{
                nextnode = temp.getChild (keyindex);
                father.add(nextnode,tempnode)
                temp = ReadFromFile (nextnode);
                tempnode = nextnode;
            }
            keyindex = temp.getKeyIndex (key);
            if (keyindex>0 && key == temp.getKeyValue (keyindex-1)){
                return temp.getChild (keyindex);
            }
            else{
                return -1;
            }
        }
    }

    filepoint findLeaf(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint nextp,temp;
        int keyindex;
        int keynum;
        if (root != NULL){
            tempnode = *root;
            temp = rootloc;
            while (tempnode.getLeaf ()==0){
                keyindex = tempnode.getKeyIndex (key);
                keynum = tempnode.getKeyNum ();
                if (keyindex==keynum){
                    nextp = tempnode.getChild (keynum);
                    father.add(nextp,temp)
                    tempnode = ReadFromFile (nextp);
                    temp = nextp;
                }
                else{
                    nextp = tempnode.getChild (keyindex);
                    father.add(nextp,temp)
                    tempnode = ReadFromFile (nextp);
                    temp = nextp;
                }
            }
            return temp;
        }else std::cout<<"error in index findleaf"<<endl;
    }

    vector<filepoint> select(KeyType compareKey, int compareOpeartor){    // 范围查询，BETWEEN

    }

    bool insert_in_leaf(Tnode<KeyType> &node,const KeyType &key,const filepoint& data){
        node.insert (key,data);
        return true;
    }
    bool insert_in_parent(filepoint x,const KeyType &key,filepoint y){
        if (x == rootloc){
            Tnode<KeyType> newroot;
            newroot.setKeyNum (1);
            newroot.setChild (0,x);
            newroot.setChild (1,y);
            newroot.setKeyValue (0,key);
            return true;
        }else{
            filepoint parent = father.get("")

        }
    }

    bool insert(const KeyType& key, const filepoint& data){
        if (root==NULL){
            root = new Tnode();
            first = root;
            root->insert (key,data);
            firstloc = WritetoFile (*first);
            rootloc = firstloc;
            return true;
        }                                                                          //空树
        Tnode<KeyType> tempnode;
        filepoint temp;
        int keyindex;
        temp = findLeaf (key);
        tempnode = ReadFromFile (temp);
        keyindex = tempnode.getKeyIndex (key);
        if (keyindex>0 && key == tempnode.getKeyValue (keyindex-1)){
            return false;
        }                                                                       //已经在树中
        if (tempnode.getKeyNum ()<MAXMUN_KEY){
            insert_in_leaf();
            UpdateFile (temp,tempnode);
        }else{
            Tnode<KetType> newnode;
            insert_in_leaf(tempnode,key,data);
            for (int i=0;(i+ORDER)<MAXNUM_KEY+1;++i){
                newnode.setKeyValue (i,tempnode.getKeyValue (i+ORDER));
                newnode.setChild (i,tempnode.getChild (i+ORDER));
            }
            newnode.setChild (MAXNUM_CHILD-1,tempnode.getChild (MAXNUM_CHILD-1));
            tempnode.setKeyNum (ORDER);
            tempnode.removeKey (ORDER,ORDER+1);
            filepoint newp = WritetoFile (newnode);
            tempnode.setChild (MAXNUM_CHILD-1,newp);
            KeyType newkey = newnode.getChild (0);
            insert_in_parent(temp,newkey,newp);
        }
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
               clear()
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



/* data */
private:

    int recursive_search(CNode *pNode, KeyType key)const{
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
    filepoint WritetoFile(Tnode x){
        char* s = new char[4096];
        x.WritetoFile(s);
        filepoint p = buff.Insert(fp,s);
        delete[] s;
        return p;
    }
    Tnode<KeyType> ReadFromFile (filepoint p){
        Tnode<KeyType> tempnode;
        fp.dataaddr = p;
        char* s = new char[4096];
        buff.Search(fp,s);
        tempnode.ReadFromFile(s);
        delete[] s;
        return tempnode;
    }
};
#endif

