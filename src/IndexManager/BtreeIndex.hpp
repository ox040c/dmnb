#ifndef BtreeIndex_H_
#define BtreeIndex_H_
#include "Tnode.hpp"
#include "utility.hpp"
#include "BufferManager.hpp"
#include <vector>
#include <map>
#include <list>
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
    Tnode<KeyType> *root=NULL;
    Tnode<KeyType> *first=NULL;
    filepoint rootloc;
    filepoint firstloc;
    struct DataAddr fp;
    BufferManager buff;
    std::map<filepoint,filepoint> father;
    int top;
    Tnode<KeyType> rem[400];


public:
    BtreeIndex(){
        rootloc = 0;
        firstloc = 0;
        fp.filename = "index.txt";
        fp.datalen = 4096;
        top = 0;
    }

//    BtreeIndex(const BufferManager &x,filepoint rootloc,filepoint firstloc){
//        fp.filename = "index.data";
//        fp.datalen = 4096;
//        root = new Tnode<KeyType>(ReadFromFile (rootloc));
//        firstloc = new Tnode<KeyType>(ReadFromFile (firstloc));
//        buff = &x;
//    }

//    BtreeIndex(const BufferManager &x){
//        fp.filename = "index.data";
//        fp.datalen = 4096;
//        root = NULL;
//        firstloc = NULL;
//        buff = &x;
//    }

    ~BtreeIndex(){
        rootloc = 0;
        firstloc = 0;
    }
    filepoint getRoot(){
        return rootloc;
    }
    filepoint getFirst(){
        return firstloc;
    }

    void clear(){
        firstloc = 0;
        rootloc = 0;
        father.clear ();
    }
        // 清空
    void print(){        // 打印树关键字
        std::cout<<"============BtreeIndex=========="<<std::endl;
        if (rootloc!=0)
            printData(rootloc);
        std::cout<<"============BtreeIndex=========="<<std::endl;
    }
    void printData(filepoint p){    // 打印数据
        Tnode<KeyType> tmpnode;
        filepoint pt;
        std::cout<<"this point:"<<p<<std::endl;
        if (p!=0) {
            tmpnode = ReadFromFile (p);
            tmpnode.printData ();
            std::cout<<std::endl;
            if (tmpnode.getLeaf ()==0){
                for (int i=0;i<tmpnode.getKeyNum ();++i){
                    pt = tmpnode.getChild (i);
                    if (pt != 0) printData (pt);
                }
                pt = tmpnode.getChild (tmpnode.getKeyNum ());
                if (pt != 0) printData (pt);
            }
        }
    }

    filepoint WritetoFile(Tnode<KeyType> &x){
//        char* s = new char[fp.datalen];
//        x.WritetoFile(s);
//        struct DataAddr fp1;
//        fp1 = buff.Insert (fp,s);
//        delete[] s;
//        return fp1.dataaddr;
        top++;
        rem[top] = x;
        return top;
    }

    Tnode<KeyType> ReadFromFile (filepoint p){
        Tnode<KeyType> tempnode;
//        fp.dataaddr = p;
//        char* s = new char[fp.datalen];
//        buff.Search(fp,s);
//        tempnode.ReadFromFile(s);
//        delete[] s;
//        return tempnode;
        tempnode = rem[p];
        return tempnode;
    }

    void UpdateFile(filepoint p,Tnode<KeyType> x){
        rem[p] = x;
//        char* s = new char[fp.datalen];
//        x.WritetoFile (s);
//        fp.dataaddr = p;
//        buff.Update(fp,s);
//        delete[] s;
    }
    void RemoveFile(filepoint p){
//        fp.dataaddr = p;
//        buff.Delete(fp);
//        delete rem[p];
    }

    std::list<filepoint> findless(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint temp;
        std::list<filepoint> ans;
        if (firstloc ==0){
            return ans;
        }
        if (first == NULL) return ans;
        temp = firstloc;
        while (temp!=0){
            tempnode = ReadFromFile (temp);
            if (tempnode.getLeaf ()==0) {
                std::cout<<"wrong in findless"<<std::endl;
                return ans;
            }
            int kid = tempnode.getKeyIndex (key);
            for (int i=0;i<kid;++i) ans.push_back (tempnode.getChild (i));
            if (tempnode.getKeyValue (kid-1)==key) ans.pop_back ();
            if (kid>0 && kid == tempnode.getKeyNum ())
                temp = tempnode.getChild (kid);
            else
                return ans;
        }
        return ans;
    }
    std::list<filepoint> findlesseq(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint temp;
        std::list<filepoint> ans;
        if (firstloc ==0){
            return ans;
        }
        if (first == NULL) return ans;
        temp = firstloc;
        while (temp!=0){
            tempnode = ReadFromFile (temp);
            if (tempnode.getLeaf ()==0) {
                std::cout<<"wrong in findless"<<std::endl;
                return ans;
            }
            int kid = tempnode.getKeyIndex (key);
            for (int i=0;i<kid;++i) ans.push_back (tempnode.getChild (i));
            if (kid>0 && kid == tempnode.getKeyNum ())
                temp = tempnode.getChild (kid);
            else
                return ans;
        }
        return ans;
    }
    filepoint findLeaf(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint nextp,temp;
        int keyindex;
        int keynum;
        if (root != NULL){
            delete root;
            root = new Tnode<KeyType>(ReadFromFile(rootloc));
            tempnode = *root;
            temp = rootloc;
//            std::cout<<"_________find______"<<std::endl;
//            tempnode.printData ();
//            std::cout<<"___________________"<<std::endl;
            while (tempnode.getLeaf ()==0){
                keyindex = tempnode.getKeyIndex (key);
                keynum = tempnode.getKeyNum ();
//                if (keyindex==keynum){
//                    nextp = tempnode.getChild (keynum);
//                    father[nextp]=temp;
//                    tempnode = ReadFromFile (nextp);
//                    temp = nextp;
//                }
//                else{
                    nextp = tempnode.getChild (keyindex);
                    father[nextp]=temp;
                    tempnode = ReadFromFile (nextp);
                    temp = nextp;
            }
            return temp;
        }else std::cout<<"error in index findleaf"<<std::endl;
    }
    std::list<filepoint> findbigeq(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint temp;
        std::list<filepoint> ans;
        temp = findLeaf (key);
        while (temp!=0){
            tempnode = ReadFromFile (temp);
            if (tempnode.getLeaf ()==0) {
                std::cout<<"wrong in findless"<<std::endl;
                return ans;
            }
            int kid = tempnode.getKeyIndex (key);
            if (kid>0 && tempnode.getKeyValue (kid-1)==key)
                ans.push_back (tempnode.getChild (kid-1));
            for (int i=kid;i<tempnode.getKeyNum ();++i)
                ans.push_back (tempnode.getChild (i));
            temp = tempnode.getChild (tempnode.getKeyNum ());
        }
        return ans;
    }
    std::list<filepoint> findbig(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint temp;
        temp = findLeaf (key);
        std::list<filepoint> ans;
        while (temp!=0){
            tempnode = ReadFromFile (temp);
            if (tempnode.getLeaf ()==0) {
                std::cout<<"wrong in findless"<<std::endl;
                return ans;
            }
            int kid = tempnode.getKeyIndex (key);
            for (int i=kid;i<tempnode.getKeyNum ();++i)
                ans.push_back (tempnode.getChild (i));
            temp = tempnode.getChild (tempnode.getKeyNum ());
        }
        return ans;
    }
    std::list<filepoint> findeq(const KeyType &key){
        Tnode<KeyType> tempnode;
        filepoint temp;
        std::list<filepoint> ans;
        temp = findLeaf (key);
        tempnode = ReadFromFile (temp);
        if (tempnode.getLeaf ()==0) {
            std::cout<<"wrong in findless"<<std::endl;
            return ans;
        }
        int kid = tempnode.getKeyIndex (key);
        if (kid>0 && tempnode.getKeyValue (kid-1)==key)
            ans.push_back (tempnode.getChild (kid-1));
        return ans;
    }
    //    vector<filepoint> select(KeyType compareKey, int compareOpeartor){    // 范围查询，BETWEEN

