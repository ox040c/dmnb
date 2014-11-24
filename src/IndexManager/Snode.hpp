#ifndef SNODE_H
#define SNODE_H
#include "string.h"
#include "string"
typedef int filepoint;
const int ORDERs = 7;     //7              // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEYs = ORDERs-1;        // 最小键值个数
const int MAXNUM_KEYs = 2*ORDERs-1;      // 最大键值个数
const int MINNUM_CHILDs = MINNUM_KEYs+1; // 最小子树个数
const int MAXNUM_CHILDs = MAXNUM_KEYs+1; // 最大子树个数
const int MINNUM_LEAFs = MINNUM_KEYs;    // 最小叶子结点键值个数
const int MAXNUM_LEAFs = MAXNUM_KEYs;    // 最大叶子结点键值个数
using namespace std;
// 结点基类
class Snode{
public:
    int leaf;
    int keynum;
    string keyvalues[MAXNUM_KEYs+1];
    filepoint childpoint[MAXNUM_CHILDs+1];

public:
    Snode(){
        leaf = 0;
        keynum = 0;
        childpoint[keynum] = 0;
    }
    void clear(){
        leaf = 0;
        keynum = 0;
        childpoint[keynum] = 0;
    }
    ~Snode(){

    }
    void printData(){
        std::cout<<"leaf:"<<leaf<<std::endl;
        std::cout<<"keynum:"<<keynum<<std::endl;
        for (int i=0;i<keynum;i++){
            std::cout<<keyvalues[i]<<" ";
        }
        std::cout<<std::endl;
        for (int i=0;i<keynum+1;i++){
            std::cout<<childpoint[i]<<" ";
        }
        std::cout<<std::endl;
    }

    void writeToFile(char* t){
        char *s = t;
        *((int*)s) = leaf;
        s=s+4;
        *((int*)(s)) = keynum;
        s=s+4;
        for (int i=0;i<MAXNUM_KEYs;++i){
            strcpy (s,keyvalues[i].c_str ());
            s=s+256;
        }
        for (int i=0;i<MAXNUM_CHILDs;++i){
            *(( int*)(s)) = childpoint[i];
            s=s+8;
        }
    }

    void readFromFile(const char* t){
        const char* s =t;
        leaf = *(int*)s;s=s+4;
        keynum = *(int*)s;s=s+4;
        for (int i=0;i<MAXNUM_KEYs;++i){
            keyvalues[i]=s;
            s=s+256;
        }
        for (int i=0;i<MAXNUM_CHILDs;++i){
            childpoint[i]= *(( int*)(s));
            s=s+8;
        }
    }

    int getLeaf() const {
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

    string getKeyValue(int i) const {
        return keyvalues[i];
    }

    void setKeyValue(int i, string key){
        keyvalues[i] = key;
    }


    int getKeyIndex(string key)const{        // 找到键值在结点中存储的下标
        if (keynum ==0) return 0;
        for (int i=0;i<keynum;i++){
            if (key<keyvalues[i]) return i;
        }
        return keynum;
    }

    int getChildIndex(filepoint p)const{
        for (int i=0;i<keynum+1;i++){
            if (p==childpoint[i]) return i;
        }
        return 0;
    }

    filepoint getChild(int i) const{
        return childpoint[i];
    }
    void setChild(int i,filepoint p){
        childpoint[i] = p;
    }

    void removeKey(int keyIndex, int childIndex){  // 从结点中移除键值
        int i;
        keynum--;
        for (i=keyIndex;i<keynum;++i){
            keyvalues[i] = keyvalues[i+1];
        }
        for (i=childIndex;i<keynum+1;++i){
            childpoint[i] = childpoint[i+1];
        }
    }

    void remove(string key,filepoint p){
        int kid = getKeyIndex(key);
        int cid = getChildIndex (p);
        if (kid>0 && keyvalues[kid-1]==key)
        removeKey (kid-1,cid);
    }

    void insert(int keyIndex, int childIndex,const string &key,const filepoint &p){
        int i;
        for (i=keynum;i>keyIndex;--i){
            keyvalues[i] = keyvalues[i-1];
        }
        keyvalues[keyIndex] = key;
        for (i=keynum+1;i>childIndex;--i){
            childpoint[i] = childpoint[i-1];
        }
        childpoint[childIndex] = p;
        keynum++;
    }

    void insert(string key, const filepoint& data)//在叶子节点插入数据
    {
        int i;
        for (i=keynum; i>=1 && keyvalues[i-1]>key; --i){
            keyvalues[i] = keyvalues[i-1];
            childpoint[i+1] =childpoint[i];
        }
        keyvalues[i]=key;
        childpoint[i+1]=childpoint[i];
        childpoint[i]=data;
        keynum++;
    }
};
#endif // SNODE_H
