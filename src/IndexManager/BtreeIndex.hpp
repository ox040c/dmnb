#ifndef BtreeIndex_H_
#define BtreeIndex_H_
#include "Tnode.hpp"
#include "BufferManager.hpp"
#include <vector>
#include <map>
typedef unsigned int filepoint;
extern const int ORDER;                   // B+树的阶（非根内结点的最小子树个数）
extern const int MINNUM_KEY;        // 最小键值个数
extern const int MAXNUM_KEY;      // 最大键值个数
extern const int MINNUM_CHILD; // 最小子树个数
extern const int MAXNUM_CHILD; // 最大子树个数
extern const int MINNUM_LEAF;    // 最小叶子结点键值个数
extern const int MAXNUM_LEAF;    // 最大叶子结点键值个数

template <class KeyType>
class BtreeIndex {
private:
    Tnode<KeyType> *root;
    Tnode<KeyType> *first;
    filepoint rootloc;
    filepoint firstloc;
    struct DataAddr fp;
    BufferManager buff;
    std::map<filepoint,filepoint> father;

public:
    BtreeIndex(){

    }

    BtreeIndex(const BufferManager &x,filepoint rootloc,filepoint firstloc){
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
        fp.filename = "index.data";
        fp.datalen = 4096;
    }
    void clear();             // 清空
    void print()const;        // 打印树关键字
    void printData()const;    // 打印数据

    filepoint WritetoFile(Tnode<KeyType> x){
        char* s = new char[4096];
        x.WritetoFile(s);
        delete[] s;
        struct DataAddr fp1;
        fp1 = buff.Insert (fp,s);
        return fp1.dataaddr;
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

    void UpdateFile(filepoint p,Tnode<KeyType> x){
        char* s = new char[4096];
        x.WritetoFile (s);
        fp.dataaddr = p;
        buff.Update(fp,s);
        delete[] s;
    }
    void RemoveFile(filepoint p){
        fp.dataaddr = p;
        buff.Delete(fp);
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
                father.insert (std::pair<filepoint,filepoint>(nextnode,tempnode));
                temp = ReadFromFile (nextnode);
                tempnode = nextnode;
            }
            else{
                nextnode = temp.getChild (keyindex);
                father.insert (std::pair<filepoint,filepoint>(nextnode,tempnode));
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
                    father.insert(std::pair<filepoint,filepoint>(nextp,temp));
                    tempnode = ReadFromFile (nextp);
                    temp = nextp;
                }
                else{
                    nextp = tempnode.getChild (keyindex);
                    father.insert(std::pair<filepoint,filepoint>(nextp,temp));
                    tempnode = ReadFromFile (nextp);
                    temp = nextp;
                }
            }
            return temp;
        }else std::cout<<"error in index findleaf"<<std::endl;
    }

//    vector<filepoint> select(KeyType compareKey, int compareOpeartor){    // 范围查询，BETWEEN

