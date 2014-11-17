#ifndef TNODE_H_
#define TNODE_H_

#define NULL 0
#define filepoint int
enum NODE_TYPE{INTERNAL, LEAF};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION{LEFT, RIGHT};   // 兄弟结点方向：左兄弟结点、右兄弟结点
typedef float KeyType;                 // 键类型
typedef int DataType;                  // 值类型
const int ORDER = 20;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1;        // 最小键值个数
const int MAXNUM_KEY = 2*ORDER-1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数

// 结点基类
template <class KeyType>
class TNode{
protected:
    bool leaf;
    int keynum;
    KeyType keyvalues[MAXNUM_KEY];
    filepoint childpoint[MAXNUM_CHILD];
    filepoint sibling;
    filepoint self;
    BufferManager &bufferManager;

public:
    TNode(BufferManager &x){
        bufferManager = x;
        leaf = 0;
        keynum = 0;
        self = bufferManager.alloc();

    }

    void setRightSibling(filepoint s){
        sibling =s;
    }
    filepoint getRightSibling(){
        return sibling;
    }

    virtual ~TNode();
    TNode getFromFile(filepoint p){

    }


    filepoint getself(){
        return self;
    }

    filepoint writeToFile(){
        bufferManager.write(xx,xxx,xx)

    }

    void clear(){

    }

    bool isLeaf() const {
        return leaf;
    }

    bool setLeaf(bool type){
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
        if (!leaf) return childpoint[i];
    }
    filepoint getData(int i) const{
        if (leaf) return childpoint[i];
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

    void insert(KeyType key, const filepoint& data)
    {
        int i;
        for (i=keynum; i>=1 && m_KeyValues[i-1]>key; --i)
        {
            setKeyValue(i, keyvalues[i-1]);
            setData(i, childpoint[i-1]);
        }
        setKeyValue(i, key);
        setChild(i, data);
        keynum++;
    }

    void split(CNode* parentNode, int childIndex){ // 分裂结点
        if (!leaf){
            Tnode* newnode = new <KeyType>Tnode();//分裂后的右节点
            newNode->setKeyNum(MINNUM_KEY);
            int i;
            for (i=0; i<MINNUM_KEY; ++i){// 拷贝关键字的值
                newNode->setKeyValue(i, keyvalues[i+MINNUM_CHILD]);
            }
            for (i=0; i<MINNUM_CHILD; ++i){ // 拷贝孩子节点指针
                newNode->setChildpoint(i, childpoint[i+MINNUM_CHILD]);
            }
            setKeyNum(MINNUM_KEY);  //更新左子树的关键字个数
            parentNode->insert(childIndex, childIndex+1, m_KeyValues[MINNUM_KEY], newNode->getself());
        } else{
            Tnode *newNode = new <KeyType>Tnode();//分裂后的右节点
            setKeyNum(MINNUM_LEAF);
            newNode->setKeyNum(MINNUM_LEAF+1);
            newNode->setRightSibling(getRightSibling());
            setRightSibling(newNode->getself());
            int i;
            for (i=0; i<MINNUM_LEAF+1; ++i){// 拷贝关键字的值
                newNode->setKeyValue(i, keyvalues[i+MINNUM_LEAF]);
            }
            for (i=0; i<MINNUM_LEAF+1; ++i){// 拷贝数据
                newNode->setChild(i, childpoint[i+MINNUM_LEAF]);
            }
            parentNode->insert(childIndex, childIndex+1, m_KeyValues[MINNUM_LEAF], newNode->getself());
        }
    }

    void mergeChild(TNode* parentNode, TNode* childNode, int keyIndex){  // 合并结点
        if (!leaf){
            insert(keynum, keynum+1, parentNode->getKeyValue(keyIndex), childNode->getChild(0));
            int i;
            for (i=1; i<=childNode->getKeyNum(); ++i) {
                insert(keynum+i, keynum+i+1, childNode->getKeyValue(i-1), childNode->getChild(i));
            }
            //父节点删除index的key
            parentNode->removeKey(keyIndex, keyIndex+1);
            bufferManager.erase(parentNode->getChild(keyIndex+1));

        }
        else{
            for (int i=0; i<childNode->getKeyNum(); ++i){
                insert(childNode->getKeyValue(i), ((CLeafNode*)childNode)->getData(i));
            }
            setRightSibling(childNode->getRightSibling());
            //父节点删除index的key，
            parentNode->removeKey(keyIndex, keyIndex+1);
            bufferManager.erase(childNode->getself());
        }
    }

    void borrowFrom(TNode* siblingNode, TNode* parentNode, int keyIndex, int d) {// 从兄弟结点中借一个键值
        if (leaf){
            switch(d) {
               case 0:{  // 从左兄弟结点借
                   insert(siblingNode->getKeyValue(siblingNode->getKeyNum()-1),siblingNode->getData(siblingNode->getKeyNum()-1));
                   siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum()-1);
                   parentNode->setKeyValue(keyIndex, keyvalues[0]);
                   break;
                }

               case 1:{  // 从右兄弟结点借
                   insert(siblingNode->getKeyValue(0),siblingNode->getData(0));
                   siblingNode->removeKey(0, 0);
                   parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
                   break;
               }
           }
        }else{
            switch(d){
                case 0:{  // 从左兄弟结点借
                    insert(0, 0,parentNode->getKeyValue(keyIndex),siblingNode->getChild(siblingNode->getKeyNum()));
                    parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum()));
                    siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum());
                    break;
                }
                case 1:{  // 从右兄弟结点借
                    insert(keynum, keynum+1, parentNode->getKeyValue(keyIndex), (siblingNode)->getChild(0));
                    parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
                    siblingNode->removeKey(0, 0);
                    break;
                 }
            }

        }

    }

};
#endif