//    }

    bool insert_in_leaf(Tnode<KeyType> &node,const KeyType &key,const filepoint& data){
        node.insert (key,data);
        return true;
    }

    bool insert_in_parent(filepoint x,const KeyType &key,filepoint y){
        if (x == rootloc){
//            std::cout<<"insert_in_parent"<<std::endl;
            Tnode<KeyType> newroot;
            newroot.setKeyNum (1);
            newroot.setChild (0,x);
            newroot.setChild (1,y);
            newroot.setKeyValue (0,key);            
            rootloc = WritetoFile (newroot);
//            delete root;
//            root = new Tnode<KeyType>(ReadFromFile(rootloc));
            return true;
        }else{
            filepoint parent = father[x];
//            std::cout<<"fid: "<<parent<<std::endl;
            Tnode<KeyType> parentnode=ReadFromFile (parent);
            if(parentnode.getKeyNum ()<MAXNUM_KEY){
                int childindex = parentnode.getChildIndex (x);
                parentnode.insert (childindex,childindex+1,key,y);
                UpdateFile (parent,parentnode);
            }else{
                int kid = parentnode.getKeyIndex (key);
//                std::cout<<"key="<<key<<std::endl;
//                std::cout<<"kid="<<kid<<std::endl;
//                parentnode.printData ();
                parentnode.insert (kid,kid+1,key,y);
//                parentnode.printData ();
                Tnode<KeyType> newnode;
                for (int i=0;i<ORDER;++i){
                    newnode.insert (i,i,parentnode.getKeyValue (ORDER),parentnode.getChild (ORDER));
                    parentnode.removeKey (ORDER,ORDER);
                }
                newnode.setChild (ORDER,parentnode.getChild (ORDER));
                filepoint newp = WritetoFile (newnode);
                KeyType newkey = parentnode.getKeyValue (ORDER-1);
                parentnode.removeKey (ORDER-1,ORDER);
                UpdateFile (parent,parentnode);
//                std::cout<<"    left:>>>>"<<std::endl;
//                parentnode.printData ();
//                std::cout<<"    right:>>>>"<<std::endl;
//                newnode.printData ();
                filepoint fid = father[parent];
                Tnode<KeyType> fnode = ReadFromFile (fid);
//                std::cout<<"    father:>>>>"<<std::endl;
//                fnode.printData ();
                insert_in_parent (parent,newkey,newp);
            }
            return true;
        }
        return false;
    }

    bool insert(const KeyType& key, const filepoint& data){
//        std::cout<<"-----ok in-----"<<std::endl;
        if (root==NULL){
//            std::cout<<"-----root NULL-----"<<std::endl;
            root = new Tnode<KeyType>;
            first = root;
            root->insert (key,data);
            root->setLeaf (1);

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
//        std::cout<<"leaf_id::"<<temp<<std::endl;
//        std::cout<<"keynum::"<<tempnode.getKeyNum ()<<std::endl;
//        std::cout<<"kid::"<<keyindex<<std::endl;
        if (keyindex>0 && key == tempnode.getKeyValue (keyindex-1)){
            return false;
        }                                                                       //已经在树中
        if (tempnode.getKeyNum ()<MAXNUM_KEY){   //不分裂插入
//            std::cout<<"notspl::"<<std::endl;
            insert_in_leaf(tempnode,key,data);
            UpdateFile (temp,tempnode);
        }else{                                  //裂开点
//            std::cout<<"splite::"<<std::endl;
            Tnode<KeyType> newnode;
            insert_in_leaf(tempnode,key,data);
            newnode.setLeaf (1);
            for (int i=0;i<ORDER+1;++i){
                newnode.setKeyValue (i,tempnode.getKeyValue (ORDER-1));
                newnode.setChild (i,tempnode.getChild (ORDER-1));
                tempnode.removeKey (ORDER-1,ORDER-1);
            }
            newnode.setChild (ORDER+1,tempnode.getChild (ORDER-1));
            newnode.setKeyNum (ORDER+1);
            tempnode.setKeyNum (ORDER-1);
            filepoint newp = WritetoFile (newnode);
            tempnode.setChild (ORDER-1,newp);
            UpdateFile (temp,tempnode);
            KeyType newkey = newnode.getChild (0);
//            std::cout<<"left:>>>>"<<std::endl;
//            tempnode.printData ();
//            std::cout<<"right:>>>>"<<std::endl;
//            newnode.printData ();
//            filepoint fid = father[temp];
//            Tnode<KeyType> fnode = ReadFromFile (fid);
//            std::cout<<"father:>>>>"<<std::endl;
//            fnode.printData ();
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
//        std::cout<<"remove"<<"key="<<key<<"point:"<<p<<std::endl;
//        xnode.printData ();
        UpdateFile (x,xnode);
        //到根节点了就1孩子
        if (x==rootloc && root->getKeyNum ()==0){
            filepoint pt = rootloc;
            rootloc = root->getChild (0);
            RemoveFile (pt);
//            delete root;
//            root = new Tnode<KeyType>(ReadFromFile (rootloc));
            return true;
        }
        //孩子太少了
        if (x!=rootloc && xnode.getKeyNum ()<MINNUM_KEY){
            filepoint parent = father[x];
            Tnode<KeyType> parentnode = ReadFromFile (parent);
//            std::cout<<"<parent>"<<std::endl;
//            parentnode.printData ();
            filepoint leftp,rightp;
            KeyType tmpk;
            Tnode<KeyType> left,right;
            int cid = parentnode.getChildIndex (x);
//            std::cout<<"<cid> "<<cid<<std::endl;
            //向左边借点
            if (cid>0) {
                rightp = x;
                right = xnode;
                leftp = parentnode.getChild (cid-1);
                left = ReadFromFile (leftp);
                tmpk = parentnode.getKeyValue (cid-1);
//                left.printData ();
//                right.printData ();
                if (left.getKeyNum ()>MINNUM_KEY){
//                    std::cout<<"------borrow left"<<std::endl;

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
                    UpdateFile (leftp,left);
                    UpdateFile (rightp,right);
                    UpdateFile (parent,parentnode);
                    return true;
                }
            }
            //向右边借点
            if(cid<parentnode.getKeyNum ()){
                rightp = parentnode.getChild (cid+1);
                right = ReadFromFile (rightp);
                tmpk = parentnode.getKeyValue (cid);
                left = xnode;
                leftp = x;
                if (right.getKeyNum ()>MINNUM_KEY){
//                    std::cout<<"------borrow right"<<std::endl;
                    left = xnode;
                    tmpk = parentnode.getKeyValue (cid);
                    if (xnode.getLeaf ()!=1){
                        int m = left.getKeyNum ();
                        left.insert (m,m+1,tmpk,right.getChild (0));
                        parentnode.setKeyValue (cid,right.getKeyValue (0));
                        right.removeKey (0,0);
                    }
                    else{
                        int m = left.getKeyNum ();
                        left.insert (m,m,right.getKeyValue (0),right.getChild (0));
                        right.removeKey (0,0);
                        parentnode.setKeyValue (cid,right.getKeyValue (0));
                    }
                    UpdateFile (leftp,left);
                    UpdateFile (rightp,right);
                    UpdateFile (parent,parentnode);
                    return true;
                }
            }
            //向左边合
            if (cid>0) {
//                std::cout<<"------com left"<<std::endl;
                leftp = parentnode.getChild (cid-1);
                left = ReadFromFile (leftp);
                tmpk = parentnode.getKeyValue (cid-1);
                right = xnode;
                rightp = x;
//                //======================================
//                std::cout<<">0>left>>"<<std::endl;
//                left.printData ();
//                std::cout<<">0>this_right>>"<<std::endl;
//                right.printData ();
//                std::cout<<">0>parent"<<std::endl;
//                parentnode.printData ();
//                //======================================
                if (right.getLeaf ()!=1){
                    left.insert (left.getKeyNum (),left.getKeyNum()+1,tmpk,right.getChild (0));
                    for (int i=0;i<right.getKeyNum ();++i){
                        left.insert (left.getKeyNum(),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                    }
                    UpdateFile (leftp,left);
                }
                else{
                    for (int i=0;i<right.getKeyNum ();++i){
                        left.insert (left.getKeyNum(),left.getKeyNum (),right.getKeyValue (i),right.getChild (i));
                    }
                    left.setChild (left.getKeyNum (),right.getChild(right.getKeyNum ()));
                    UpdateFile (leftp,left);
                }
                remove_entry (parent,tmpk,rightp,parentnode);
//                //======================================
//                std::cout<<">1>left>>"<<std::endl;
//                left.printData ();
//                std::cout<<">1>this_right"<<std::endl;
//                right.printData ();
//                std::cout<<">1>parent"<<std::endl;
//                parentnode.printData ();
//                //======================================
                RemoveFile (rightp);
                return true;
            }
            //右子合并
            if (cid<parentnode.getKeyNum ()) {
//                std::cout<<"------com right"<<std::endl;
                rightp = parentnode.getChild (cid+1);
                right = ReadFromFile (rightp);
                tmpk = parentnode.getKeyValue (cid);
                left = xnode;
                leftp = x;
//                //======================================
//                std::cout<<"left>>"<<std::endl;
//                left.printData ();
//                std::cout<<"this_right"<<std::endl;
//                right.printData ();
//                std::cout<<"parent"<<std::endl;
//                parentnode.printData ();
//                //======================================
                if (right.getLeaf ()!=1){
                    left.insert (left.getKeyNum (),left.getKeyNum()+1,tmpk,right.getChild (0));
                    for (int i=0;i<right.getKeyNum ();++i){
                        left.insert (left.getKeyNum(),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                    }
                    UpdateFile (leftp,left);
                }
                else{
                    for (int i=0;i<right.getKeyNum ();++i){
                        left.insert (left.getKeyNum(),left.getKeyNum ()+1,right.getKeyValue (i),right.getChild (i+1));
                    }
                    left.setChild (MAXNUM_CHILD-1,right.getChild(MAXNUM_CHILD-1));
                    UpdateFile (leftp,left);
                }
                remove_entry (parent,tmpk,rightp,parentnode);
//                //======================================
//                std::cout<<"left>>"<<std::endl;
//                left.printData ();
//                std::cout<<"this_right"<<std::endl;
//                right.printData ();
//                std::cout<<"parent"<<std::endl;
//                parentnode.printData ();
//                //======================================
                RemoveFile (rightp);
                return true;
            }
        }//孩子太少
    }
};
#endif