//    }

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
            filepoint parent = father[x];
            Tnode<KeyType> parentnode;
            if(parentnode.getKeyNum ()<MAXNUM_KEY){
                int childindex = parentnode.getChildIndex (x);
                parentnode.insert (childindex,childindex+1,key,y);
            }else{
                parentnode.insert (key,y);
                Tnode<KeyType> newnode;
                KeyType newkey = parentnode.getKeyValue (ORDER);
                for (int i=0;i+ORDER<MAXNUM_CHILD;++i){
                    newnode.setChild (i,parentnode.getChild (i+ORDER));
                    newnode.setKeyValue (i,parentnode.getKeyValue (i+ORDER));
                }
                filepoint newp = WritetoFile (newnode);
                insert_in_parent (parent,newkey,newp);
            }
            return true;
        }
        return false;
    }

    bool insert(const KeyType& key, const filepoint& data){
        if (root==NULL){
            root = new Tnode<KeyType>();
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
        if (tempnode.getKeyNum ()<MAXNUM_KEY){
            insert_in_leaf(tempnode,key,data);
            UpdateFile (temp,tempnode);
        }else{
            Tnode<KeyType> newnode;
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

    bool remove(KeyType key){
        if (root==NULL){
            return false;
        }                                                                          //空树
        Tnode<KeyType> tempnode;
        filepoint temp;
        int keyindex;
        temp = findLeaf (key);
        tempnode = ReadFromFile (temp);
        keyindex = tempnode.getKeyIndex (key);
        if (keyindex>0 && key == tempnode.getKeyValue (keyindex-1)){
            remove_entry (temp,key,tempnode.getChild (keyindex-1),tempnode);
        }
    }

    bool remove_entry(filepoint x,KeyType key,filepoint p,Tnode<KeyType> xnode){
        xnode.remove (key,p);
        if (x==rootloc && root->getKeyNum ()==0){
            filepoint pt = rootloc;
            rootloc = root->getChild (0);
            root = new Tnode<KeyType>(ReadFromFile (rootloc));
            RemoveFile (pt);
        }else{
            if (xnode.getKeyNum ()==MINNUM_KEY){
                bool flag = false;
                filepoint parent = father.find(x);
                filepoint leftp,rightp;
                KeyType tmpk;
                Tnode<KeyType> left,right;
                Tnode<KeyType> parentnode = ReadFromFile (parent);
                int cid = parentnode.getChildIndex (x);
                if (cid>0) {//左子合并
                    leftp = parentnode.getChild (cid-1);
                    left = ReadFromFile (leftp);
                    tmpk = parentnode.getKeyValue (cid-1);
                    if (left.getKeyNum ()+xnode.getKeyNum ()<MAXNUM_KEY){
                        right = xnode;
                        rightp = x;
                        if (right.getLeaf ()!=1){
                            left.insert (left.getKeyNum (),left.getKeyNum ()+1,tmpk,right.getChild (0));
                            for (int i=0;i<right.getKeyNum ();++i){
                                left.insert (left.getKeyNum (),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                            }
                        }
                        else{
                            for (int i=0;i<right.getKeyNum ();++i){
                                left.insert (left.getKeyNum (),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                            }
                            left.setChild (MAXNUM_CHILD-1,right.getChild (MAXNUM_CHILD-1));
                        }

                        remove_entry (parent,tmpk,rightp,parentnode);
                        RemoveFile (rightp);
                        flag =true;
                    }
                }
                if (cid<parentnode.getKeyNum ()) {//右子合并
                    rightp = parentnode.getChild (cid+1);
                    right = ReadFromFile (rightp);
                    tmpk = parentnode.getKeyValue (cid);
                    if (right.getKeyNum ()+xnode.getKeyNum ()<MAXNUM_KEY){
                        left = xnode;
                        leftp = x;
                        if (right.getLeaf ()!=1){
                            left.insert (left.getKeyNum (),left.getKeyNum ()+1,tmpk,right.getChild (0));
                            for (int i=0;i<right.getKeyNum ();++i){
                                left.insert (left.getKeyNum (),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                            }
                        }
                        else{
                            for (int i=0;i<right.getKeyNum ();++i){
                                left.insert (left.getKeyNum (),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                            }
                            left.setChild (MAXNUM_CHILD-1,right.getChild (MAXNUM_CHILD-1));
                        }
                        remove_entry (parent,tmpk,rightp,parentnode);
                        RemoveFile (rightp);
                        flag =true;
                    }
                }
                if (!flag){
                    if (cid>0){//左边
                        right = xnode;
                        tmpk = parentnode.getKeyValue (cid-1);
                        if (xnode.getLeaf ()!=1){
                            int m = left.getKeyNum ();
                            right.insert (0,0,tmpk,left.getChild (m));
                            parentnode.setKeyValue (cid-1,left.getKeyValue (m-1));
                            left.removeKey (m-1,m);
                        }
                        else{
                            int m = left.getKeyNum ();
                            right.insert (0,0,left.getKeyValue (m-1),left.getChild (m-1));
                            parentnode.setKeyValue (cid-1,left.getKeyValue (m-1));
                            left.removeKey (m-1,m-1);
                        }
                    }else{
                        if(cid<parentnode.getKeyNum ()){//右边
                            left = xnode;
                            tmpk = parentnode.getKeyValue (cid);
                            if (xnode.getLeaf ()!=1){
                                int m = left.getKeyNum ();
                                left.insert (m,m+1,tmpk,right.getChild (0));
                                parentnode.setKeyValue (cid,right.getKeyValue (0));
                                left.removeKey (0,0);
                            }
                            else{
                                int m = left.getKeyNum ();
                                left.insert (m,m,right.getKeyValue (0),right.getChild (0));
                                parentnode.setKeyValue (cid,right.getKeyValue (0));
                                left.removeKey (0,0);
                            }
                        }
                    }
                }
            }
        }
    }
};
#endif

