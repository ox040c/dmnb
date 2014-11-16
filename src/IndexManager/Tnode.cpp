#ifndef TNODE_H_
#define TNODE_H_

#define NULL 0
#define filepoint int
//#include "file.h"
enum NODE_TYPE{INTERNAL, LEAF};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION{LEFT, RIGHT};   // 兄弟结点方向：左兄弟结点、右兄弟结点
typedef float KeyType;                 // 键类型
typedef int DataType;                  // 值类型
const int ORDER = 20;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1;        // 最小键值个数
const int MAXNUM_KEY = 2*ORDER-1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
//const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
//const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数

// 结点基类
template <class KeyType>
class TNode{
protected:
    int leaf;
    int keynum;
    KeyType keyvalues[MAXNUM_KEY];
    filepoint childpoint[MAXNUM_CHILD];
    filepoint sibling;
    BufferManager &bufferManager;

public:
    TNode(BufferManager &x){
        bufferManager = x;
        leaf = 0;
        keynum = 0;

    }

    virtual ~TNode();
    TNode getFromFile(filepoint p){

    }

    filepoint writeToFile(){
        bufferManager.write(xx,xxx,xx)

    }

    void clear(){

    }

    int isLeaf() const {
        return leaf;
    }

    void setLeaf(int type){
        leaf = type;
    }

    int getKeyNum() const {
        return keynum;
    }

    void setKeyNum(int n){
        keynum = n;
    }

    KeyType getKeyValue(int i) const {
        return keyvalues[i];
    }

    void setKeyValue(int i, KeyType key){
        keyvalues[i] = key;
    }


    int getKeyIndex(KeyType key)const{        // 找到键值在结点中存储的下标
        int left = 0;
        int right = this->keynum;
        int mid = (left+right)/2;
        while (left!=right){
            if (key>keyvalues[mid]) left=mid;else right= mid;
            mid = (left+right)/2;
        }
        return left;
    }
    filepoint getChild(int i) const{
        return childpoint[i];
    }
    void setChild(int i,filepoint p){
        childpoint[i] = p;
    }

    void removeKey(int keyIndex, int childIndex){  // 从结点中移除键值
        int i;
        keynum++;
        for (i=keynum;i>keyIndex;--i){
            keyvalues[i] = keyvalues[i-1];
        }
        keyvalues[keyIndex] = key;
        for (i=keynum;i>childIndex;--i){
            childpoint[i] = childpoint[i-1];
        }
        childpoint[childIndex] = p;
    }

    void insert(int keyIndex, int childIndex,const keyType &key,const filepoint &p){
        int i;
        keynum++;
        for (i=keynum;i>keyIndex;--i){
            keyvalues[i] = keyvalues[i-1];
        }
        keyvalues[keyIndex] = key;
        for (i=keynum;i>childIndex;--i){
            childpoint[i] = childpoint[i-1];
        }
        childpoint[childIndex] = p;
    }

    void split(CNode &parentNode, int childIndex){ // 分裂结点
        Tnode newnode = new <KeyType>Tnode();
        int original = DEGREE;
        int newindex=0;
        for (;original<this->keynum;original++){
            newnode.setKeyValue(newindex,this->keyvalues[original]);
            newnode.setChildpoint(newindex,this->childpoint[original+1]);
            newindex++;
        }
        filepoint page = newnode.writeToFile();
        parentNode.insert(childIndex,childIndex+1,keyvalues[DEGREE],page);
    }

    void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex){  // 合并结点
    }
    void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d) {// 从兄弟结点中借一个键值
    }

};
#endif
